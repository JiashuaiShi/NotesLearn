# POCO NetSSL_OpenSSL 模块详解

`NetSSL_OpenSSL` 模块为 POCO 的 `Net` 模块提供了基于 OpenSSL 库的 SSL (Secure Sockets Layer) 和 TLS (Transport Layer Security) 支持。它使得 `Net` 模块中的网络类（如 HTTP 客户端/服务器、TCP 套接字）能够进行安全的、加密的通信。

**前提条件:** 与 `Crypto` 模块类似，使用此模块需要系统中已安装 OpenSSL 开发库，并且在编译 POCO 时启用了 `NetSSL_OpenSSL` 支持。

## 主要功能

1.  **SSL/TLS 上下文管理 (`Context`):**
    *   `Context` 类封装了 OpenSSL 的 `SSL_CTX`，用于配置 SSL/TLS 的参数，如协议版本 (TLSv1.2, TLSv1.3 等)、私钥、证书、证书颁发机构 (CA) 列表、密码套件 (cipher suites) 等。
    *   区分客户端 (`Context::CLIENT_USE`) 和服务器端 (`Context::SERVER_USE`) 配置。
2.  **安全套接字 (`SecureStreamSocket`, `SecureServerSocket`):**
    *   `SecureStreamSocket`: 继承自 `Net::StreamSocket`，增加了 SSL/TLS 功能的客户端 TCP 套接字。在建立连接后执行 SSL/TLS 握手。
    *   `SecureServerSocket`: 继承自 `Net::ServerSocket`，用于创建接受 SSL/TLS 连接的服务器端套接字。
3.  **HTTPS 支持:**
    *   `HTTPSClientSession`: 继承自 `Net::HTTPClientSession`，用于建立到 HTTPS 服务器的安全连接。
    *   `HTTPServer`: `Net::HTTPServer` 可以通过配置 `SecureServerSocket` 来支持 HTTPS 连接。
    *   `HTTPSSessionInstantiator`: 用于 `HTTPClientSession` 内部创建 HTTPS 连接。
4.  **SSL/TLS 会话管理 (`Session`):**
    *   封装了 OpenSSL 的 `SSL` 对象，代表一个具体的 SSL/TLS 连接会话。
5.  **证书验证:**
    *   提供了配置证书验证选项的功能，包括验证对等方证书、检查主机名等。
    *   `InvalidCertificateHandler`, `PrivateKeyPassphraseHandler`: 用于处理证书错误和私钥密码的回调接口。
6.  **初始化与清理:**
    *   `SSLManager`: 负责 OpenSSL 库的全局初始化和清理工作，管理默认的 `Context` 对象。

## 设计特点

*   **集成 Net 模块:** 无缝地为 `Net` 模块的网络类添加安全层，API 使用方式与非安全版本类似。
*   **封装 OpenSSL:** 将复杂的 OpenSSL C API 封装成易于使用的 C++ 类。
*   **配置驱动:** 通过 `Context` 对象集中配置 SSL/TLS 参数。
*   **RAII:** 使用 RAII 管理 OpenSSL 资源。

## 关键组件关系示例 (HTTPS 客户端)

以下 Mermaid 图展示了使用 `HTTPSClientSession` 连接 HTTPS 服务器的基本流程：

```mermaid
sequenceDiagram
    participant App as 应用程序代码
    participant Context as Poco::Net::Context
    participant SSLManager as Poco::Net::SSLManager
    participant HTTPSSession as Poco::Net::HTTPSClientSession
    participant SecureSocket as Poco::Net::SecureStreamSocket
    participant OpenSSL as OpenSSL 库
    participant Server as HTTPS 服务器

    App->>SSLManager: initializeClient() (确保 OpenSSL 初始化)
    App->>Context: 创建 Context(CLIENT_USE, ...)
    App->>Context: 配置 (私钥, 证书, CA, 验证模式等)
    App->>HTTPSSession: 创建 HTTPSClientSession(host, port, context)
    activate HTTPSSession
    HTTPSSession->>SecureSocket: 创建 SecureStreamSocket
    activate SecureSocket
    SecureSocket->>Server: 建立 TCP 连接
    SecureSocket->>Context: 应用 SSL/TLS 配置
    SecureSocket->>OpenSSL: 执行 SSL/TLS 握手
    activate OpenSSL
    OpenSSL-->>Server: SSL/TLS 握手消息
    Server-->>OpenSSL: SSL/TLS 握手消息
    OpenSSL-->>SecureSocket: 握手完成/失败
    deactivate OpenSSL
    alt 握手成功
        SecureSocket-->>HTTPSSession: 连接建立
        deactivate SecureSocket
        App->>HTTPSSession: sendRequest(request)
        HTTPSSession->>SecureSocket: 写入加密的 HTTP 请求
        activate SecureSocket
        SecureSocket->>OpenSSL: 加密数据
        activate OpenSSL
        OpenSSL-->>Server: 发送加密数据
        Server-->>OpenSSL: 返回加密的 HTTP 响应
        OpenSSL-->>SecureSocket: 解密数据
        deactivate OpenSSL
        SecureSocket-->>HTTPSSession: 读取解密的 HTTP 响应
        deactivate SecureSocket
        HTTPSSession-->>App: 返回 HTTPResponse
    else 握手失败 或 证书验证失败
        SecureSocket-->>HTTPSSession: 抛出异常 (e.g., SSLException)
        deactivate SecureSocket
        HTTPSSession-->>App: 异常传递
    end
    deactivate HTTPSSession
```

**流程说明:**

1.  **初始化:** 调用 `SSLManager::instance().initializeClient()` 来初始化 OpenSSL 库（通常只需调用一次）。
2.  **创建和配置 Context:** 创建一个 `Context` 对象，指定用途为客户端 (`CLIENT_USE`)，并配置必要的参数，如信任的 CA 证书 (`loadCertificateAuthorities`)、客户端证书和私钥（如果需要双向认证）、验证模式 (`setVerificationMode`) 等。
3.  **创建 HTTPSClientSession:** 传入服务器主机名、端口号以及配置好的 `Context` 对象。
4.  **建立连接与握手:** `HTTPSClientSession` 内部会创建一个 `SecureStreamSocket`。`SecureStreamSocket` 首先建立底层的 TCP 连接，然后利用 `Context` 的配置，调用 OpenSSL 库执行 SSL/TLS 握手过程。此过程包括协议协商、密码套件选择、证书交换和验证、密钥交换等。
5.  **发送请求:** 握手成功后，应用程序可以通过 `HTTPSClientSession` 发送 `HTTPRequest`。
6.  **安全通信:** `HTTPSClientSession` 通过 `SecureStreamSocket` 发送和接收数据。`SecureStreamSocket` 在内部使用 OpenSSL 对数据进行加密和解密。
7.  **接收响应:** `HTTPSClientSession` 返回解密后的 `HTTPResponse`。

`NetSSL_OpenSSL` 模块是构建需要安全网络通信（如 HTTPS, SMTPS 等）的 POCO 应用程序的关键组件。 
# HTTP 模块 (`src/http`)

## 概览

`http` 模块提供了 HTTP/1.1 客户端和服务器实现，并支持 WebSocket。它基于 `net` 模块构建，利用了其异步网络能力。

## 关键组件与概念

*   **头文件位置:** `src/http/include/scy/http/`
*   **依赖:** `scy/base`, `scy/net`

*   **核心类:**
    *   **`Message` (`message.h`):** HTTP 请求和响应的基类，处理 Headers、版本、状态码/方法等。
    *   **`Request` (`request.h`):** 表示 HTTP 请求，继承自 `Message`，包含方法、URI、查询参数等。
    *   **`Response` (`response.h`):** 表示 HTTP 响应，继承自 `Message`，包含状态码、原因短语等。
    *   **`Parser` (`parser.h`):** HTTP 消息解析器，基于 `nginx` 的 C 解析器 (`http_parser.h`)，用于解析传入的字节流。
    *   **`Connection` (`connection.h`):** HTTP 连接的基类，封装了 `net::Socket`，管理 `Request`, `Response`, `Parser`，处理连接生命周期和数据收发。
    *   **`URL` (`url.h`):** URL 解析和表示类。
    *   **`Cookie` (`cookie.h`), `Form` (`form.h`):** 处理 HTTP Cookie 和表单数据。
    *   **`Authenticator` (`authenticator.h`):** 可能用于处理 HTTP 认证。

*   **服务器端 (`server.h`):**
    *   **`Server`:** 监听端口，接受连接，管理 `ServerConnection`。
        *   使用 `net::TCPSocket` (或 `net::SSLSocket`，虽未直接显示但可推测) 进行监听。
        *   通过 `ServerConnectionFactory` 创建连接和响应处理器。
        *   `Connection` 信号通知新连接。
    *   **`ServerConnection`:** 代表一个服务器端连接，继承自 `Connection`。
        *   处理单个客户端的请求-响应周期。
        *   关联一个 `ServerResponder` 处理请求。
    *   **`ServerResponder`:** 处理请求的抽象基类，用户需继承并实现 `onRequest` 等方法。
    *   **`ServerConnectionFactory`:** 创建 `ServerConnection` 和 `ServerResponder` 的工厂。

*   **客户端 (`client.h`):**
    *   **`Client`:** 管理器/单例，负责创建和管理 `ClientConnection`。
    *   **`ClientConnection`:** 代表一个客户端连接，继承自 `Connection`。
        *   根据 URL scheme (http/https/ws/wss) 自动选择 `net::TCPSocket` 或 `net::SSLSocket`。
        *   通过 `send()` 发起请求。
        *   提供多种信号 (`Connect`, `Headers`, `Payload`, `Complete`, `Close`) 反馈连接状态和响应。
        *   可将响应体写入 `std::ostream` (`setReadStream`)。
        *   支持通过替换 `Adapter` 来处理 WebSocket (`ws::ConnectionAdapter`)。

*   **WebSocket (`websocket.h`):**
    *   **`ws::ConnectionAdapter`:** 用于处理 WebSocket 协议握手和帧解析/封装的适配器。
    *   **`ws::Frame`:** 表示 WebSocket 数据帧。
    *   客户端通过 `createConnectionT` 或 `createConnection` 创建 WebSocket 连接时，会自动使用此适配器。
    *   服务器端可能通过 `ServerResponder` 或特定的工厂逻辑来升级连接到 WebSocket。

## 设计理念

*   **基于 `net` 模块:** 完全构建在异步、事件驱动的网络层之上。
*   **信号/槽:** 大量使用信号进行事件通知 (连接、数据、完成、关闭等)。
*   **工厂模式:** `ServerConnectionFactory` 用于解耦和定制化。
*   **适配器模式:** `ws::ConnectionAdapter` 用于在通用 `Connection` 框架下处理 WebSocket 协议。
*   **封装 HTTP 细节:** `Request`, `Response`, `Parser` 等类封装了协议细节。

## Mermaid 图示例 (服务器组件关系)

```mermaid
componentDiagram
    UserApp --> Server : Uses
    Server --> ServerConnectionFactory : Creates
    Server o-- net.TCPSocket : Listens On
    Server --> ServerConnection : Manages
    ServerConnectionFactory --> ServerConnection : Creates
    ServerConnectionFactory --> ServerResponder : Creates

    ServerConnection o-- net.TCPSocket : Uses
    ServerConnection o-- Parser : Uses
    ServerConnection o-- Request : Holds
    ServerConnection o-- Response : Holds
    ServerConnection --> ServerResponder : Uses

    ServerResponder --> ServerConnection : Accesses
``` 
# 网络模块 (`src/net`)

## 概览

`net` 模块构成了 LibSourcey 的核心网络层，提供了基于 `libuv` 的异步、事件驱动的套接字编程能力。

## 关键组件与概念

*   **头文件位置:** `src/net/include/scy/net/`
*   **依赖:** `scy/base`, `libuv`, `OpenSSL` (用于 `SSLSocket`)

*   **`Address` (`address.h`):**
    *   表示 IPv4/IPv6 地址和端口号。
    *   可从字符串 (IP, 主机名)、`sockaddr` 等构造。
    *   提供 `host()`, `port()`, `toString()`, `family()`, `addr()` 等方法。
    *   使用 Pimpl 模式 (`AddressBase`)。

*   **`Socket` (`socket.h`):**
    *   所有套接字类型的抽象基类 (接口)。
    *   继承自 `SocketAdapter` (可能是 libuv 的集成层)。
    *   定义纯虚方法: `connect()`, `bind()`, `listen()`, `shutdown()`, `close()`, `address()`, `peerAddress()`, `transport()`, `error()` 等。
    *   需要关联一个 `uv::Loop`。
    *   依赖异步事件 (信号/回调可能通过 `SocketAdapter`/`Stream` 实现)。

*   **`TCPSocket` (`tcpsocket.h`):**
    *   具体的 TCP 套接字实现。
    *   继承自 `Socket` 和 `Stream<uv_tcp_t>` (libuv TCP 流封装)。
    *   异步实现核心 TCP 操作。
    *   提供 TCP 选项: `setNoDelay()`, `setKeepAlive()`, `setReusePort()`。
    *   通过 `AcceptConnection` 信号和虚 `onXxx` 回调 (`onConnect`, `onRead`, `onError`, `onClose`) 处理事件。

*   **`UDPSocket` (`udpsocket.h`):**
    *   (根据文件名推测，未读取) 可能提供 UDP 套接字实现。

*   **`SSLSocket` (`sslsocket.h`):**
    *   具体的 SSL/TLS 套接字实现。
    *   继承自 `TCPSocket`，添加了加密层。
    *   需要 `SSLContext` (来自 `sslcontext.h`) 进行配置。
    *   通过 `SSLSession` (来自 `sslsession.h`) 支持会话复用。
    *   使用 `SSLAdapter` (来自 `ssladapter.h`) 进行 OpenSSL 操作。
    *   重写 `send`, `onRead`, `shutdown` 等以处理 SSL 协议。
    *   提供 `peerCertificate()`。

*   **SSL 管理 (`sslcontext.h`, `sslmanager.h`, `sslsession.h`):**
    *   (根据文件名推测，未读取) 提供全面的 SSL/TLS 上下文设置、证书处理和会话管理功能。

*   **数据处理:**
    *   `SocketPacket` (`socket.h`): 默认的数据包类型，携带数据缓冲区 (`MutableBuffer`) 和 `PacketInfo` (源套接字, 对端地址)。
    *   可能与 `socket.h` 包含的 `PacketStream` (`scy/packetstream.h`) 集成。

*   **工具类 (`util.h`, `dns.h`, `types.h`, `transaction.h`, `packetsocket.h`):**
    *   可能包含辅助函数、DNS 解析 (`dns.h`)、类型定义，以及可能用于基于数据包或事务性通信的抽象。

## 设计理念

*   **异步 & 事件驱动:** 利用 `libuv` 实现非阻塞 I/O。
*   **面向对象:** 使用抽象基类和继承来组织不同的套接字类型。
*   **模块化:** 具体的套接字类型 (`TCP`, `UDP`, `SSL`) 继承自公共基类。
*   **平台抽象:** 隐藏了平台特定的套接字细节 (`net.h`)。

## Mermaid 图表示例 (类继承关系)

```mermaid
classDiagram
    SocketAdapter <|-- Socket
    Socket <|-- TCPSocket
    Stream_uv_tcp_t_ <|-- TCPSocket
    TCPSocket <|-- SSLSocket
    Socket <|-- UDPSocket // 推测

    class Socket{
        <<Abstract>>
        +connect()
        +bind()
        +listen()
        +close()
        +address()
        +peerAddress()
        +transport()
        +loop()
    }
    class TCPSocket{
        +send()
        +acceptConnection()
        +setNoDelay()
        +setKeepAlive()
        +setReusePort()
        +AcceptConnection Signal
        #onConnect()
        #onRead()
        #onError()
        #onClose()
    }
    class SSLSocket{
        +useContext()
        +useSession()
        +peerCertificate()
        +send() // override
        #onRead() // override
        #onConnect() // override
    }
    class SocketAdapter{
        // libuv 适配层?
    }
    class Stream_uv_tcp_t_{
        // libuv 流封装?
    }
``` 
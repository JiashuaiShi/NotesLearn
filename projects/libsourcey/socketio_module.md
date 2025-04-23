# Socket.IO 客户端模块 (`src/socketio`)

## 概览

`socketio` 模块提供了 Socket.IO 协议的 C++ 客户端实现。它允许 LibSourcey 应用程序与 Socket.IO 服务器进行实时、双向、基于事件的通信。

## 关键组件与概念

*   **头文件位置:** `src/socketio/include/scy/socketio/`
*   **依赖:** `scy/base`, `scy/net`, `scy/http` (特别是 WebSocket), `scy/json`

*   **核心类:**
    *   **`Client` (`client.h`):** Socket.IO 客户端主类。
        *   继承自 `Stateful<ClientState>` (管理 Closed, Connecting, Connected, Online, Error 状态)。
        *   继承自 `net::SocketAdapter` 和 `PacketSignal` (处理底层网络事件和发出 Packet)。
        *   **连接:** `connect()`, `close()`, `Options` (配置 host, port, 重连逻辑)。
        *   **通信:**
            *   `send()`: 发送字符串/JSON 消息或具名事件。
            *   `send(Packet&)`: 发送原始 Socket.IO 包。
            *   支持 ACK (`ack = true` 参数)。
        *   **事务:** `createTransaction()`: 发送需要 ACK 回复的消息。
        *   **底层传输:** 使用 `http::ws::WebSocket` (`_ws`)。
        *   **协议处理:** 处理 Socket.IO 握手 (`onHandshake`)、Ping/Pong 心跳、包解析 (`onPacket`, `onMessage`)。
        *   **状态/错误:** `sessionID()`, `error()`, `isOnline()` 等。
    *   **`Packet` (`packet.h`):** 表示 Socket.IO 协议的数据包，封装了类型 (CONNECT, DISCONNECT, EVENT, ACK, ERROR, BINARY_EVENT, BINARY_ACK)、路径/命名空间、ID (用于 ACK)、数据 (通常是 JSON) 等。
    *   **`Transaction` (`transaction.h`):** 用于处理需要服务器 ACK 回复的请求-响应模式。

*   **具体实现:**
    *   `TCPClient`, `SSLClient`: 分别使用 `net::TCPSocket` 和 `net::SSLSocket` 作为 WebSocket 的底层传输。
    *   `createTCPClient()`, `createSSLClient()`: 工厂函数。

## 设计理念

*   **封装 Socket.IO 协议:** 将 Socket.IO 的连接、握手、包格式、心跳、ACK 等细节封装起来。
*   **基于 WebSocket:** 使用 LibSourcey 的 `http` 模块中的 WebSocket 实现作为主要传输方式。
*   **事件驱动:** 通过回调函数 (`onConnect`, `onOnline`, `onClose`, `onPacket`, `onMessage` 等) 和 `Stateful` 状态机处理异步事件。
*   **易用性:** 提供简洁的 `connect`, `close`, `send` 接口。

## 与其他模块关系

*   **`net` & `http`:** 依赖这两个模块提供底层的 TCP/SSL Socket 和 WebSocket 传输。
*   **`json`:** Socket.IO 消息体通常使用 JSON 格式。
*   **`symple`:** `symple` 模块的客户端直接继承自 `socketio::Client`，在其基础上构建更高级别的协议。 
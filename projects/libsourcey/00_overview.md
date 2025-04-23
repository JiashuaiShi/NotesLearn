# LibSourcey 项目概览

## 目标

LibSourcey 是一个跨平台的 C++14 模块集合，旨在快速开发高性能的基于网络的点对点 (P2P) 和媒体流应用程序。它的目标是结合 `libuv` 的高性能以及类似于 `FFmpeg`、`OpenCV` 和 `WebRTC` 的功能特性，并采用现代 C++ 标准。

## 核心特性与已识别模块

*   **基础 (Foundation):** 基于 `libuv` 构建，提供事件驱动的异步 I/O。跨平台 (C++14)。模块化设计。使用 `CMake` 构建。
*   **网络 (`src/net`):** 核心网络层，使用 `libuv` 和 `OpenSSL`。提供 TCP、SSL、UDP 套接字。
*   **加密 (`src/crypto`):** 可能通过 OpenSSL 集成处理 SSL/TLS 相关方面。
*   **HTTP (`src/http`):** HTTP 协议栈，包括服务器、客户端、WebSocket。使用了 `nginx` 的高性能 C 解析器。
*   **音视频 (AV) (`src/av`):** 对 `FFmpeg` (编码/解码、流式传输、录制) 和 `OpenCV` (媒体分析) 的封装。支持网络摄像头流式传输和编码 (如 H.264/AAC) 等功能。
*   **实时消息:**
    *   **Socket.IO (`src/socketio`):** 支持 Socket.IO 协议 (v4+) 的 C++ 客户端。
    *   **Symple (`src/symple`):** 基于 Socket.IO 构建的自定义协议，为在线游戏和聊天应用增加了名册 (Rostering)、状态 (Presence) 等功能。
*   **WebRTC (`src/webrtc`):** 原生 WebRTC 支持，用于构建 P2P 桌面和服务器端应用程序，集成了实时消息和媒体功能。
    *   **STUN (`src/stun`):** 实现可能基于 RFC 5389。
    *   **TURN (`src/turn`):** 实现可能基于 RFC 5766 和 RFC 6062。
*   **工具类 (`src/util`, `src/base`, `src/sched`):** 可能包含基类、工具函数、调度机制、进程管理辅助类等。
*   **JSON (`src/json`):** 可能提供 JSON 解析/序列化功能。
*   **构建/文档:** 使用 `CMake` 进行构建，使用 `Doxygen` 生成文档。`vendor` 目录可能包含第三方依赖。

## 关键技术栈

*   C++14
*   `libuv`
*   `OpenSSL`
*   `FFmpeg`
*   `OpenCV` (可选?)
*   `nginx` HTTP 解析器
*   `CMake`

## 注意事项

*   头文件似乎与实现文件一起位于 `src` 的子目录中，项目根目录没有顶层的 `include` 目录。
*   项目强调性能、模块化和简洁的 C++ 代码。
*   提供示例代码和 Docker 镜像。 
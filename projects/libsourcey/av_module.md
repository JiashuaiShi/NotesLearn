# 音视频 (AV) 模块 (`src/av`)

## 概览

`av` 模块提供了对音视频处理库 (主要是 FFmpeg，可能也涉及 OpenCV) 的封装，用于实现媒体捕获、编码、解码、格式转换、流式处理等功能。

## 关键组件与概念

*   **头文件位置:** `src/av/include/scy/av/`
*   **依赖:** `scy/base`, `scy/packetstream`, `FFmpeg` (核心), `OpenCV` (可能用于特定分析功能)

*   **核心抽象:**
    *   **`ICapture` (`icapture.h`):** 采集器接口 (抽象基类)。
        *   继承自 `PacketSource` 和 `basic::Startable`。
        *   定义 `start()`, `stop()`, `getEncoderFormat()`。
        *   作为 `PacketStream` 的数据源，通过 `emitter` 发出 `Packet`。
        *   具体实现: `VideoCapture`, `AudioCapture` (`videocapture.h`, `audiocapture.h`), `MediaCapture` (`mediacapture.h`)。
    *   **`IEncoder` (`iencoder.h`):** 编码器接口 (抽象基类)。
        *   继承自 `Stateful<EncoderState>`。
        *   定义 `init()`, `uninit()`, `options()`。
        *   管理 `EncoderOptions` (输入/输出格式、文件、时长)。
        *   具体实现: `VideoEncoder`, `AudioEncoder`, `MultiplexEncoder` (混合音视频), `MultiplexPacketEncoder` (处理 Packet 流的混合编码器)。
    *   **(推测) `IDecoder`:** 可能存在解码器接口 (如 `videodecoder.h`, `audiodecoder.h`)。

*   **数据流处理 (`PacketStream`):**
    *   `av` 模块深度集成 `scy/packetstream.h` (可能属于 `base` 或 `util` 模块)。
    *   **工作流程:** `ICapture` (Source) -> [可选的处理节点] -> `IEncoder` (Processor) -> [可选的输出节点, 如 `net::Socket` (Sink)]。
    *   `Packet` (`packet.h`): 在 `PacketStream` 中流动的数据单元，封装了音视频帧或其他数据。
    *   `RealtimePacketQueue` (`realtimepacketqueue.h`): 可能用于处理实时流中的数据包缓冲和同步。

*   **FFmpeg 封装 (`ffmpeg.h`):**
    *   提供对 FFmpeg 底层结构 (如 `AVFormatContext`, `AVCodecContext`, `AVFrame`, `AVPacket`) 的 C++ 封装或辅助函数。
    *   `Format` (`format.h`), `Codec` (`codec.h`), `FormatRegistry` (`formatregistry.h`): 用于描述和管理媒体格式与编解码器信息，可能基于 FFmpeg 的数据。
    *   `VideoContext`, `AudioContext` (`videocontext.h`, `audiocontext.h`): 封装与特定流相关的上下文信息。

*   **音视频处理工具:**
    *   `VideoConverter` (`videoconverter.h`): 视频格式/像素格式转换 (可能使用 `libswscale`)。
    *   `AudioResampler` (`audioresampler.h`): 音频重采样 (可能使用 `libswresample`)。
    *   `AudioBuffer` (`audiobuffer.h`): 音频数据缓冲区。
    *   `FPSCounter` (`fpscounter.h`): 帧率计算工具。

*   **设备管理 (`devicemanager.h`):**
    *   `DeviceManager`: 用于枚举系统上的音视频捕获设备。
    *   `Device`: 表示一个具体的设备。

*   **平台特定:** 包含 `win32/` 和 `apple/` 子目录，处理特定平台的捕获或其他细节。

## 设计理念

*   **封装 FFmpeg:** 将复杂的 FFmpeg API 简化为面向对象的 C++ 接口。
*   **基于 PacketStream:** 使用统一的 `PacketStream` 模型处理音视频数据流，易于组合不同的处理阶段 (捕获、处理、编码、传输)。
*   **接口与实现分离:** 使用 `ICapture`, `IEncoder` 等接口定义核心功能，允许多种具体实现。
*   **模块化:** 将功能划分为采集、编码、解码、格式处理、设备管理等子组件。

## Mermaid 图示例 (简化的 PacketStream 流程)

```mermaid
graph TD
    subgraph AV Module
        A[ICapture (e.g., VideoCapture)] -- Emits Packet --> B(PacketStream);
        B -- Passes Packet --> C[IEncoder (e.g., MultiplexPacketEncoder)];
    end
    subgraph Other Modules
        D[net.Socket or FileSink etc.];
    end
    C -- Emits Encoded Packet --> D;

    style A fill:#ccf,stroke:#333,stroke-width:2px
    style C fill:#ccf,stroke:#333,stroke-width:2px
    style B fill:#f9f,stroke:#333,stroke-width:2px
``` 
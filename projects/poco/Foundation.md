# POCO Foundation 模块详解

`Foundation` 模块是 POCO C++ Libraries 的基石，提供了构建可移植、健壮的 C++ 应用程序所需的核心功能和抽象。

## 主要功能

1.  **平台抽象:** 封装了操作系统特定的功能，如文件系统访问 (`File`, `Path`)、环境变量、进程信息 (`Environment`, `Process`) 等，提供了跨平台的一致接口。
2.  **基本数据类型:** 定义了一些增强的数据类型，如 `Int64`, `UInt64`, `Any`, `DynamicAny`, `UUID` 等。
3.  **字符串与文本处理:** 提供了强大的字符串工具 (`StringTokenizer`, `RegularExpression`) 和文本编码转换 (`TextEncoding`, `TextConverter`)。
4.  **流处理:** 扩展了 C++ 标准库的流，增加了内存流、文件流、日志流、压缩流 (`MemoryStream`, `FileInputStream`, `FileOutputStream`, `LoggingStream`, `ZLibStream`) 等。
5.  **日志框架:** 提供了一个灵活且可扩展的日志系统 (`Logger`, `Channel`, `Formatter`)，支持多种输出目标和格式。
6.  **配置管理:** 支持从不同来源（如属性文件、XML 文件、环境变量）加载和管理应用程序配置 (`AbstractConfiguration`, `PropertyFileConfiguration`, `XMLConfiguration`)。
7.  **多线程与同步:** 提供了面向对象的线程模型 (`Thread`, `Runnable`)、线程池 (`ThreadPool`) 以及各种同步原语 (`Mutex`, `ScopedLock`, `Event`, `Semaphore`, `Condition`)。
8.  **内存管理:** 包含智能指针 (`SharedPtr`, `AutoPtr`) 和内存池 (`MemoryPool`) 等工具。
9.  **通知框架:** 一个通用的发布/订阅模式实现 (`Notification`, `NotificationCenter`, `NotificationQueue`)，用于组件间的解耦通信。
10. **事件处理:** 提供了类似 C# 的事件机制 (`Poco::BasicEvent`, `Poco::delegate`)。
11. **异常处理:** 定义了一套标准的异常类层次结构，方便错误处理。
12. **日期与时间:** 提供了 `Timestamp`, `Timespan`, `DateTime`, `DateTimeFormatter`, `DateTimeParser` 等类来处理日期和时间。
13. **URI 处理:** `URI` 类用于解析和操作统一资源标识符。

## 设计特点

*   **RAII:** 广泛使用 RAII 模式管理资源，特别是锁 (`ScopedLock`, `ScopedUnlock`) 和指针 (`AutoPtr`, `SharedPtr`)。
*   **异常安全:** 设计时考虑异常安全，确保在异常发生时资源能正确释放，程序状态保持一致。
*   **值语义与引用语义:** 根据需要灵活运用值语义（如 `Timestamp`）和引用语义（通过智能指针管理的对象）。

## 关键组件关系示例 (多线程)

以下 Mermaid 图展示了 Foundation 模块中基本的多线程组件及其关系：

```mermaid
graph TD
    subgraph 多线程核心
        A[Thread] -- "拥有" --> B(Runnable);
        A -- "执行 run()" --> B;
        C{Mutex} -- "保护" --> D(共享资源);
        E(ScopedLock<Mutex>) -- "管理" --> C;
        F[Condition] -- "配合" --> C;
        G[ThreadPool] -- "管理/复用" --> A;
    end

    ThreadRunnable[用户定义Runnable实现] --|> B;
    ThreadClass[用户定义Thread子类] --|> A;
```

*   `Thread` 类代表一个执行线程。
*   `Runnable` 是一个接口，定义了线程要执行的任务 (`run()` 方法)。`Thread` 对象可以持有一个 `Runnable` 对象并在新线程中执行其 `run()` 方法。
*   `Mutex` (互斥量) 用于保护共享资源，防止并发访问冲突。
*   `ScopedLock` 是一个 RAII 包装器，用于自动获取和释放 `Mutex`。
*   `Condition` (条件变量) 用于线程间的等待和通知，通常与 `Mutex` 配合使用。
*   `ThreadPool` 管理一个线程池，用于复用线程，减少创建和销毁线程的开销。

Foundation 模块是 POCO 的核心，理解它对于使用 POCO 的其他模块至关重要。 
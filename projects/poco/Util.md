# POCO Util 模块详解

`Util` 模块提供了一系列独立或补充性的实用工具和框架，这些功能不适合放在 Foundation 或其他专门模块中。

## 主要功能

1.  **应用程序框架 (`Application`, `ServerApplication`, `Subsystem`):**
    *   提供了一个标准化的应用程序入口点和生命周期管理框架。
    *   `Application` 是基础类，`ServerApplication` 针对后台服务或守护进程做了扩展（如处理命令行参数、配置文件、日志初始化、作为 Windows 服务或 Unix 守护进程运行）。
    *   支持子系统 (`Subsystem`) 概念，允许将应用程序功能模块化，由框架管理其初始化和清理。
2.  **命令行选项处理 (`Option`, `OptionSet`, `OptionProcessor`):**
    *   提供了一套强大的机制来定义和解析命令行参数和选项。
    *   支持短选项、长选项、带参数的选项、回调处理等。
3.  **定时器 (`Timer`, `TimerCallback`):**
    *   提供了高精度的定时器功能，可以在指定延迟后或按固定间隔执行任务。
    *   任务通过 `TimerCallback` 函数对象或成员函数指针指定。
4.  **缓存框架 (`AbstractCache`, `LRUCache`, `TimedLRUCache`):**
    *   提供了一个通用的缓存策略框架。
    *   内置了基于最近最少使用 (LRU) 策略的缓存实现，以及带有超时淘汰机制的 LRU 缓存。
5.  **全局服务注册表 (`Registry`):**
    *   一个简单的单例注册表，用于存储和检索全局可用的对象或服务 (通常不推荐大量使用，以避免过度依赖全局状态)。
6.  **可执行代码加载 (`SharedLibrary`):**
    *   提供了跨平台的动态链接库（DLL 或 .so）加载和函数符号查找功能。

## 设计特点

*   **框架化:** 应用程序框架和命令行处理提供了结构化的方法来构建应用程序。
*   **可配置性:** 应用程序框架与 Foundation 的配置管理紧密集成。
*   **通用性:** 缓存、定时器等工具设计为通用组件，可应用于多种场景。

## 关键组件关系示例 (应用程序框架)

以下 Mermaid 图展示了 `ServerApplication` 及其子系统的基本结构：

```mermaid
graph TD
    A[操作系统] -- "启动进程" --> B(main 函数);
    B -- "创建并运行" --> C{MyApp (继承自 ServerApplication)};

    subgraph ServerApplication 框架
        C -- "初始化时注册" --> D[Subsystem 1];
        C -- "初始化时注册" --> E[Subsystem 2];
        C -- "管理生命周期" --> D;
        C -- "管理生命周期" --> E;
        F(Application) -- "基类" --> C;
        C -- "使用" --> G[OptionProcessor];
        C -- "使用" --> H[Logger];
        C -- "使用" --> I[Config];
    end

    subgraph Foundation 模块
        G;
        H;
        I;
    end

    D -- "实现具体功能 A" --> X[...];
    E -- "实现具体功能 B" --> Y[...];
```

**流程说明:**

1.  程序的 `main` 函数创建用户定义的 `ServerApplication` 子类（例如 `MyApp`）的实例，并调用其 `run` 方法。
2.  `ServerApplication` 的构造函数或 `initialize` 方法通常会：
    *   设置命令行选项 (`OptionSet`)。
    *   解析命令行参数 (`OptionProcessor`)。
    *   加载配置文件 (`Config`)。
    *   初始化日志系统 (`Logger`)。
    *   创建并注册应用程序所需的子系统 (`Subsystem`) 实例。
3.  框架负责调用每个已注册子系统的 `initialize` 方法。
4.  框架的核心逻辑运行（通常在 `main` 方法中实现，例如启动一个网络服务器）。
5.  当应用程序需要终止时（例如收到信号或调用 `terminate()`）：
    *   框架负责调用每个子系统的 `uninitialize` 方法（通常按注册的反序）。
    *   执行其他清理工作。

Util 模块通过提供这些高级工具和框架，进一步提升了使用 POCO 开发应用程序的效率和规范性。 
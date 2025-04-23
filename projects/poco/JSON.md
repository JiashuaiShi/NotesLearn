# POCO JSON 模块详解

`JSON` 模块提供了处理 JSON (JavaScript Object Notation) 数据格式的功能，包括解析 JSON 文本、构建和操作 JSON 对象，以及将 JSON 对象序列化为文本。

## 主要功能

1.  **解析 (Parsing):**
    *   `Parser`: 用于将 JSON 文本（来自字符串或流）解析为内存中的数据结构。
    *   `Handler`: 一个接口类，`Parser` 在解析过程中会将 JSON 结构（对象开始/结束、数组开始/结束、键、值等）通过 `Handler` 的方法回调通知给用户。这是一种类似 SAX 的事件驱动模型。
    *   `ParseHandler` (继承自 `Handler`): 一个具体的 Handler 实现，它在解析时直接构建起 `Poco::Dynamic::Var` 对象，这是最常用的解析方式。
2.  **数据表示:**
    *   `Poco::Dynamic::Var`: POCO 使用 `Foundation` 模块中的 `Var` 类来表示解析后的 JSON 数据。`Var` 可以持有各种类型的值（整数、浮点数、字符串、布尔值、空值），并且可以表示 JSON 对象 (`Poco::JSON::Object::Ptr`) 和 JSON 数组 (`Poco::JSON::Array::Ptr`)。
    *   `Poco::JSON::Object`: 代表一个 JSON 对象（键值对集合）。提供了添加、获取、检查、移除成员的方法。
    *   `Poco::JSON::Array`: 代表一个 JSON 数组（值的有序列表）。提供了添加、获取、设置元素的方法。
3.  **序列化 (Stringifying):**
    *   `Stringifier`: 提供了静态方法 `stringify()`，用于将 `Poco::Dynamic::Var` 对象（包括其嵌套的 `Object` 和 `Array`）转换回 JSON 格式的文本字符串或输出到流。
    *   可以控制输出格式，例如是否缩进以提高可读性。
4.  **查询与操作:**
    *   由于 JSON 数据被表示为 `Var`、`Object` 和 `Array`，可以直接使用这些类提供的方法来查询和修改 JSON 结构。
    *   `Object` 和 `Array` 支持链式调用，方便构建复杂的 JSON 结构。
    *   `Var` 强大的类型转换能力使得从 JSON 中提取数据非常方便。

## 设计特点

*   **基于 `Dynamic::Var`:** 核心是利用 `Var` 的动态类型能力来灵活地表示 JSON 数据结构。
*   **流式接口:** 解析和序列化都支持 C++ 流。
*   **易用性:** API 设计简洁，特别是使用 `ParseHandler` 和 `Stringifier` 时，解析和序列化过程非常直接。

## 关键组件关系示例 (解析与序列化)

以下 Mermaid 图展示了使用 POCO JSON 模块解析和序列化 JSON 数据的典型流程：

```mermaid
graph TD
    subgraph 输入
        A[JSON 文本 (字符串/流)]
    end

    subgraph POCO JSON
        B{Parser} -- "解析" --> A;
        C[ParseHandler] -- "作为回调" --> B;
        B -- "触发事件给" --> C;
        C -- "构建" --> D(Dynamic::Var 对象);
        D -- "包含" --> E(JSON::Object::Ptr);
        D -- "包含" --> F(JSON::Array::Ptr);
        E -- "包含" --> D;  // Object can contain other Vars
        F -- "包含" --> D;  // Array can contain other Vars

        G[应用程序代码];
        G -- "使用" --> B;
        G -- "使用" --> C;
        G -- "获取/操作" --> D;

        H{Stringifier} -- "序列化" --> D;
        H -- "输出" --> I[JSON 文本 (字符串/流)];
        G -- "使用" --> H;
    end

    subgraph 输出
        I;
    end

    G -- "提供输入" --> A;
    I -- "获取输出" --> G;
```

**流程说明:**

1.  **解析:**
    *   创建一个 `ParseHandler` 实例 (通常 `ParseHandler::Ptr` 智能指针)。
    *   创建一个 `Parser` 实例，并将 `ParseHandler` 传递给它。
    *   调用 `Parser::parse()` 方法，传入 JSON 源文本（字符串或流）。
    *   `Parser` 读取文本，根据 JSON 语法规则调用 `ParseHandler` 的回调方法。
    *   `ParseHandler` 在内部构建一个 `Dynamic::Var` 对象来表示整个 JSON 结构。
    *   调用 `ParseHandler::result()` 获取最终构建的 `Var` 对象。
2.  **操作:**
    *   应用程序代码现在可以检查 `Var` 的类型，并根据需要将其转换为 `Object::Ptr` 或 `Array::Ptr`。
    *   使用 `Object` 和 `Array` 提供的方法（如 `getObject`, `getArray`, `getValue<T>`, `set`, `add` 等）来访问和修改 JSON 数据。
3.  **序列化:**
    *   调用 `Stringifier::stringify()` 静态方法，传入要序列化的 `Var` 对象以及可选的格式化参数（如缩进空格数）。
    *   该方法返回 JSON 格式的字符串，或者可以将结果直接写入输出流。

JSON 模块使得在 C++ 中处理 JSON 数据变得简单而高效。 
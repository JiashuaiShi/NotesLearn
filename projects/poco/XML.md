# POCO XML 模块详解

`XML` 模块提供了在 C++ 中处理 XML (可扩展标记语言) 文档的功能，包括解析、操作和查询。

## 主要功能

1.  **DOM (文档对象模型) 解析与操作:**
    *   `DOMParser`: 用于将 XML 文档解析成内存中的树状结构 (DOM树)。
    *   `Document`, `Element`, `Attr`, `Text`, `Comment` 等节点类: 代表 DOM 树中的不同部分，提供了遍历、访问和修改节点内容、属性、子节点的方法。
    *   `DOMWriter`: 用于将内存中的 DOM 树序列化回 XML 文本格式。
    *   支持命名空间 (`NamespaceStrategy`)。
2.  **SAX (Simple API for XML) 解析:**
    *   `SAXParser`: 提供基于事件的 XML 解析方式。解析器在读取 XML 文档时，遇到不同的元素（如开始标签、结束标签、文本内容等）会触发相应的事件。
    *   `ContentHandler`, `DTDHandler`, `ErrorHandler` 等接口: 用户需要实现这些接口中的方法来处理解析过程中触发的事件。SAX 解析通常比 DOM 解析更快且内存占用更少，特别适合处理大型 XML 文件，但处理逻辑相对复杂。
3.  **XPath 支持:**
    *   允许使用 XPath 表达式来查询 DOM 树中的节点。
    *   `Poco::XML::Node` 类提供了 `selectSingleNode()` 和 `selectNodes()` 方法来执行 XPath 查询。

## 设计特点

*   **标准兼容:** 同时支持 DOM Level 2/3 和 SAX2 标准接口。
*   **灵活性:** 提供了 DOM 和 SAX 两种不同的解析模型，适用于不同的应用场景。
*   **易用性:** DOM API 提供了直观的树状结构操作，XPath 支持简化了节点查询。
*   **集成性:** 与 Foundation 模块的流 (`InputSource`, `XMLWriter` 可以写入流) 等功能集成。

## 关键组件关系示例 (DOM 解析)

以下 Mermaid 图展示了使用 DOM 解析器处理 XML 文档的基本流程：

```mermaid
graph TD
    subgraph 输入
        A[XML 文件或字符串]
        B(InputSource) -- "包装" --> A;
    end

    subgraph POCO XML (DOM)
        C{DOMParser} -- "解析" --> B;
        C -- "生成" --> D[Document (DOM 树根节点)];
        D -- "包含" --> E(Element);
        D -- "包含" --> F(Text);
        D -- "包含" --> G(Comment);
        E -- "包含属性" --> H(Attr);
        E -- "包含子节点" --> E;
        E -- "包含子节点" --> F;
        E -- "包含子节点" --> G;
        I[应用程序代码] -- "使用" --> C;
        I -- "遍历/操作" --> D;
        J(DOMWriter) -- "序列化" --> D;
        J -- "输出到" --> K[XML 输出流或字符串];
        I -- "使用" --> J;
    end

    I -- "提供输入" --> B;
    K -- "获取输出" --> I;
```

**流程说明:**

1.  **准备输入:** 将 XML 数据源（文件、内存字符串等）包装成 `InputSource` 对象。
2.  **创建解析器:** 创建一个 `DOMParser` 实例。
3.  **解析文档:** 调用 `DOMParser::parse()` 方法，传入 `InputSource`。解析器会读取 XML 数据，并在内存中构建一个 DOM 树。
4.  **获取文档对象:** `parse()` 方法返回一个 `Document` 对象的智能指针 (`AutoPtr<Document>`)，这是 DOM 树的根节点。
5.  **操作 DOM:** 应用程序代码可以通过 `Document` 对象访问和操作整个 DOM 树，包括获取根元素、查找特定元素、修改节点内容、添加/删除节点或属性等。
6.  **(可选) XPath 查询:** 使用 `selectSingleNode()` 或 `selectNodes()` 方法配合 XPath 表达式快速定位节点。
7.  **(可选) 序列化:** 创建 `DOMWriter` 实例，调用其 `writeNode()` 方法将 `Document` (或其任何子节点) 序列化为 XML 文本，输出到指定的流或字符串。

XML 模块为需要处理 XML 数据的 C++ 应用程序提供了强大而灵活的工具集。 
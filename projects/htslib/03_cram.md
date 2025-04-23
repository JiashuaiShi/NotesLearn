# HTSlib CRAM 处理

## 1. CRAM 格式简介

CRAM 是一种用于存储测序比对记录的文件格式，其设计目标是相比 BAM 实现更高的压缩率。它通过以下方式实现：

*   **基于参考的压缩**: CRAM 主要存储 read 相对于参考序列的**差异**信息，而不是存储完整的 read 序列。对于匹配参考序列的部分，只需记录其匹配状态；对于不匹配的部分（如 SNP、Indel），则存储具体的碱基或变异信息。
*   **多序列压缩**: 它将不同比对记录的相同类型数据（如 read 名称、碱基质量、比对标志等）组织在一起，然后对这些同类数据流应用专门的压缩算法（如 rANS、Gzip、Bzip2 等）。
*   **分解到容器 (Container) 和块 (Slice)**: CRAM 文件由一系列容器组成，每个容器包含一个或多个块 (Slice)。块是 CRAM 的基本解码单元，通常包含一定基因组范围或一定数量的比对记录。每个块内部又包含核心数据块 (Core Data Block) 和多个外部数据块 (External Data Blocks)，分别存储固定字段和可变字段数据。

**优点:**

*   显著减小文件体积，尤其是在高覆盖度测序数据中。
*   仍支持基于坐标的随机访问（需要 `.crai` 索引文件）。

**缺点:**

*   压缩和解压缩通常比 BAM 需要更多的计算资源。
*   解码（读取）时通常需要访问参考基因组序列 (FASTA 文件)。

## 2. HTSlib 中的 CRAM 处理

HTSlib 提供了完整的 CRAM 文件读写支持，包含在 `htslib/cram.h` 头文件中。

*   **API 统一性**: HTSlib 努力保持 CRAM API 与 SAM/BAM API 的一致性。用户通常可以使用相同的 `hts_open()`, `sam_hdr_read()`, `sam_read1()`, `sam_hdr_write()`, `sam_write1()`, `hts_close()` 等函数来处理 CRAM 文件，就像处理 SAM/BAM 一样。
*   **自动格式检测**: `hts_open()` 会自动检测文件是否为 CRAM 格式。
*   **参考序列依赖**: 当读取 CRAM 文件时，HTSlib 通常需要访问参考基因组 FASTA 文件。可以通过以下方式提供：
    *   设置环境变量 `REF_PATH`：指向包含参考序列 FASTA 文件（及其 `.fai` 索引）的路径模式。
    *   设置环境变量 `REF_CACHE`：用于缓存下载或查找的参考序列片段。
    *   在 `hts_open()` 或相关函数调用中通过选项 (`hts_opt`) 明确指定参考文件路径。
    *   如果 CRAM 文件内嵌了所需的参考序列片段（MD5 匹配），则可能不需要外部参考文件。
*   **核心结构**: CRAM 的读写同样使用 `sam_hdr_t` (头部) 和 `bam1_t` (单条记录)。HTSlib 在后台处理 CRAM 格式到 `bam1_t` 结构的转换。
*   **索引**: CRAM 使用 `.crai` 文件作为索引，支持基于基因组区域的快速检索。`sam_index_load()` 可以加载 `.crai` 索引，`hts_iterator_t` 可以用于区域查询。

## 3. CRAM 压缩原理示意 (Mermaid)

```mermaid
graph TD
    subgraph CRAM 文件结构
        A[CRAM 文件] --> B(容器 1)
        A --> C(容器 2)
        A --> D(...)
        B --> E{块 1 (Slice)}
        B --> F{块 2}
        E --> G[块头]
        E --> H(核心数据块)
        E --> I(外部块: 名称)
        E --> J(外部块: 序列)
        E --> K(外部块: 质量)
        E --> L(外部块: ...)
    end

    subgraph 压缩流程 (简化)
        M[多条 BAM 记录 (bam1_t)] --> N{按数据类型分组}
        N --> O[Read 名称流]
        N --> P[序列差异流 (相对于参考)]
        N --> Q[质量分数流]
        N --> R[比对标志流]
        N --> S[其他字段流...]
        O --> T{应用特定压缩算法 (如 rANS, Gzip)}
        P --> T
        Q --> T
        R --> T
        S --> T
        T --> U[打包成块和容器]
        U --> A
    end

    subgraph 解压流程 (简化)
        A --> V[读取容器和块]
        V --> W{根据块头信息}
        W --> X[解压各数据流]
        X -- 结合参考序列 (FASTA) --> Y[重建 BAM 记录 (bam1_t)]
    end
```

HTSlib 通过统一的 API 隐藏了 CRAM 格式的复杂性，使得开发者可以用类似处理 BAM 的方式来处理 CRAM 数据。 
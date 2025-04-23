# HTSlib 索引机制

## 1. 索引的目的

对于大型的、按基因组坐标排序的 HTS 文件（如 BAM, CRAM, BCF, VCF.gz），如果想快速访问特定基因组区域（例如，`chr1:1,000,000-2,000,000`）的数据，线性扫描整个文件会非常低效。索引文件的作用就是存储一种数据结构，能够快速定位到包含目标区域记录的文件块（通常是 BGZF 块），从而大大加速区域查询。

HTSlib 支持多种索引格式，主要用于不同的文件类型和需求。

## 2. 主要索引格式

*   **BAI (BAM Index)**: `.bai` 文件
    *   专为 BAM 文件设计。
    *   基于 R-tree 的变种 (Binning Index Scheme) 和线性索引 (Linear Index)。
    *   **Binning 索引**: 将基因组划分为一系列预定义的、大小不一的区间（bins），并记录每个 bin 内的比对记录所跨越的文件块（BGZF 块）的虚拟文件偏移量范围。
    *   **线性索引**: 为每个参考序列每隔一定距离（如 16384 bp）记录一个文件偏移量，用于快速定位到大致区域，并辅助 binning 索引。
    *   缺点：坐标范围限制为 2^29 (约 512 Mbp)，对于非常长的染色体可能不够用。
*   **CSI (Coordinate-Sorted Index)**: `.csi` 文件
    *   更通用的索引格式，可用于 BAM, BCF, VCF.gz, BED.gz 等多种按坐标排序的文件。
    *   解决了 BAI 的坐标范围限制问题。
    *   结构与 BAI 类似，也使用 Binning 索引和线性索引，但参数（如最小 bin 大小、线性索引间距）可配置。
    *   通常是新版工具（如 Samtools 1.x 后续版本）为 BAM 文件生成的默认索引格式。
*   **Tabix (Tab-delimited file Index)**: `.tbi` 文件
    *   专为 BGZF 压缩的、按坐标排序的**文本**文件设计，如 VCF.gz, GFF/GTF.gz, BED.gz 等。
    *   结构与 BAI/CSI 类似，也使用 Binning 和线性索引方案。
    *   需要用户在建立索引时指定染色体、起始位置、结束位置所在的列。
*   **CRAI (CRAM Index)**: `.crai` 文件
    *   专为 CRAM 文件设计。
    *   存储每个 CRAM 块 (Slice) 的信息，包括其所属参考序列 ID、起始/结束坐标、在 CRAM 文件中的字节偏移量、在容器中的字节偏移量以及块的大小等。
    *   结构相对简单，本质上是一个查找表，将基因组区域映射到包含该区域数据的 CRAM 块。

## 3. HTSlib 中的索引 API (`hts.h`, `sam.h`, `vcf.h`)

HTSlib 提供了统一的 API 来处理这些索引：

*   **加载索引**: `sam_index_load()` (用于 BAI, CSI), `tbx_index_load()` (用于 TBI), `cram_index_load()` (用于 CRAI)。这些函数通常需要主数据文件 (`htsFile`) 和索引文件名作为参数。HTSlib 也提供了 `hts_idx_load()` 作为通用入口，它可以根据文件名自动推断索引类型并加载。
*   **核心结构**: `hts_idx_t` (`hts.h`) 是表示已加载索引的通用结构体。对于 Tabix，还有专门的 `tbx_t` 结构。
*   **构建索引**: `sam_index_build()` (BAI/CSI), `tbx_index_build()` (TBI)。通常需要提供排序后的主数据文件名和输出索引文件名。
*   **区域查询 (迭代器)**:
    *   这是索引最常用的功能。通过 `hts_iterator_t` 实现。
    *   `sam_itr_queryi()` / `sam_itr_querys()`: 为 BAM/CRAM 创建区域查询迭代器。
    *   `tbx_itr_queryi()` / `tbx_itr_querys()`: 为 Tabix 索引文件创建区域查询迭代器。
    *   `bcf_itr_queryi()` / `bcf_itr_querys()`: 为 VCF/BCF 创建区域查询迭代器。
    *   这些函数接受已加载的索引 (`hts_idx_t` 或 `tbx_t`)、区域信息（参考序列 ID 或名称，起始/结束位置）作为参数，返回一个迭代器。
    *   然后可以使用 `sam_itr_next()` / `bcf_itr_next()` / `tbx_itr_next()` 结合相应的读取函数（如 `sam_read1`, `bcf_read1`）来遍历目标区域内的所有记录。
    *   使用完毕后，需要用 `hts_itr_destroy()` 销毁迭代器。

## 4. 索引查询流程图 (Mermaid - 以 BAI/CSI/TBI 为例)

```mermaid
graph TD
    A[用户指定查询区域 (chr, start, end)] --> B{加载索引文件 (.bai/.csi/.tbi)}
    B --> C{解析区域字符串/获取 TID}
    C --> D{查找 Binning 索引}
    D --> E[获取覆盖查询区域的 Bins]
    E --> F[获取这些 Bins 关联的文件块 (BGZF Chunks) 列表]
    C --> G{查找线性索引 (可选辅助)}
    G --> H[获取查询区域附近的粗略文件偏移]
    F & H --> I{合并和过滤文件块列表}
    I --> J[得到需要读取的最小 BGZF 块集合的虚拟文件偏移范围]
    J --> K{创建迭代器 (hts_iterator_t)}
    K -- seek 到第一个相关块 --> L[BGZF 文件]
    M[用户调用 itr_next()] --> N{迭代器内部逻辑}
    N -- 读取并解压块 --> L
    N -- 解析记录 --> O[BAM/BCF/VCF 记录]
    N -- 检查记录是否在查询区域内 --> P{在区域内?}
    P -- 是 --> Q[返回记录给用户]
    P -- 否 --> N
    Q --> M
```

索引是 HTSlib 实现对大型 HTS 数据高效随机访问的关键技术。 
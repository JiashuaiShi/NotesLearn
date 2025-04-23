# HTSlib 项目概览

## 1. 项目目标与定位

HTSlib 是一个用于读写高通量测序（High-Throughput Sequencing, HTS）数据的 C 语言库。其核心目标是提供一套统一、高效且健壮的 API，用于处理常见的 HTS 数据格式，包括 SAM (Sequence Alignment/Map)、BAM (Binary Alignment/Map)、CRAM、VCF (Variant Call Format) 和 BCF (Binary Call Format)。

HTSlib 广泛应用于生物信息学领域，是许多知名工具（如 Samtools 和 Bcftools）的基础依赖库。它旨在：

*   **统一接口**：为不同的文件格式提供相似的读写函数接口。
*   **高效性能**：优化 I/O 操作和数据解析，特别是对于大型文件。
*   **格式支持**：支持各种 HTS 核心格式及其索引。
*   **可移植性**：能够在多种操作系统和平台上编译运行。

## 2. 核心功能模块

HTSlib 的功能可以大致分为以下几个核心模块：

*   **I/O 与压缩 (`htsFile`, BGZF)**：提供统一的文件访问接口 (`htsFile`)，并处理底层的文件读写、网络流访问（如 http/ftp）以及 BGZF (Blocked GNU Zip Format) 压缩/解压缩。BGZF 是 BAM 和其他一些格式使用的分块压缩格式，支持快速随机访问。
*   **SAM/BAM 处理**：解析和操作 SAM/BAM 文件，包括文件头 (`sam_hdr_t`) 和比对记录 (`bam1_t`) 的读写、操作和验证。
*   **CRAM 处理**：支持 CRAM 文件的读写。CRAM 是一种比 BAM 更紧凑的比对记录存储格式，它通过基于参考序列的压缩来实现更高的压缩率。
*   **VCF/BCF 处理**：解析和操作 VCF/BCF 文件，包括文件头 (`vcf_hdr_t`) 和变异记录 (`bcf1_t`) 的读写与操作。BCF 是 VCF 的二进制版本，通常更紧凑且解析更快。
*   **索引 (`hFILE`, Tabix, CSI)**：支持为基于坐标排序的文件（如 BAM, BCF, VCF.gz）创建和查询索引，实现快速的区域检索。主要支持 BAI (BAM Index)、CSI (Coordinate-Sorted Index，比 BAI 更通用) 和 Tabix (用于 VCF/BCF 等文本格式)。
*   **线程池**：提供可选的后台线程池，用于加速 BGZF 块的压缩和解压缩，从而提高 I/O 吞吐量。

## 3. 架构图示 (Mermaid)

下面是一个简化的 HTSlib 架构图，展示了核心模块之间的关系：

```mermaid
graph TD
    subgraph 应用层 (e.g., Samtools, Bcftools)
        A[用户代码/工具]
    end

    subgraph HTSlib API
        B(htsFile API) -- 统一文件访问 --> F{文件/网络流}
        C(SAM/BAM API) -- 读写 --> G[SAM/BAM 文件]
        D(CRAM API) -- 读写 --> H[CRAM 文件]
        E(VCF/BCF API) -- 读写 --> I[VCF/BCF 文件]
        J(Indexing API) -- 读写/查询 --> K[索引文件 BAI/CSI/TBI]
    end

    subgraph 底层支持
        L(BGZF 压缩/解压)
        M(线程池)
        N(数据结构 Bam1/Bcf1 等)
    end

    A --> B
    A --> C
    A --> D
    A --> E
    A --> J

    B --> L
    C -- 使用 --> B
    C -- 使用 --> N
    D -- 使用 --> B
    D -- 使用 --> N
    E -- 使用 --> B
    E -- 使用 --> N
    J -- 使用 --> B

    L -- 可选使用 --> M
```

这个概览提供了 HTSlib 的基本认识。后续的文件将深入探讨各个具体模块。 
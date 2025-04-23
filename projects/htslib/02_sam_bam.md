# HTSlib SAM/BAM 处理

## 1. SAM/BAM 格式简介

*   **SAM (Sequence Alignment/Map)**：一种基于文本的格式，用于存储测序 read 相对于参考基因组的比对信息。它包含一个头部（Header）部分（以 `@` 开头，描述参考序列、使用的程序等元数据）和一个比对记录（Alignment）部分（每行一个比对记录）。
*   **BAM (Binary Alignment/Map)**：SAM 格式的二进制、BGZF 压缩版本。它存储与 SAM 相同的信息，但体积更小，且支持通过索引进行快速随机访问。BAM 是目前存储比对信息最常用的格式。

## 2. HTSlib 中的核心结构

HTSlib 使用以下主要结构体来处理 SAM/BAM 数据：

*   **`sam_hdr_t` (`sam.h`)**: 表示 SAM/BAM 文件的头部信息。
    *   存储参考序列字典 (`@SQ` 行)、读取组 (`@RG` 行)、程序信息 (`@PG` 行) 等。
    *   提供 `sam_hdr_init()`, `sam_hdr_read()`, `sam_hdr_write()`, `sam_hdr_parse()`, `sam_hdr_find_tag_id()` 等函数来创建、读写和查询头部信息。
    *   头部信息对于解释比对记录中的参考序列 ID (TID) 和读取组 ID 至关重要。
*   **`bam1_t` (`sam.h`)**: 表示单个比对记录 (Alignment Record)。
    *   这是一个核心结构，包含比对的所有信息，如 QNAME (Read 名称), FLAG (位标志), RNAME (参考序列名，存储为 TID), POS (比对位置), MAPQ (比对质量), CIGAR (比对细节), RNEXT/PNEXT (双端测序 mate 信息), TLEN (模板长度), SEQ (Read 序列), QUAL (碱基质量) 以及可选的 Tag 字段 (Auxiliary Data)。
    *   **动态大小**：`bam1_t` 的大小不是固定的，因为它需要容纳可变长度的数据（如 QNAME, CIGAR, SEQ, QUAL, Tags）。HTSlib 内部会动态管理其内存。
    *   相关 API：`bam_init1()`, `bam_destroy1()`, `sam_read1()`, `sam_write1()`, `bam_get_qname()`, `bam_get_seq()`, `bam_get_aux()` 等。
*   **`bam1_core_t` (`sam.h`)**: `bam1_t` 结构体内部的一个固定大小的子结构，包含比对记录的核心数值信息（FLAG, TID, POS, MAPQ, CIGAR 操作数数量, Mate TID, Mate POS, TLEN 等）。这部分数据在 BAM 文件中是固定长度存储的。

## 3. 主要 API 函数

*   **打开/关闭文件**: 使用 `hts_open()` 打开 SAM/BAM 文件，`hts_close()` 关闭。
*   **读写头部**: 使用 `sam_hdr_read()` 从 `htsFile` 读取头部，`sam_hdr_write()` 写入头部。
*   **读写记录**: 使用 `sam_read1()` 从 `htsFile` 读取下一条比对记录到 `bam1_t` 结构，`sam_write1()` 将 `bam1_t` 结构写入 `htsFile`。
*   **记录操作**: 提供大量宏和函数 (`bam_get_qname`, `bam_get_seq`, `bam_get_cigar`, `bam_aux_get` 等) 来访问和操作 `bam1_t` 中的字段。
*   **迭代器**: HTSlib 也提供了基于索引的迭代器 (`hts_iterator_t`)，用于高效检索特定基因组区域的比对记录 (需要 BAM 索引文件，如 `.bai` 或 `.csi`)。

## 4. BAM 文件结构图示 (Mermaid)

```mermaid
graph BT
    subgraph BAM 文件 (.bam)
        direction LR
        A[BGZF 块 1]
        B[BGZF 块 2]
        C[...]
        D[BGZF 块 N]
    end

    subgraph BGZF 块内容
        direction TB
        E{解压后数据}
        E --> F[可选: SAM 头部文本]
        E --> G[BAM 记录 1]
        E --> H[BAM 记录 2]
        E --> I[...]
    end

    subgraph 单个 BAM 记录 (内存中对应 bam1_t)
        direction TB
        J[固定核心部分 (bam1_core_t)]
        K[可变长度数据]
        J --> L[Block Size]
        J --> M[Ref ID (TID)]
        J --> N[Position]
        J --> O[Bin, MAPQ, FLAG, ...]
        J --> P[CIGAR 操作数数量 (n_cigar_op)]
        J --> Q[Read Name 长度 (l_read_name)]
        J --> R[Mate Ref ID, Mate Pos, TLEN]
        K --> S[Read Name (QNAME)]
        K --> T[CIGAR 操作 (编码)]
        K --> U[Sequence (SEQ - 4bit 编码)]
        K --> V[Quality (QUAL)]
        K --> W[Auxiliary Tags (Tag-Type-Value)]
    end

    A --> E
    G --> J
    G --> K
```

理解 SAM/BAM API 和 `bam1_t` 结构是使用 HTSlib 处理比对数据的关键。 
# HTSlib VCF/BCF 处理

## 1. VCF/BCF 格式简介

*   **VCF (Variant Call Format)**：一种文本格式，用于存储基因组序列变异信息，如 SNP (Single Nucleotide Polymorphism)、Indel (Insertion/Deletion) 和结构变异 (Structural Variation)。它也包含一个元信息头部（以 `##` 开头，定义 INFO, FORMAT, FILTER 等字段）和一个数据体部分（以 `#CHROM` 开头的表头行，随后是每行一个变异记录）。
*   **BCF (Binary Call Format)**：VCF 格式的二进制版本。它存储与 VCF 相同的信息，但采用二进制编码并通常与 BGZF 压缩结合使用 (`.bcf` 文件)。BCF 文件体积更小，解析速度更快，并且支持通过索引进行快速随机访问。

## 2. HTSlib 中的核心结构

HTSlib 使用以下主要结构体来处理 VCF/BCF 数据（定义在 `htslib/vcf.h`）：

*   **`vcf_hdr_t`**: 表示 VCF/BCF 文件的头部信息。
    *   存储元信息行（如 `##INFO`, `##FORMAT`, `##FILTER`, `##contig`）和样本列表。
    *   提供 `bcf_hdr_init()`, `bcf_hdr_read()`, `bcf_hdr_write()`, `bcf_hdr_parse()`, `bcf_hdr_add_sample()`, `bcf_hdr_name2id()` 等函数来创建、读写、解析和查询头部信息。
    *   头部对于解释变异记录中的字段（如 INFO, FORMAT 标签）和样本基因型至关重要。
*   **`bcf1_t`**: 表示单个变异记录。
    *   这是核心结构，包含一个变异位点的所有信息，如染色体 (`CHROM` -> `rid`)、位置 (`POS`)、ID (`ID` 字段)、参考等位基因 (`REF`)、替代等位基因 (`ALT`)、质量 (`QUAL`)、过滤器 (`FILTER`)、信息字段 (`INFO`) 和样本基因型信息 (`FORMAT` 和样本列)。
    *   **统一处理 VCF/BCF**: HTSlib 设计使得 `bcf1_t` 可以同时表示 VCF 和 BCF 记录。读取 VCF 行时，会将其解析并填充到 `bcf1_t` 结构中；读取 BCF 记录时，则直接解码到 `bcf1_t`。
    *   **动态与共享数据**: `bcf1_t` 内部包含固定部分 (`core`) 和可变长度、可共享的部分 (`shared`, `indiv`)。`shared` 部分包含所有样本共享的信息（CHROM, POS, ID, REF, ALT, QUAL, FILTER, INFO），`indiv` 部分包含每个样本独有的信息（FORMAT 字段的值）。HTSlib 提供了 `bcf_unpack()` 函数来解开这些数据以供访问。
    *   相关 API：`bcf_init1()`, `bcf_destroy1()`, `bcf_read()`, `bcf_write()`, `bcf_unpack()`, `bcf_get_info()`, `bcf_get_format()`, `bcf_get_genotypes()` 等。

## 3. 主要 API 函数

*   **打开/关闭文件**: 使用 `hts_open()` 打开 VCF/BCF 文件（通常是 BGZF 压缩的 VCF.gz 或 BCF），`hts_close()` 关闭。
*   **读写头部**: 使用 `bcf_hdr_read()` 读取头部，`bcf_hdr_write()` 写入头部。
*   **读写记录**: 使用 `bcf_read()` (或 `bcf_read1`) 读取下一条变异记录到 `bcf1_t` 结构，`bcf_write()` (或 `bcf_write1`) 将 `bcf1_t` 结构写入文件。
*   **记录解包与访问**: 读取记录后，通常需要调用 `bcf_unpack()` 来解码记录中的 `INFO`, `FORMAT` 等字段，使其可被 `bcf_get_*` 系列函数访问。
    *   `bcf_unpack(record, BCF_UN_INFO)`: 解包 INFO 字段。
    *   `bcf_unpack(record, BCF_UN_FMT)`: 解包 FORMAT 字段。
    *   `bcf_unpack(record, BCF_UN_ALL)`: 解包所有可解包的字段。
*   **访问数据**: 使用 `bcf_get_info_*()`, `bcf_get_format_*()`, `bcf_get_genotypes()` 等函数获取具体的变异信息和样本基因型。
*   **迭代器**: 支持使用 `hts_iterator_t` 配合索引文件 (Tabix `.tbi` 或 CSI `.csi`) 进行区域查询。

## 4. BCF 记录内部结构示意 (Mermaid)

```mermaid
graph TD
    A[BCF 记录 (内存中 bcf1_t)]
    A --> B(共享部分 shared)
    A --> C(个体部分 indiv)

    subgraph shared 部分
        B --> D[l_shared (共享部分长度)]
        B --> E[CHROM (rid)]
        B --> F[POS]
        B --> G[rlen (REF 长度)]
        B --> H[QUAL]
        B --> I[n_allele (等位基因数)]
        B --> J[n_info, n_fmt (INFO/FORMAT 字段数)]
        B --> K[ID (字符串)]
        B --> L[REF (字符串)]
        B --> M[ALT (逗号分隔字符串)]
        B --> N[FILTER (ID 列表)]
        B --> O[INFO 字段 (键值对序列)]
    end

    subgraph indiv 部分
        C --> P[l_indiv (个体部分长度)]
        C --> Q[FORMAT 字段 (键值对序列) x n_sample]
    end

    R{bcf_unpack() 解包}
    O -- R --> S[可访问的 INFO 值]
    Q -- R --> T[可访问的 FORMAT 值 / Genotypes]

    U[VCF 文本行] <-- 解析/格式化 --> A
```

通过 `vcf.h` 提供的 API 和 `bcf1_t` 结构，HTSlib 实现了对 VCF 和 BCF 格式的统一、高效处理。 
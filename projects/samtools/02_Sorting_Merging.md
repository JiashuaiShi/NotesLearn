# Samtools 排序与合并

## 1. samtools sort: 比对记录排序

排序是许多下游分析（如索引、去重、区域查询）的必要前提步骤。`samtools sort` 提供了对比对记录进行排序的功能。

*   **主要排序方式:**
    *   **按坐标排序 (Coordinate Sort):** 这是最常用的排序方式，也是 `samtools index` 的要求。命令: `samtools sort in.bam -o sorted.bam` (默认按坐标排序)。记录会首先按参考序列名称（RNAME）排序，然后在同一参考序列内按起始位置（POS）排序。
    *   **按读段名称排序 (Name Sort):** 主要用于需要处理配对读段（paired-end reads）关系的步骤，例如 `samtools fixmate`。命令: `samtools sort -n in.bam -o name_sorted.bam`。
*   **算法与性能:**
    *   对于大型文件，`samtools sort` 通常采用**外部排序 (External Sort)** 算法。它会将文件分割成多个小块（chunks），在内存中对每个块进行排序，然后将排序后的块写入临时文件。最后，通过多路归并 (multi-way merge) 的方式将所有临时文件合并成一个最终的排序文件。
    *   **多线程:** 可以使用 `-@` 参数指定线程数来加速排序过程，尤其是在多路归并阶段。
    *   **内存控制:** `-m` 参数用于指定每个线程可使用的最大内存，影响内存中排序块的大小。合理设置内存可以提高效率，但过大会导致资源浪费或系统不稳定。
*   **其他选项:**
    *   压缩级别 (`-l`): 控制输出 BAM 文件的压缩级别。
    *   临时文件目录 (`-T`): 指定存放临时排序文件的目录。

## 2. samtools merge: 合并多个排序文件

`samtools merge` 用于将多个**已按坐标排序**的 SAM/BAM/CRAM 文件合并成一个单一的、同样按坐标排序的文件。

*   **基本用法:** `samtools merge merged.bam sorted1.bam sorted2.bam sorted3.bam ...`
*   **输入要求:** 所有输入文件必须是**按坐标排序**的，并且它们的头信息 (header) 必须兼容（例如，使用相同的参考序列字典）。`merge` 命令会自动处理头信息的合并。
*   **应用场景:**
    *   合并来自不同测序泳道 (lane) 或批次的比对结果。
    *   合并经过分片处理（例如按染色体拆分）后的排序文件。
*   **选项:**
    *   `-h file.sam`: 使用 `file.sam` 中的头信息作为输出文件的头信息模板。
    *   `-r`: 合并后，仅保留比对到输入文件中头信息 `@RG` 行所定义参考序列区域的比对。
    *   `-c`: 当合并文件的 `@RG` ID 冲突时，将 RG ID 附加到 QNAME 上。
    *   `-p`: 使用和 `samtools sort -n` 类似的名称排序（通常不用于标准合并流程）。

## 3. 工作流程示意图

```mermaid
graph TD
    subgraph "排序 (sort)"
        A[Input SAM/BAM/CRAM] --> B{samtools sort};
        B -- 按坐标 --> C[Coordinate Sorted BAM/CRAM];
        B -- 按名称 --> D[Name Sorted BAM/CRAM];
        B -- 使用多线程/内存 --> B;
    end

    subgraph "合并 (merge)"
        E[Coord Sorted BAM 1] --> G{samtools merge};
        F[Coord Sorted BAM 2] --> G;
        H[...] --> G;
        G --> I[Merged Coord Sorted BAM];
        J[Header Template (Optional)] -- -h --> G;
    end

    style A fill:#lightblue,stroke:#333
    style C fill:#lightgreen,stroke:#333
    style D fill:#khaki,stroke:#333
    style E fill:#lightgreen,stroke:#333
    style F fill:#lightgreen,stroke:#333
    style H fill:#lightgreen,stroke:#333
    style I fill:#orange,stroke:#333
    style J fill:#grey,stroke:#333
``` 
# HTSlib I/O 与 BGZF

## 1. 统一 I/O 接口: `htsFile`

HTSlib 的核心设计之一是 `htsFile` 结构体和相关的 API，它提供了一个统一的文件访问层。无论底层是本地文件、标准输入/输出、管道还是网络流 (http, https, ftp)，用户都可以使用相同的 `hts_open`, `hts_read`, `hts_write`, `hts_close` 等函数进行操作。

**主要特点：**

*   **协议透明**：自动处理不同的 URI 协议（如 `file://`, `http://`, `ftp://`）以及普通文件路径。
*   **格式检测**：`hts_open` 可以自动检测输入流的数据格式（SAM, BAM, CRAM, VCF, BCF, FASTA, FASTQ 等），并据此设置合适的处理模式。
*   **压缩感知**：自动处理 BGZF 压缩/解压缩。当打开一个 `.gz` 后缀的文件或明确指定需要压缩输出时，`htsFile` 会在后台透明地进行 BGZF 操作。
*   **缓冲**：内部实现了缓冲机制，减少底层系统调用的次数，提高效率。

**示例用法:**

```c
#include "htslib/hts.h"

int main() {
    // 打开文件 (可以是本地文件、URL 或 "-" 代表 stdin/stdout)
    htsFile *fp = hts_open("input.bam", "rb"); // 以二进制读模式打开
    if (fp == NULL) { /* 错误处理 */ }

    // ... 使用 hts_read 等函数读取数据 ...

    // 关闭文件
    if (hts_close(fp) < 0) { /* 错误处理 */ }

    return 0;
}
```

## 2. BGZF (Blocked GNU Zip Format)

BGZF 是 HTSlib（以及许多生物信息学工具）广泛使用的压缩格式。它基于标准的 Gzip (DEFLATE 算法)，但增加了**分块 (Blocking)** 的特性。

**关键点：**

*   **分块压缩**：数据被分割成多个块（通常最大 64KB），每个块独立进行 Gzip 压缩。每个块包含压缩数据以及原始未压缩数据的长度。
*   **支持随机访问**：由于数据是分块压缩的，可以通过查找块的起始位置（虚拟文件偏移量，Virtual File Offset）来直接跳转到文件的特定区域进行解压，而无需从头解压整个文件。这是实现 BAM、BCF 等格式快速区域查询的基础。
*   **虚拟文件偏移量 (Virtual File Offset)**：一个 64 位整数，高 48 位表示该 BGZF 块在压缩文件中的起始字节偏移量，低 16 位表示解压后数据在该块内的字节偏移量。
*   **`bgzf.h` API**：HTSlib 提供了 `bgzf_open`, `bgzf_read`, `bgzf_write`, `bgzf_seek`, `bgzf_close` 等底层 API 来直接操作 BGZF 文件。`htsFile` 在需要时会内部调用这些 API。

## 3. I/O 流程图 (Mermaid)

```mermaid
graph TD
    A[用户调用 hts_read/hts_write] --> B{htsFile 实例}
    B -- 检查格式和压缩 --> C{需要 BGZF?}
    C -- 是 --> D[调用 BGZF API (bgzf_read/bgzf_write)]
    C -- 否 --> E[直接读写底层流]
    D --> F{BGZF 缓冲和块处理}
    F -- 进行压缩/解压 --> G[读写 Gzip 块]
    G --> H[底层文件/网络流]
    E --> H

    subgraph 网络流处理 (可选)
        H -- 如果是 URL --> I[libcurl 等库]
    end
```

这个模块是 HTSlib 实现其高效、灵活文件操作的基础。 
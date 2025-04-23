# HTSlib 线程支持

## 1. 目的与背景

处理大型 HTS 数据文件时，主要的性能瓶颈之一是 I/O 操作，特别是涉及压缩（如 BGZF 写入）和解压缩（如 BGZF 读取）的过程。这些压缩/解压缩步骤是计算密集型的。

为了利用现代多核处理器的优势，HTSlib 引入了一个可选的线程池 (`hts_tpool`) 机制，专门用于**并行化 BGZF 块的压缩和解压缩**。

## 2. 核心机制

*   **线程池 (`hts_tpool`)**: HTSlib 可以创建一个包含多个后台工作线程的池。
*   **任务分发**: 当主线程需要压缩或解压缩一个 BGZF 块时，如果配置了线程池，它可以将这个任务（连同数据块）提交给线程池中的一个空闲线程去执行。
*   **异步执行**: 主线程提交任务后可以继续执行其他操作（例如，准备下一个要处理的数据块，或者处理已完成解压的数据），而工作线程在后台并行地执行压缩/解压缩。
*   **结果回收**: 主线程在需要压缩/解压缩结果时，会等待相应的后台任务完成。

**关键点:**

*   这种多线程主要加速的是**计算密集**的压缩/解压缩部分，而不是磁盘 I/O 本身。
*   它对于**写入** BGZF 压缩文件（如 BAM, BCF）通常效果最显著，因为压缩通常比解压缩更耗时。
*   对于读取，当解压缩速度是瓶颈时（例如，CPU 速度相对较慢，或文件压缩比较高），也能带来性能提升。
*   线程数并非越多越好，最佳线程数取决于 CPU 核心数、磁盘速度以及具体的工作负载。通常设置为 CPU 核心数减一或稍小的值是一个合理的起点。

## 3. API 使用 (`hts.h`)

*   **创建线程池**: `hts_tpool *hts_tpool_init(int n_threads)`
    *   创建一个包含 `n_threads` 个工作线程的池。返回线程池指针，失败则返回 NULL。
*   **销毁线程池**: `void hts_tpool_destroy(hts_tpool *pool)`
    *   等待所有任务完成，然后清理并释放线程池资源。
*   **将线程池附加到 `htsFile`**: `int hts_set_opt(htsFile *fp, enum hts_fmt_option opt, ...)`
    *   使用选项 `HTS_OPT_THREAD_POOL` 将一个已创建的 `hts_tpool` 与一个 `htsFile` 关联起来。
    *   示例: `hts_set_opt(fp, HTS_OPT_THREAD_POOL, &pool_struct);` 其中 `pool_struct` 是包含 `hts_tpool*` 和输入/输出队列大小的结构体指针。
    *   **更简便的方法**: `int hts_set_threads(htsFile *fp, int n_threads)`
        *   这是推荐的、更高级的接口。它会自动处理线程池的创建（如果需要）和关联。只需指定希望用于该文件操作的线程数。
        *   如果 `n_threads > 0`，HTSlib 会尝试使用 `n_threads` 个后台线程进行压缩/解压缩。
        *   如果 `n_threads = 0`，则禁用后台线程，所有操作在主线程同步执行。
        *   **必须在打开文件后、进行任何读写操作之前调用**。
*   **将线程池附加到 BGZF 文件**: `int bgzf_mt(BGZF *fp, int n_threads, int n_sub_threads)`
    *   这是更底层的接口，直接作用于 `BGZF` 句柄。`hts_set_threads()` 内部会调用类似的功能。

## 4. 使用流程示意 (Mermaid)

```mermaid
graph TD
    A[开始] --> B{创建线程池 (可选, hts_set_threads 更常用)}
    B -- hts_tpool_init --> C[hts_tpool *pool]
    A --> D[打开 htsFile]
    D --> E{设置线程数}
    E -- hts_set_threads(fp, n) --> F[HTSlib 内部管理线程池并关联到 fp]
    C --> G{关联线程池 (可选, 如果手动创建)}
    G -- hts_set_opt(fp, HTS_OPT_THREAD_POOL, ...) --> F
    F --> H[执行 hts_read / hts_write]
    H --> I{需要压缩/解压 BGZF 块?}
    I -- 是 --> J{任务提交给线程池}
    J --> K[工作线程并行执行]
    I -- 否 --> L[主线程直接处理]
    K --> M{主线程等待/获取结果}
    M & L --> N[处理完成的数据]
    N --> H
    H -- 文件结束/关闭 --> O[关闭 htsFile]
    O --> P{销毁线程池 (hts_set_threads 会自动处理)}
    C --> P
    P --> Q[结束]
```

通过使用 `hts_set_threads()`，可以相对简单地为 HTSlib 的文件操作启用多线程加速，从而在多核系统上提高处理 BGZF 压缩文件的效率。 
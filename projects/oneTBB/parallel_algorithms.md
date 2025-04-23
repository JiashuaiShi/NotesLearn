# oneTBB 模块: 并行算法 (Parallel Algorithms)

## 概述

该模块提供了一系列通用的并行算法模板，用于替代 C++ 标准库中的串行循环和算法，充分利用多核处理器的计算能力。这些算法通常基于"分而治之"的策略，将大的任务分解成可以并行处理的小任务。

## 核心组件与功能

1.  **`parallel_for`**:
    *   **功能**: 并行地对一个范围（Range）执行指定的函数体（Body）。这是最常用的并行模式之一，适用于可以独立处理数据元素的循环。
    *   **机制**: 接受一个范围对象（如 `blocked_range`）、一个定义了 `operator()` 的函数对象（Body），以及一个可选的分区器（Partitioner）。分区器决定如何将范围分割成子范围交给不同的线程处理（如 `auto_partitioner`, `simple_partitioner`, `static_partitioner`）。
    *   **使用场景**: 替代 `for` 循环，用于数据并行处理，例如并行处理数组或容器中的元素。

2.  **`parallel_reduce`**:
    *   **功能**: 并行地对一个范围进行计算，并将各个子范围的计算结果进行归约（合并）得到最终结果。
    *   **机制**: 除了需要范围和函数体（Body），Body还需要定义一个 `join` 方法来合并两个子任务的结果。它也接受分区器。TBB 保证 `join` 操作的线程安全性。
    *   **使用场景**: 计算总和、查找最大/最小值、并行构建数据结构等需要合并局部结果的操作。还存在 `parallel_deterministic_reduce` 用于保证归约顺序的确定性（以牺牲部分性能为代价）。

3.  **`parallel_scan`**: (基于 `include/oneapi/tbb/parallel_scan.h` - 待分析)
    *   **功能**: 执行并行前缀扫描（或称为并行累加）。它计算一个序列的部分和（或其他结合操作的结果）。
    *   **使用场景**: 计算数组的前缀和、数据压缩、直方图生成等。

4.  **`parallel_sort`**: (基于 `include/oneapi/tbb/parallel_sort.h` - 待分析)
    *   **功能**: 对一个序列进行并行排序。
    *   **使用场景**: 对大规模数据集进行快速排序。

5.  **`parallel_invoke`**: (基于 `include/oneapi/tbb/parallel_invoke.h` - 待分析)
    *   **功能**: 并行地执行多个独立的函数对象（任务）。
    *   **使用场景**: 当有多个不相关的计算任务可以同时执行时。

6.  **`parallel_for_each`**: (基于 `include/oneapi/tbb/parallel_for_each.h` - 待分析)
    *   **功能**: 对容器中的每个元素并行应用一个函数。与 `parallel_for` 类似，但通常直接作用于迭代器。
    *   **使用场景**: 并行处理标准库容器（如 `std::vector`, `std::list`）中的元素。

7.  **`parallel_pipeline`**: (基于 `include/oneapi/tbb/parallel_pipeline.h` - 待分析)
    *   **功能**: 构建一个并行的流水线处理模式，数据在多个处理阶段（Filter）之间流动。
    *   **使用场景**: 流式数据处理、图像处理、信号处理等具有阶段性依赖的任务。

## 设计思想

*   **任务并行**: 算法内部通常基于 TBB 的任务调度系统，将工作分解为小任务。
*   **分治策略**: 大多采用分而治之，递归地将问题分解，直到任务粒度足够小，然后并行处理，最后合并结果（如 `parallel_reduce`）。
*   **分区器 (Partitioner)**: 提供了对任务分割策略的控制，允许用户根据任务特性选择合适的分割方式以优化性能（如 `simple_partitioner` 用于简单、均匀的任务分割，`auto_partitioner` 自动适应负载，`static_partitioner` 用于静态分割）。
*   **范围 (Range)**: 使用 `Range` 概念（如 `blocked_range`）来定义可分割的工作区间。

## 依赖关系

*   依赖于 TBB 的任务调度模块。
*   依赖于 TBB 的分区器 (`partitioner.h`) 和范围类型 (`blocked_range.h` 等)。
*   依赖于底层的任务 (`task.h`) 和任务组上下文 (`task_group.h`)。

*(注意: 此文档是基于对部分核心头文件的初步分析生成的，后续可根据需要进行补充和细化。)* 
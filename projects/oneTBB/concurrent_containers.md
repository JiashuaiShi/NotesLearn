# oneTBB 模块: 并发容器 (Concurrent Containers)

## 概述

该模块提供了一系列线程安全的容器类，旨在替代标准库中非线程安全的对应容器（如 `std::vector`, `std::queue`, `std::unordered_map` 等），使得在多线程环境下可以安全、高效地共享和修改数据，而无需外部加锁（或显著减少锁的需求）。这些容器针对并发访问进行了优化。

## 核心组件与功能

1.  **`concurrent_vector<T>`**:
    *   **功能**: 线程安全的动态数组，类似于 `std::vector`。
    *   **特点**: 
        *   **并发增长**: 允许多个线程同时通过 `push_back()` 或 `grow_by()` 等方法向容器添加元素。内部通过分段（segment）存储实现，避免了全局锁和大规模数据复制。
        *   **并发访问**: 允许一个线程增长容器的同时，其他线程安全地读取或写入已存在的元素（通过 `operator[]` 或 `at()`）。
        *   **随机访问**: 提供随机访问迭代器。
    *   **使用场景**: 需要动态数组功能，并且会有并发写入（尤其是尾部插入）和并发读取的场景。

2.  **`concurrent_hash_map<Key, T, HashCompare, Allocator>`**:
    *   **功能**: 线程安全的哈希表，类似于 `std::unordered_map`。
    *   **特点**:
        *   **高并发**: 内部实现通常采用分桶（bucket）锁或者更细粒度的锁策略，允许多个线程同时对哈希表的不同部分进行读写操作（插入、查找、删除）。
        *   **动态调整**: 哈希表可以根据需要动态增长（rehashing）以保持性能。
        *   **访问器 (Accessor)**: 通常通过 `accessor` 或 `const_accessor` 对象来安全地访问元素，这些访问器在其生命周期内持有对元素的锁。
    *   **使用场景**: 需要键值对存储，并且有高并发的查找、插入、更新和删除操作。

3.  **`concurrent_queue<T>`**: (基于 `include/oneapi/tbb/concurrent_queue.h` - 待分析)
    *   **功能**: 线程安全的队列，支持并发的入队（push）和出队（pop）操作。
    *   **类型**: 可能包含多种实现，如 `concurrent_bounded_queue` (有界队列) 和 `concurrent_unbounded_queue` (无界队列)。
    *   **使用场景**: 生产者-消费者模式，任务队列等需要多线程安全地传递数据的场景。

4.  **`concurrent_priority_queue<T>`**: (基于 `include/oneapi/tbb/concurrent_priority_queue.h` - 待分析)
    *   **功能**: 线程安全的优先队列。
    *   **使用场景**: 需要根据优先级并发处理任务或数据的场景。

5.  **`concurrent_unordered_map`, `concurrent_unordered_set`, `concurrent_map`, `concurrent_set`**: (基于对应头文件 - 待分析)
    *   **功能**: 提供与标准库对应的其他并发容器，如无序集合、有序映射和有序集合。
    *   **特点**: 同样针对并发访问进行了优化，但具体并发策略和性能特征可能与 `concurrent_hash_map` 不同（例如，有序容器可能并发度较低）。

6.  **`concurrent_lru_cache<Key, T>`**: (基于 `include/oneapi/tbb/concurrent_lru_cache.h` - 待分析)
    *   **功能**: 线程安全的最近最少使用（LRU）缓存。

## 设计思想

*   **减少锁争用**: 通过细粒度锁（如分段锁、分桶锁）或无锁技术（在可能的情况下）来最大化并发度。
*   **特定操作优化**: 针对特定的并发操作（如 `concurrent_vector` 的 `push_back`）进行特殊设计以提高效率。
*   **接口兼容性**: 尽量保持与标准库容器相似的接口，降低学习和使用成本。
*   **安全访问**: 使用访问器（Accessor）等机制确保即使在并发修改容器结构（如 rehash）时，对元素的安全访问。

## 依赖关系

*   可能依赖 TBB 的内存管理模块（如 `tbb_allocator`）。
*   可能依赖 TBB 的同步原语（如 `spin_rw_mutex`）。
*   依赖 C++ 标准库（如 `<atomic>`, `<utility>` 等）。

*(注意: 此文档是基于对 `concurrent_vector` 和 `concurrent_hash_map` 的初步分析生成的，后续可根据需要进行补充和细化。)* 
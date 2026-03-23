# Data Structures & Algorithms in C++

A comprehensive reference library covering every major data structure and algorithm, implemented from scratch in modern C++17.

---

## Directory Structure

```
data_structures_and_algorithms/
├── stack/
├── queue/
├── linked_list/
├── hash_table/
├── sorting/
├── heap/
├── trees/
├── graphs/
├── disjoint_set/
├── strings/
├── dynamic_programming/
├── bit_manipulation/
├── matrix/
├── two_pointers/
├── sliding_window/
├── backtracking/
├── greedy/
├── math/
└── recursion/
```

---

## Contents

### Stack
| File | Description |
|------|-------------|
| `array_stack.cpp` | Fixed-size array-based stack |
| `linked_list_stack.cpp` | Dynamic linked-list stack |
| `min_stack.cpp` | Stack with O(1) getMin |
| `monotonic_stack.cpp` | Next greater element, largest rectangle in histogram, daily temperatures |

### Queue
| File | Description |
|------|-------------|
| `array_queue.cpp` | Circular array queue |
| `linked_list_queue.cpp` | Linked-list queue |
| `deque.cpp` | Doubly-ended queue + sliding window maximum |
| `priority_queue.cpp` | Binary heap PQ (min & max) + heap sort |

### Linked List
| File | Description |
|------|-------------|
| `singly_linked_list.cpp` | Full ops: insert, delete, reverse, cycle detection, merge sorted, remove nth from end |
| `doubly_linked_list.cpp` | Bidirectional traversal, insert after, delete node |
| `circular_linked_list.cpp` | Circular singly linked list + Josephus problem |

### Hash Table
| File | Description |
|------|-------------|
| `hash_table_chaining.cpp` | Separate chaining with dynamic rehashing |
| `hash_table_open_addressing.cpp` | Double hashing with lazy deletion |

### Sorting
| File | Description |
|------|-------------|
| `bubble_sort.cpp` | Bubble sort + Cocktail shaker sort |
| `selection_sort.cpp` | Selection sort + Stable selection |
| `insertion_sort.cpp` | Insertion sort + Binary insertion + Shell sort |
| `merge_sort.cpp` | Recursive & iterative merge sort + count inversions |
| `quick_sort.cpp` | Lomuto / Hoare / 3-way / Randomized quicksort + Quickselect |
| `heap_sort.cpp` | Heap sort + k-th largest + sort nearly sorted array |
| `counting_radix_bucket_sort.cpp` | Counting sort + Radix sort (LSD) + Bucket sort |

### Heap
| File | Description |
|------|-------------|
| `min_heap.cpp` | Min-heap: push, pop, build O(n), decrease key, k smallest |
| `max_heap.cpp` | Max-heap + Median of a Data Stream (two-heap trick) |

### Trees
| File | Description |
|------|-------------|
| `binary_tree.cpp` | All traversals (recursive, iterative, Morris), diameter, LCA, max path sum, serialization |
| `bst.cpp` | BST insert/delete/search, floor, ceil, kth smallest, LCA |
| `avl_tree.cpp` | Self-balancing BST with LL/LR/RL/RR rotations |
| `red_black_tree.cpp` | Full Red-Black Tree with insert/delete fixup |
| `b_tree.cpp` | B-Tree of order t — used in databases & file systems |
| `segment_tree.cpp` | Generic segment tree (sum/min/max) + lazy propagation range updates |
| `fenwick_tree.cpp` | 1D & 2D Binary Indexed Tree, prefix sums, k-th element |
| `trie.cpp` | Standard trie + XOR trie (max XOR) + autocomplete |
| `sparse_table.cpp` | O(1) RMQ + LCA via Euler tour + Binary Lifting LCA |

### Graphs
| File | Description |
|------|-------------|
| `bfs_dfs.cpp` | BFS, DFS (recursive & iterative), shortest path (unweighted), cycle detection, bipartite check |
| `dijkstra.cpp` | Dijkstra's SSSP with priority queue + path reconstruction |
| `bellman_ford_floyd_warshall.cpp` | Bellman-Ford (negative weights), SPFA, Floyd-Warshall all-pairs |
| `topological_sort.cpp` | Kahn's (BFS) + DFS-based topo sort, course schedule, DAG shortest path |
| `minimum_spanning_tree.cpp` | Kruskal's, Prim's, Borůvka's algorithms |
| `scc_articulation.cpp` | Kosaraju & Tarjan SCC, articulation points, bridges |
| `network_flow.cpp` | Edmonds-Karp, Dinic's max flow, bipartite matching |

### Disjoint Set
| File | Description |
|------|-------------|
| `union_find.cpp` | Union-Find with path compression + union by rank/size; number of islands, cycle detection, accounts merge |

### Strings
| File | Description |
|------|-------------|
| `kmp_rabin_karp.cpp` | KMP, Rabin-Karp, Z-algorithm, Boyer-Moore, Manacher's (longest palindrome) |
| `suffix_array_lcp.cpp` | Suffix array O(n log² n) + Kasai LCP O(n), pattern search, longest repeated/common substring |
| `string_algorithms.cpp` | Aho-Corasick multi-pattern search, rolling hash, longest duplicate substring, regex DP |

### Dynamic Programming
| File | Description |
|------|-------------|
| `classic_dp.cpp` | Fibonacci, 0/1 Knapsack, unbounded knapsack, coin change, LCS, LIS O(n log n), edit distance, matrix chain, LPS, Kadane's, rod cutting, partition subset sum, egg drop |
| `interval_tree_dp.cpp` | Burst balloons, palindrome cuts, TSP (bitmask DP), assignment problem, Tree DP, Digit DP |

### Bit Manipulation
| File | Description |
|------|-------------|
| `bit_tricks.cpp` | Get/set/clear/toggle bits, count bits (Kernighan), XOR single/two/three-time numbers, gray code, bitmask subsets, all-unique string check |

### Matrix
| File | Description |
|------|-------------|
| `matrix_operations.cpp` | Matrix multiply, matrix exponentiation (Fibonacci), rotate 90°, spiral order, sorted matrix search, set zeroes, maximal rectangle, number of islands |

### Two Pointers
| File | Description |
|------|-------------|
| `two_pointers.cpp` | Two sum, 3Sum, trapping rain water, container with most water, remove duplicates, Dutch national flag, valid palindrome, minimum window substring |

### Sliding Window
| File | Description |
|------|-------------|
| `sliding_window.cpp` | Max sum subarray, sliding window maximum (deque), longest substring without repeating, longest k-distinct, min subarray sum, find anagrams, max consecutive ones, permutation in string |

### Backtracking
| File | Description |
|------|-------------|
| `backtracking.cpp` | N-Queens, Sudoku solver, subsets (with/without duplicates), permutations, combination sum, word search, generate parentheses |

### Greedy
| File | Description |
|------|-------------|
| `greedy.cpp` | Activity selection, interval merge, weighted job scheduling, Huffman coding, fractional knapsack, jump game I & II, gas station, task scheduler, meeting rooms II |

### Math
| File | Description |
|------|-------------|
| `math_algorithms.cpp` | GCD/LCM, extended GCD, modular exponentiation, modular inverse, Sieve of Eratosthenes, segmented sieve, prime factorization, Pascal's row, nCr with modular arithmetic, Chinese Remainder Theorem, Euler's totient, Miller-Rabin primality |

### Recursion
| File | Description |
|------|-------------|
| `recursion.cpp` | Tower of Hanoi, binary search, power set, string permutations, staircase (memoized), fast power, flood fill, binary strings, palindrome check |

---

## Complexity Reference

| Algorithm | Time | Space |
|-----------|------|-------|
| Binary Search | O(log n) | O(1) |
| Merge Sort | O(n log n) | O(n) |
| Quick Sort (avg) | O(n log n) | O(log n) |
| Heap Sort | O(n log n) | O(1) |
| Counting Sort | O(n + k) | O(k) |
| Radix Sort | O(d·n) | O(n) |
| BFS / DFS | O(V + E) | O(V) |
| Dijkstra | O((V+E) log V) | O(V) |
| Bellman-Ford | O(V·E) | O(V) |
| Floyd-Warshall | O(V³) | O(V²) |
| Kruskal / Prim | O(E log E) | O(V) |
| Dinic's Max Flow | O(V²·E) | O(V+E) |
| AVL / RB Tree | O(log n) | O(n) |
| Segment Tree | O(log n) query | O(n) |
| Fenwick Tree | O(log n) | O(n) |
| Trie | O(m) per op | O(ALPHABET·n) |
| Union-Find (α) | O(α(n)) ≈ O(1) | O(n) |
| KMP | O(n + m) | O(m) |
| Suffix Array | O(n log² n) | O(n) |
| Aho-Corasick | O(n + m + z) | O(m·ALPHABET) |
| LCS / Edit Distance | O(n·m) | O(n) or O(n·m) |
| LIS | O(n log n) | O(n) |
| 0/1 Knapsack | O(n·W) | O(W) |
| TSP (bitmask) | O(2ⁿ·n²) | O(2ⁿ·n) |

---

## How to Compile & Run

Each file is standalone. Compile with:

```bash
g++ -std=c++17 -O2 -o output <file>.cpp && ./output
```

Example:
```bash
g++ -std=c++17 -O2 -o out data_structures_and_algorithms/trees/avl_tree.cpp && ./out
```

---

## Topics Covered

- **Linear DS**: Stack, Queue, Deque, Linked List (singly, doubly, circular)
- **Hashing**: Chaining, Open Addressing, Rolling Hash
- **Trees**: Binary Tree, BST, AVL, Red-Black, B-Tree, Segment Tree, Fenwick Tree, Trie, Sparse Table
- **Graphs**: BFS/DFS, Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim, Borůvka, Topological Sort, SCC, Articulation Points, Network Flow
- **Sorting**: Bubble, Selection, Insertion, Shell, Merge, Quick, Heap, Counting, Radix, Bucket
- **Dynamic Programming**: Classical 1D/2D, Interval, Tree, Bitmask, Digit DP
- **Strings**: KMP, Rabin-Karp, Z-algorithm, Boyer-Moore, Manacher, Suffix Array, Aho-Corasick
- **Math**: Number theory, Combinatorics, Modular arithmetic, Primality tests
- **Paradigms**: Greedy, Backtracking, Divide & Conquer, Two Pointers, Sliding Window, Recursion, Bit Manipulation

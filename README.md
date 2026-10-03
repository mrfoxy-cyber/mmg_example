# MMG Graph Search Example

A small, standalone C++20 example inspired by the graph-traversal concepts used in
Mental Model Graph (MMG). It demonstrates two classic ways to find a path through
a directed graph:

- **Breadth-first search (BFS)** explores level by level and returns a shortest
  path in an unweighted graph.
- **Depth-first search (DFS)** follows one branch as far as possible before
  backtracking.

This repository is intentionally independent of the private MMG codebase. It is
designed as a readable portfolio example with no third-party dependencies.

## Example graph

```text
source ──► parser ──► validator ──► graph ──► report
   │                                  ▲
   └──────────► indexer ──────────────┘
```

With neighbors visited in insertion order:

```text
BFS: source -> indexer -> graph -> report
DFS: source -> parser -> validator -> graph -> report
```

## Build and run

You need CMake 3.16 or newer and a compiler with C++20 support.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Run the demonstration:

```sh
./build/mmg_graph_example
```

On a multi-configuration generator such as Visual Studio, the executable may be
under `build/Debug/` or `build/Release/`.

## Project layout

```text
include/mmg_example/graph.hpp  Public graph API
src/graph.cpp                  BFS and DFS implementations
src/main.cpp                   Runnable example
tests/graph_tests.cpp          Lightweight behavior tests
```

## Design notes

- Nodes are identified by strings so the examples read like a software graph.
- Edges are directed and duplicate edges are ignored.
- Traversal order is deterministic: neighbors are considered in insertion order.
- Both searches return `std::nullopt` when no path exists.
- BFS and DFS are iterative, avoiding recursion-depth limits on large graphs.

## License

MIT — see [LICENSE](LICENSE).

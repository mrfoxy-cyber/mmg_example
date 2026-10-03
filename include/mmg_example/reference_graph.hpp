#pragma once

#include "mmg_example/graph.hpp"

namespace mmg_example {

inline Graph makeReferenceGraph() {
    Graph graph;
    const auto edge = [&graph](const char* from, const char* to) {
        static_cast<void>(graph.addEdge(from, to));
    };

    // Main island.
    edge("X", "C");
    edge("X", "R");
    edge("C", "O1");
    edge("R", "O1");
    edge("O1", "E");
    edge("E", "G1");
    edge("E", "T");
    edge("G1", "T");
    edge("T", "G1");
    edge("T", "M");
    edge("M", "Z");
    edge("M", "Q");
    edge("Z", "O2");
    edge("Q", "O2");

    // Dense left island.
    edge("F", "K");
    edge("F", "I");
    edge("F", "L");
    edge("K", "I");
    edge("K", "G2");
    edge("I", "K");
    edge("I", "L");
    edge("I", "H");
    edge("L", "I");
    edge("L", "H");
    edge("L", "F");
    edge("H", "G2");

    // Isolated pair island.
    edge("N", "D");
    edge("D", "N");
    return graph;
}

}  // namespace mmg_example

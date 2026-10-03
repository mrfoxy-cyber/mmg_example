#include "mmg_example/graph.hpp"

#include <iostream>
#include <string_view>

namespace {

void printPath(
    const std::string_view label,
    const std::optional<mmg_example::Path>& path) {
    std::cout << label << ": ";
    if (!path) {
        std::cout << "no path\n";
        return;
    }

    for (std::size_t index = 0; index < path->size(); ++index) {
        if (index > 0) {
            std::cout << " -> ";
        }
        std::cout << path->at(index);
    }
    std::cout << '\n';
}

}  // namespace

int main() {
    mmg_example::Graph graph;
    graph.addEdge("source", "parser");
    graph.addEdge("source", "indexer");
    graph.addEdge("parser", "validator");
    graph.addEdge("validator", "graph");
    graph.addEdge("indexer", "graph");
    graph.addEdge("graph", "report");

    printPath("BFS", graph.breadthFirstPath("source", "report"));
    printPath("DFS", graph.depthFirstPath("source", "report"));
}

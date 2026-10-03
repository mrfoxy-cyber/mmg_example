#include "mmg_example/graph.hpp"
#include "mmg_example/reference_graph.hpp"

#include <iostream>
#include <string_view>

namespace mmg_example::demo {

void printNode(const mmg_example::NodeId& node) {
    std::cout << '[' << node << ']';
}

void printNodes(const mmg_example::Path& nodes, const std::string_view separator) {
    for (std::size_t index = 0; index < nodes.size(); ++index) {
        if (index > 0) {
            std::cout << separator;
        }
        printNode(nodes.at(index));
    }
}

void printGraph(const mmg_example::Graph& graph) {
    std::cout << "MMG Reference Graph\n"
              << "===================\n"
              << graph.nodeCount() << " nodes | "
              << graph.edgeCount() << " directed edges\n\n"
              << "Nodes and outgoing edges\n"
              << "------------------------\n";

    for (const auto& node : graph.nodes()) {
        std::cout << "  ";
        printNode(node);
        std::cout << " -> ";

        const auto& neighbors = graph.neighbors(node);
        if (neighbors.empty()) {
            std::cout << "(none)";
        } else {
            printNodes(neighbors, "  ");
        }
        std::cout << '\n';
    }
}

void printIslands(const mmg_example::Islands& islands) {
    std::cout << "\nIslands\n"
              << "-------\n";

    for (std::size_t index = 0; index < islands.size(); ++index) {
        const auto& island = islands.at(index);
        std::cout << "  Island " << index + 1 << " (" << island.size() << " nodes): ";
        printNodes(island, "  ");
        std::cout << '\n';
    }
}

void printSearch(
    const std::string_view label,
    const mmg_example::SearchResult& result) {
    std::cout << "\n" << label << "\n"
              << std::string(label.size(), '-') << "\n"
              << "  Path: ";
    if (!result.path) {
        std::cout << "no path\n";
        return;
    }

    printNodes(*result.path, " -> ");
    std::cout << "\n  Visited: ";
    printNodes(result.visitOrder, "  ");
    std::cout << "\n  Examined edges: " << result.examinedEdges << '\n';
}

int run() {
    const auto graph = makeReferenceGraph();
    const auto islands = graph.islands();

    printGraph(graph);
    printIslands(islands);
    printSearch("Breadth-first search: X to O2", graph.breadthFirstSearch("X", "O2"));
    printSearch("Depth-first search: X to O2", graph.depthFirstSearch("X", "O2"));
    return 0;
}

}  // namespace mmg_example::demo

int main() {
    return mmg_example::demo::run();
}

#include "mmg_example/graph.hpp"

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string_view>

namespace {

int failures = 0;

void expectPath(
    const std::string_view testName,
    const std::optional<mmg_example::Path>& actual,
    const mmg_example::Path& expected) {
    if (actual == expected) {
        std::cout << "[PASS] " << testName << '\n';
        return;
    }

    std::cerr << "[FAIL] " << testName << '\n';
    ++failures;
}

void expectNoPath(
    const std::string_view testName,
    const std::optional<mmg_example::Path>& actual) {
    if (!actual) {
        std::cout << "[PASS] " << testName << '\n';
        return;
    }

    std::cerr << "[FAIL] " << testName << '\n';
    ++failures;
}

mmg_example::Graph makeBranchingGraph() {
    mmg_example::Graph graph;
    graph.addEdge("start", "left");
    graph.addEdge("start", "right");
    graph.addEdge("left", "deep");
    graph.addEdge("deep", "goal");
    graph.addEdge("right", "goal");
    return graph;
}

void breadthFirstSearchFindsShortestPath() {
    const auto graph = makeBranchingGraph();
    expectPath(
        "BFS finds the shortest unweighted path",
        graph.breadthFirstPath("start", "goal"),
        {"start", "right", "goal"});
}

void depthFirstSearchExploresFirstBranch() {
    const auto graph = makeBranchingGraph();
    expectPath(
        "DFS follows the first branch before backtracking",
        graph.depthFirstPath("start", "goal"),
        {"start", "left", "deep", "goal"});
}

void searchesHandleCycles() {
    mmg_example::Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("b", "c");
    graph.addEdge("c", "a");
    graph.addEdge("c", "goal");

    expectPath(
        "BFS terminates when the graph contains a cycle",
        graph.breadthFirstPath("a", "goal"),
        {"a", "b", "c", "goal"});
    expectPath(
        "DFS terminates when the graph contains a cycle",
        graph.depthFirstPath("a", "goal"),
        {"a", "b", "c", "goal"});
}

void searchesReportMissingPaths() {
    mmg_example::Graph graph;
    graph.addEdge("connected", "neighbor");
    graph.addNode("island");

    expectNoPath(
        "BFS reports an unreachable goal",
        graph.breadthFirstPath("connected", "island"));
    expectNoPath(
        "DFS reports an unknown start node",
        graph.depthFirstPath("missing", "island"));
}

void aNodeCanReachItself() {
    mmg_example::Graph graph;
    graph.addNode("node");

    expectPath(
        "BFS returns the start node when start equals goal",
        graph.breadthFirstPath("node", "node"),
        {"node"});
    expectPath(
        "DFS returns the start node when start equals goal",
        graph.depthFirstPath("node", "node"),
        {"node"});
}

}  // namespace

int main() {
    breadthFirstSearchFindsShortestPath();
    depthFirstSearchExploresFirstBranch();
    searchesHandleCycles();
    searchesReportMissingPaths();
    aNodeCanReachItself();

    if (failures == 0) {
        std::cout << "All graph search tests passed.\n";
        return EXIT_SUCCESS;
    }

    std::cerr << failures << " test(s) failed.\n";
    return EXIT_FAILURE;
}

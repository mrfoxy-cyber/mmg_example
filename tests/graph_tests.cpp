#include "mmg_example/graph.hpp"
#include "mmg_example/reference_graph.hpp"

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <optional>
#include <string_view>

namespace mmg_example::tests {

int failures = 0;

void expect(const std::string_view testName, const bool condition) {
    if (condition) {
        std::cout << "[PASS] " << testName << '\n';
        return;
    }

    std::cerr << "[FAIL] " << testName << '\n';
    ++failures;
}

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
    static_cast<void>(graph.addEdge("start", "left"));
    static_cast<void>(graph.addEdge("start", "right"));
    static_cast<void>(graph.addEdge("left", "deep"));
    static_cast<void>(graph.addEdge("deep", "goal"));
    static_cast<void>(graph.addEdge("right", "goal"));
    return graph;
}

bool hasSameNodes(mmg_example::Path actual, mmg_example::Path expected) {
    std::sort(actual.begin(), actual.end());
    std::sort(expected.begin(), expected.end());
    return actual == expected;
}

bool containsIsland(
    const mmg_example::Islands& islands,
    const mmg_example::Path& expected) {
    return std::any_of(
        islands.begin(),
        islands.end(),
        [&expected](const auto& island) {
            return hasSameNodes(island, expected);
        });
}

void referenceGraphProducesThreeIslands() {
    const auto graph = mmg_example::makeReferenceGraph();
    const auto islands = graph.islands();

    expect("Reference graph contains 19 nodes", graph.nodeCount() == 19);
    expect("Reference graph contains 28 edges", graph.edgeCount() == 28);
    expect("Reference graph produces three islands", islands.size() == 3);
    expect(
        "Main island contains the expected 11 nodes",
        containsIsland(
            islands,
            {"X", "C", "R", "O1", "E", "G1", "T", "M", "Z", "Q", "O2"}));
    expect(
        "Left island contains the expected six nodes",
        containsIsland(islands, {"F", "K", "I", "L", "H", "G2"}));
    expect(
        "Pair island contains N and D",
        containsIsland(islands, {"N", "D"}));
    expectPath(
        "BFS traverses the main island from X to O2",
        graph.breadthFirstPath("X", "O2"),
        {"X", "C", "O1", "E", "T", "M", "Z", "O2"});
    expectNoPath(
        "Search does not cross between islands",
        graph.breadthFirstPath("X", "G2"));
}

void graphReportsItsStructure() {
    mmg_example::Graph graph;

    expect("Adding a new node reports success", graph.addNode("standalone"));
    expect("Adding a duplicate node is ignored", !graph.addNode("standalone"));
    expect("Adding a new edge reports success", graph.addEdge("a", "b"));
    expect("Adding a duplicate edge is ignored", !graph.addEdge("a", "b"));
    expect("A directed edge is queryable", graph.hasEdge("a", "b"));
    expect("Directed edges do not imply a reverse edge", !graph.hasEdge("b", "a"));
    expect("The graph reports its node count", graph.nodeCount() == 3);
    expect("The graph reports unique edge count", graph.edgeCount() == 1);
    expect(
        "Neighbor order is stable",
        graph.neighbors("a") == mmg_example::Path{"b"});
}

void undirectedGraphsAddReverseConnections() {
    mmg_example::Graph graph{mmg_example::GraphKind::undirected};
    static_cast<void>(graph.addEdge("north", "south"));

    expect("Undirected graph retains its kind", graph.kind() == mmg_example::GraphKind::undirected);
    expect("Undirected edge has a forward connection", graph.hasEdge("north", "south"));
    expect("Undirected edge has a reverse connection", graph.hasEdge("south", "north"));
    expect("An undirected pair counts as one edge", graph.edgeCount() == 1);
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
    static_cast<void>(graph.addEdge("a", "b"));
    static_cast<void>(graph.addEdge("b", "c"));
    static_cast<void>(graph.addEdge("c", "a"));
    static_cast<void>(graph.addEdge("c", "goal"));

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
    static_cast<void>(graph.addEdge("connected", "neighbor"));
    static_cast<void>(graph.addNode("island"));

    expectNoPath(
        "BFS reports an unreachable goal",
        graph.breadthFirstPath("connected", "island"));
    expectNoPath(
        "DFS reports an unknown start node",
        graph.depthFirstPath("missing", "island"));
}

void aNodeCanReachItself() {
    mmg_example::Graph graph;
    static_cast<void>(graph.addNode("node"));

    expectPath(
        "BFS returns the start node when start equals goal",
        graph.breadthFirstPath("node", "node"),
        {"node"});
    expectPath(
        "DFS returns the start node when start equals goal",
        graph.depthFirstPath("node", "node"),
        {"node"});
}

void searchesExposeHowTheyExplored() {
    const auto graph = makeBranchingGraph();
    const auto breadthFirst = graph.breadthFirstSearch("start", "goal");
    const auto depthFirst = graph.depthFirstSearch("start", "goal");

    expect("BFS reports that it found the goal", breadthFirst.found());
    expect(
        "BFS exposes level-order visitation",
        breadthFirst.visitOrder == mmg_example::Path{"start", "left", "right", "deep", "goal"});
    expect("BFS reports examined edges", breadthFirst.examinedEdges == 5);

    expect("DFS reports that it found the goal", depthFirst.found());
    expect(
        "DFS exposes depth-first visitation",
        depthFirst.visitOrder == mmg_example::Path{"start", "left", "deep", "goal"});
    expect("DFS reports examined edges", depthFirst.examinedEdges == 4);
}

int runAllTests() {
    referenceGraphProducesThreeIslands();
    graphReportsItsStructure();
    undirectedGraphsAddReverseConnections();
    breadthFirstSearchFindsShortestPath();
    depthFirstSearchExploresFirstBranch();
    searchesHandleCycles();
    searchesReportMissingPaths();
    aNodeCanReachItself();
    searchesExposeHowTheyExplored();

    if (failures == 0) {
        std::cout << "All graph search tests passed.\n";
        return EXIT_SUCCESS;
    }

    std::cerr << failures << " test(s) failed.\n";
    return EXIT_FAILURE;
}

}  // namespace mmg_example::tests

int main() {
    return mmg_example::tests::runAllTests();
}

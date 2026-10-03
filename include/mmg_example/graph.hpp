#pragma once

#include <optional>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>



namespace mmg_example {

using NodeId = std::string;
using Path = std::vector<NodeId>;
using Islands = std::vector<Path>;


enum class GraphKind {
    directed,
    undirected,
};

struct SearchResult {
    std::optional<Path> path;
    std::vector<NodeId> visitOrder;
    std::size_t examinedEdges = 0;

    [[nodiscard]] bool found() const noexcept {
        return path.has_value();
    }
};

class Graph {
public:
    explicit Graph(GraphKind kind = GraphKind::directed);

    [[nodiscard]] bool addNode(NodeId node);
    [[nodiscard]] bool addEdge(const NodeId& from, const NodeId& to);

    [[nodiscard]] bool contains(const NodeId& node) const;
    [[nodiscard]] bool hasEdge(const NodeId& from, const NodeId& to) const;
    [[nodiscard]] std::size_t nodeCount() const noexcept;
    [[nodiscard]] std::size_t edgeCount() const noexcept;
    [[nodiscard]] GraphKind kind() const noexcept;
    [[nodiscard]] const std::vector<NodeId>& nodes() const noexcept;
    [[nodiscard]] const std::vector<NodeId>& neighbors(const NodeId& node) const;
    [[nodiscard]] Islands islands() const;

    [[nodiscard]] SearchResult breadthFirstSearch(
        const NodeId& start,
        const NodeId& goal) const;
    [[nodiscard]] SearchResult depthFirstSearch(
        const NodeId& start,
        const NodeId& goal) const;

    [[nodiscard]] std::optional<Path> breadthFirstPath(
        const NodeId& start,
        const NodeId& goal) const;
    [[nodiscard]] std::optional<Path> depthFirstPath(
        const NodeId& start,
        const NodeId& goal) const;

private:
    [[nodiscard]] bool addDirectedEdge(const NodeId& from, const NodeId& to);

    GraphKind kind_;
    std::size_t edgeCount_ = 0;
    std::vector<NodeId> nodeOrder_;
    std::unordered_map<NodeId, std::vector<NodeId>> adjacency_;
};

}  // namespace mmg_example

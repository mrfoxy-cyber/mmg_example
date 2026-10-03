#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace mmg_example {

using NodeId = std::string;
using Path = std::vector<NodeId>;

class Graph {
public:
    void addNode(NodeId node);
    void addEdge(const NodeId& from, const NodeId& to);

    [[nodiscard]] bool contains(const NodeId& node) const;
    [[nodiscard]] std::optional<Path> breadthFirstPath(
        const NodeId& start,
        const NodeId& goal) const;
    [[nodiscard]] std::optional<Path> depthFirstPath(
        const NodeId& start,
        const NodeId& goal) const;

private:
    std::unordered_map<NodeId, std::vector<NodeId>> adjacency_;
};

}  // namespace mmg_example

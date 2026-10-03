#include "mmg_example/graph.hpp"

#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace mmg_example {
namespace {

std::optional<Path> rebuildPath(
    const NodeId& start,
    const NodeId& goal,
    const std::unordered_map<NodeId, NodeId>& parent) {
    Path path;
    for (NodeId current = goal;; current = parent.at(current)) {
        path.push_back(current);
        if (current == start) {
            break;
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}

}  // namespace

void Graph::addNode(NodeId node) {
    adjacency_.try_emplace(std::move(node));
}

void Graph::addEdge(const NodeId& from, const NodeId& to) {
    addNode(from);
    addNode(to);

    auto& neighbors = adjacency_.at(from);
    if (std::find(neighbors.begin(), neighbors.end(), to) == neighbors.end()) {
        neighbors.push_back(to);
    }
}

bool Graph::contains(const NodeId& node) const {
    return adjacency_.contains(node);
}

std::optional<Path> Graph::breadthFirstPath(
    const NodeId& start,
    const NodeId& goal) const {
    if (!contains(start) || !contains(goal)) {
        return std::nullopt;
    }

    std::queue<NodeId> frontier;
    std::unordered_set<NodeId> visited{start};
    std::unordered_map<NodeId, NodeId> parent;
    frontier.push(start);

    while (!frontier.empty()) {
        const NodeId current = std::move(frontier.front());
        frontier.pop();

        if (current == goal) {
            return rebuildPath(start, goal, parent);
        }

        for (const auto& neighbor : adjacency_.at(current)) {
            if (visited.insert(neighbor).second) {
                parent.emplace(neighbor, current);
                frontier.push(neighbor);
            }
        }
    }

    return std::nullopt;
}

std::optional<Path> Graph::depthFirstPath(
    const NodeId& start,
    const NodeId& goal) const {
    if (!contains(start) || !contains(goal)) {
        return std::nullopt;
    }

    std::stack<NodeId> frontier;
    std::unordered_set<NodeId> visited{start};
    std::unordered_map<NodeId, NodeId> parent;
    frontier.push(start);

    while (!frontier.empty()) {
        NodeId current = std::move(frontier.top());
        frontier.pop();

        if (current == goal) {
            return rebuildPath(start, goal, parent);
        }

        const auto& neighbors = adjacency_.at(current);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            if (visited.insert(*it).second) {
                parent.emplace(*it, current);
                frontier.push(*it);
            }
        }
    }

    return std::nullopt;
}

}  // namespace mmg_example

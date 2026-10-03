#include "mmg_example/graph.hpp"

#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace mmg_example {
namespace detail {

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

}  // namespace detail

Graph::Graph(const GraphKind kind) : kind_(kind) {}

bool Graph::addNode(NodeId node) {
    if (contains(node)) {
        return false;
    }

    nodeOrder_.push_back(node);
    adjacency_.try_emplace(std::move(node));
    return true;
}

bool Graph::addDirectedEdge(const NodeId& from, const NodeId& to) {
    auto& neighbors = adjacency_.at(from);
    if (std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end()) {
        return false;
    }

    neighbors.push_back(to);
    return true;
}

bool Graph::addEdge(const NodeId& from, const NodeId& to) {
    static_cast<void>(addNode(from));
    static_cast<void>(addNode(to));

    if (!addDirectedEdge(from, to)) {
        return false;
    }

    if (kind_ == GraphKind::undirected && from != to) {
        static_cast<void>(addDirectedEdge(to, from));
    }

    ++edgeCount_;
    return true;
}

bool Graph::contains(const NodeId& node) const {
    return adjacency_.contains(node);
}

bool Graph::hasEdge(const NodeId& from, const NodeId& to) const {
    const auto found = adjacency_.find(from);
    if (found == adjacency_.end()) {
        return false;
    }

    const auto& neighbors = found->second;
    return std::find(neighbors.begin(), neighbors.end(), to) != neighbors.end();
}

std::size_t Graph::nodeCount() const noexcept {
    return adjacency_.size();
}

std::size_t Graph::edgeCount() const noexcept {
    return edgeCount_;
}

GraphKind Graph::kind() const noexcept {
    return kind_;
}

const std::vector<NodeId>& Graph::nodes() const noexcept {
    return nodeOrder_;
}

const std::vector<NodeId>& Graph::neighbors(const NodeId& node) const {
    return adjacency_.at(node);
}

Islands Graph::islands() const {
    auto connected = adjacency_;

    // An island is a weakly connected component: edge direction does not split
    // nodes that still belong to the same piece of the graph.
    for (const auto& from : nodeOrder_) {
        for (const auto& to : adjacency_.at(from)) {
            auto& reverseNeighbors = connected.at(to);
            if (std::find(reverseNeighbors.begin(), reverseNeighbors.end(), from) ==
                reverseNeighbors.end()) {
                reverseNeighbors.push_back(from);
            }
        }
    }

    Islands result;
    std::unordered_set<NodeId> visited;

    for (const auto& first : nodeOrder_) {
        if (!visited.insert(first).second) {
            continue;
        }

        Path island;
        std::queue<NodeId> frontier;
        frontier.push(first);

        while (!frontier.empty()) {
            NodeId current = std::move(frontier.front());
            frontier.pop();
            island.push_back(current);

            for (const auto& neighbor : connected.at(current)) {
                if (visited.insert(neighbor).second) {
                    frontier.push(neighbor);
                }
            }
        }

        result.push_back(std::move(island));
    }

    return result;
}

SearchResult Graph::breadthFirstSearch(
    const NodeId& start,
    const NodeId& goal) const {
    SearchResult result;
    if (!contains(start) || !contains(goal)) {
        return result;
    }

    std::queue<NodeId> frontier;
    std::unordered_set<NodeId> visited{start};
    std::unordered_map<NodeId, NodeId> parent;
    frontier.push(start);

    while (!frontier.empty()) {
        const NodeId current = std::move(frontier.front());
        frontier.pop();
        result.visitOrder.push_back(current);

        if (current == goal) {
            result.path = detail::rebuildPath(start, goal, parent);
            return result;
        }

        for (const auto& neighbor : adjacency_.at(current)) {
            ++result.examinedEdges;
            if (visited.insert(neighbor).second) {
                parent.emplace(neighbor, current);
                frontier.push(neighbor);
            }
        }
    }

    return result;
}

SearchResult Graph::depthFirstSearch(
    const NodeId& start,
    const NodeId& goal) const {
    SearchResult result;
    if (!contains(start) || !contains(goal)) {
        return result;
    }

    std::stack<NodeId> frontier;
    std::unordered_set<NodeId> visited{start};
    std::unordered_map<NodeId, NodeId> parent;
    frontier.push(start);

    while (!frontier.empty()) {
        NodeId current = std::move(frontier.top());
        frontier.pop();
        result.visitOrder.push_back(current);

        if (current == goal) {
            result.path = detail::rebuildPath(start, goal, parent);
            return result;
        }

        const auto& neighbors = adjacency_.at(current);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            ++result.examinedEdges;
            if (visited.insert(*it).second) {
                parent.emplace(*it, current);
                frontier.push(*it);
            }
        }
    }

    return result;
}

std::optional<Path> Graph::breadthFirstPath(
    const NodeId& start,
    const NodeId& goal) const {
    return breadthFirstSearch(start, goal).path;
}

std::optional<Path> Graph::depthFirstPath(
    const NodeId& start,
    const NodeId& goal) const {
    return depthFirstSearch(start, goal).path;
}

}  // namespace mmg_example

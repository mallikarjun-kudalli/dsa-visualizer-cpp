#include "graph_algorithms.h"

#include <algorithm>
#include <limits>
#include <queue>
#include <stack>
#include <utility>

GraphData createGridGraph(int rows, int cols, float spacing, const sf::Vector2f& origin) {
    GraphData graph;
    graph.rows = rows;
    graph.cols = cols;
    const int totalNodes = rows * cols;
    graph.adjacency.resize(totalNodes);
    graph.positions.resize(totalNodes);

    auto indexOf = [cols](int r, int c) { return r * cols + c; };
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const int idx = indexOf(r, c);
            graph.positions[idx] = sf::Vector2f(origin.x + c * spacing, origin.y + r * spacing);

            if (r > 0) graph.adjacency[idx].push_back(indexOf(r - 1, c));
            if (c + 1 < cols) graph.adjacency[idx].push_back(indexOf(r, c + 1));
            if (r + 1 < rows) graph.adjacency[idx].push_back(indexOf(r + 1, c));
            if (c > 0) graph.adjacency[idx].push_back(indexOf(r, c - 1));
        }
    }
    return graph;
}

GraphOps bfsOps(const GraphData& graph, int startNode) {
    GraphOps ops;
    if (graph.adjacency.empty() || startNode < 0 || startNode >= static_cast<int>(graph.adjacency.size())) {
        return ops;
    }

    std::vector<bool> visited(graph.adjacency.size(), false);
    std::queue<int> q;
    visited[startNode] = true;
    q.push(startNode);
    ops.push_back({GraphOpType::Visit, startNode, -1, 0.0f});

    while (!q.empty()) {
        const int node = q.front();
        q.pop();
        for (int neighbor : graph.adjacency[node]) {
            ops.push_back({GraphOpType::EdgeTraverse, neighbor, node, 0.0f});
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                ops.push_back({GraphOpType::Visit, neighbor, node, 0.0f});
            }
        }
    }
    return ops;
}

GraphOps dfsOps(const GraphData& graph, int startNode) {
    GraphOps ops;
    if (graph.adjacency.empty() || startNode < 0 || startNode >= static_cast<int>(graph.adjacency.size())) {
        return ops;
    }

    std::vector<bool> visited(graph.adjacency.size(), false);
    std::stack<int> st;
    st.push(startNode);

    while (!st.empty()) {
        const int node = st.top();
        st.pop();
        if (visited[node]) {
            continue;
        }
        visited[node] = true;
        ops.push_back({GraphOpType::Visit, node, -1, 0.0f});

        // Push neighbors in reverse to keep traversal visually stable.
        for (auto it = graph.adjacency[node].rbegin(); it != graph.adjacency[node].rend(); ++it) {
            const int neighbor = *it;
            ops.push_back({GraphOpType::EdgeTraverse, neighbor, node, 0.0f});
            if (!visited[neighbor]) {
                st.push(neighbor);
            }
        }
    }
    return ops;
}

GraphOps dijkstraOps(const GraphData& graph, int startNode) {
    GraphOps ops;
    if (graph.adjacency.empty() || startNode < 0 || startNode >= static_cast<int>(graph.adjacency.size())) {
        return ops;
    }

    const float inf = std::numeric_limits<float>::infinity();
    std::vector<float> dist(graph.adjacency.size(), inf);
    std::vector<bool> done(graph.adjacency.size(), false);
    dist[startNode] = 0.0f;

    using NodeDist = std::pair<float, int>;
    std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;
    pq.push({0.0f, startNode});

    while (!pq.empty()) {
        const auto [currentDist, node] = pq.top();
        pq.pop();
        if (done[node]) {
            continue;
        }
        done[node] = true;
        ops.push_back({GraphOpType::Visit, node, -1, currentDist});
        ops.push_back({GraphOpType::SetDistance, node, -1, currentDist});

        for (int neighbor : graph.adjacency[node]) {
            ops.push_back({GraphOpType::EdgeTraverse, neighbor, node, 0.0f});
            const float newDist = currentDist + 1.0f;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                pq.push({newDist, neighbor});
                ops.push_back({GraphOpType::SetDistance, neighbor, node, newDist});
            }
        }
    }
    return ops;
}

std::string graphComplexity(const std::string& algorithm) {
    if (algorithm == "BFS") {
        return "Time: O(V + E), Space: O(V)";
    }
    if (algorithm == "DFS") {
        return "Time: O(V + E), Space: O(V)";
    }
    if (algorithm == "Dijkstra") {
        return "Time: O((V + E) log V), Space: O(V)";
    }
    return "Time: -, Space: -";
}

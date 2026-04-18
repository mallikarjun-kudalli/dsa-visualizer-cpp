#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

struct GraphData {
    int rows = 0;
    int cols = 0;
    std::vector<std::vector<int>> adjacency;
    std::vector<sf::Vector2f> positions;
};

enum class GraphOpType {
    Visit,
    EdgeTraverse,
    SetDistance
};

struct GraphOperation {
    GraphOpType type;
    int node = -1;
    int from = -1;
    float distance = 0.0f;
};

using GraphOps = std::vector<GraphOperation>;

GraphData createGridGraph(int rows, int cols, float spacing, const sf::Vector2f& origin);
GraphOps bfsOps(const GraphData& graph, int startNode);
GraphOps dfsOps(const GraphData& graph, int startNode);
GraphOps dijkstraOps(const GraphData& graph, int startNode);
std::string graphComplexity(const std::string& algorithm);

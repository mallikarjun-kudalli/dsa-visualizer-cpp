#include "GraphVisualizer.h"

#include <cmath>
#include <limits>
#include <sstream>

#include "../utils/constants.h"

GraphVisualizer::GraphVisualizer() = default;

void GraphVisualizer::setAlgorithm(const std::string& algorithmName) {
    if (algorithm_ == algorithmName) {
        return;
    }
    algorithm_ = algorithmName;
    reset();
}

void GraphVisualizer::setSpeed(float speed) {
    speed_ = speed;
}

void GraphVisualizer::ensureLayout(const sf::FloatRect& area) {
    rebuildGraphForArea(area);
}

void GraphVisualizer::start() {
    buildOperationsIfNeeded();
    running_ = true;
}

void GraphVisualizer::pause() {
    running_ = false;
}

void GraphVisualizer::resume() {
    if (!finished_) {
        buildOperationsIfNeeded();
        running_ = true;
    }
}

void GraphVisualizer::reset() {
    operations_.clear();
    opIndex_ = 0;
    running_ = false;
    finished_ = false;
    operationBudget_ = 0.0f;
    visitedCount_ = 0;
    activeNode_ = activeFrom_ = activeTo_ = -1;
    visitedOrder_.assign(graph_.adjacency.size(), -1);
    distance_.assign(graph_.adjacency.size(), std::numeric_limits<float>::infinity());
}

void GraphVisualizer::update(float deltaSeconds) {
    if (!running_ || finished_) {
        return;
    }

    operationBudget_ += deltaSeconds * speed_;
    while (operationBudget_ >= 1.0f && !finished_) {
        applyNextOperation();
        operationBudget_ -= 1.0f;
    }
}

void GraphVisualizer::draw(sf::RenderTarget& target, const sf::FloatRect& area, const sf::Font& font) const {
    if (graph_.adjacency.empty()) {
        return;
    }

    sf::RectangleShape board;
    board.setPosition(area.left, area.top);
    board.setSize({area.width, area.height});
    board.setFillColor(sf::Color(23, 24, 34));
    board.setOutlineThickness(1.0f);
    board.setOutlineColor(constants::BORDER_COLOR);
    target.draw(board);

    for (std::size_t i = 0; i < graph_.adjacency.size(); ++i) {
        for (int neighbor : graph_.adjacency[i]) {
            if (static_cast<int>(i) > neighbor) {
                continue;
            }
            sf::Vertex line[2];
            line[0].position = graph_.positions[i];
            line[1].position = graph_.positions[neighbor];
            const bool active = (activeFrom_ == static_cast<int>(i) && activeTo_ == neighbor) ||
                                (activeFrom_ == neighbor && activeTo_ == static_cast<int>(i));
            const sf::Color color = active ? constants::EDGE_ACTIVE : constants::EDGE_DEFAULT;
            line[0].color = color;
            line[1].color = color;
            target.draw(line, 2, sf::Lines);
        }
    }

    for (std::size_t i = 0; i < graph_.positions.size(); ++i) {
        sf::CircleShape node(13.0f);
        node.setOrigin(13.0f, 13.0f);
        node.setPosition(graph_.positions[i]);

        sf::Color color = constants::NODE_DEFAULT;
        if (visitedOrder_[i] >= 0) {
            color = constants::NODE_VISITED;
        }
        if (activeNode_ == static_cast<int>(i)) {
            color = constants::NODE_ACTIVE;
        }
        node.setFillColor(color);
        node.setOutlineThickness(1.0f);
        node.setOutlineColor(sf::Color(22, 22, 30));
        target.draw(node);

        if (visitedOrder_[i] >= 0) {
            sf::Text order;
            order.setFont(font);
            order.setCharacterSize(11);
            order.setFillColor(sf::Color(12, 12, 16));
            order.setString(std::to_string(visitedOrder_[i]));
            const sf::FloatRect b = order.getLocalBounds();
            order.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            order.setPosition(graph_.positions[i].x, graph_.positions[i].y - 1.0f);
            target.draw(order);
        }
    }
}

bool GraphVisualizer::isRunning() const {
    return running_;
}

bool GraphVisualizer::isFinished() const {
    return finished_;
}

std::size_t GraphVisualizer::visitedCount() const {
    return visitedCount_;
}

std::size_t GraphVisualizer::stepCount() const {
    return opIndex_;
}

std::string GraphVisualizer::currentAlgorithm() const {
    return algorithm_;
}

void GraphVisualizer::rebuildGraphForArea(const sf::FloatRect& area) {
    const bool changed = std::fabs(area.left - lastArea_.left) > 0.1f || std::fabs(area.top - lastArea_.top) > 0.1f ||
                         std::fabs(area.width - lastArea_.width) > 0.1f || std::fabs(area.height - lastArea_.height) > 0.1f;
    if (!changed && !graph_.adjacency.empty()) {
        return;
    }
    lastArea_ = area;

    constexpr int rows = 7;
    constexpr int cols = 11;
    const float hSpacing = area.width / static_cast<float>(cols + 1);
    const float vSpacing = area.height / static_cast<float>(rows + 1);
    const float spacing = std::min(hSpacing, vSpacing);
    const sf::Vector2f origin(area.left + (area.width - (cols - 1) * spacing) / 2.0f,
                              area.top + (area.height - (rows - 1) * spacing) / 2.0f);

    graph_ = createGridGraph(rows, cols, spacing, origin);
    visitedOrder_.assign(graph_.adjacency.size(), -1);
    distance_.assign(graph_.adjacency.size(), std::numeric_limits<float>::infinity());
    reset();
}

void GraphVisualizer::buildOperationsIfNeeded() {
    if (!operations_.empty() || graph_.adjacency.empty()) {
        return;
    }
    const int startNode = 0;
    if (algorithm_ == "BFS") {
        operations_ = bfsOps(graph_, startNode);
    } else if (algorithm_ == "DFS") {
        operations_ = dfsOps(graph_, startNode);
    } else {
        operations_ = dijkstraOps(graph_, startNode);
    }
}

void GraphVisualizer::applyNextOperation() {
    activeNode_ = -1;
    if (opIndex_ >= operations_.size()) {
        running_ = false;
        finished_ = true;
        activeFrom_ = activeTo_ = -1;
        return;
    }

    const GraphOperation& op = operations_[opIndex_++];
    if (op.type == GraphOpType::Visit) {
        activeNode_ = op.node;
        if (op.node >= 0 && op.node < static_cast<int>(visitedOrder_.size()) && visitedOrder_[op.node] < 0) {
            visitedOrder_[op.node] = static_cast<int>(visitedCount_) + 1;
            ++visitedCount_;
        }
    } else if (op.type == GraphOpType::EdgeTraverse) {
        activeFrom_ = op.from;
        activeTo_ = op.node;
    } else if (op.type == GraphOpType::SetDistance) {
        if (op.node >= 0 && op.node < static_cast<int>(distance_.size())) {
            distance_[op.node] = op.distance;
        }
    }
}

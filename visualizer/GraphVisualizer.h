#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../algorithms/graph_algorithms.h"

class GraphVisualizer {
public:
    GraphVisualizer();

    void setAlgorithm(const std::string& algorithmName);
    void setSpeed(float speed);
    void ensureLayout(const sf::FloatRect& area);
    void start();
    void pause();
    void resume();
    void reset();

    void update(float deltaSeconds);
    void draw(sf::RenderTarget& target, const sf::FloatRect& area, const sf::Font& font) const;

    bool isRunning() const;
    bool isFinished() const;
    std::size_t visitedCount() const;
    std::size_t stepCount() const;
    std::string currentAlgorithm() const;

private:
    void rebuildGraphForArea(const sf::FloatRect& area);
    void buildOperationsIfNeeded();
    void applyNextOperation();

    GraphData graph_;
    GraphOps operations_;
    std::vector<int> visitedOrder_;
    std::vector<float> distance_;

    std::string algorithm_ = "BFS";
    float speed_ = 100.0f;
    float operationBudget_ = 0.0f;
    std::size_t opIndex_ = 0;
    bool running_ = false;
    bool finished_ = false;
    std::size_t visitedCount_ = 0;

    int activeNode_ = -1;
    int activeFrom_ = -1;
    int activeTo_ = -1;
    sf::FloatRect lastArea_;
};

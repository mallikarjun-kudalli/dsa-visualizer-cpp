#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../algorithms/sorting_algorithms.h"

class SortingVisualizer {
public:
    SortingVisualizer();

    void setAlgorithm(const std::string& algorithmName);
    void resetRandom(std::size_t size);
    void setSpeed(float speed);

    void start();
    void pause();
    void resume();
    void reset();

    void update(float deltaSeconds);
    void draw(sf::RenderTarget& target, const sf::FloatRect& area) const;

    bool isRunning() const;
    bool isFinished() const;
    std::size_t comparisonCount() const;
    std::size_t swapCount() const;
    std::size_t stepCount() const;
    std::string currentAlgorithm() const;

private:
    void buildOperationsIfNeeded();
    void applyNextOperation();

    std::vector<int> data_;
    std::vector<int> originalData_;
    std::vector<bool> sortedFlags_;
    SortOps operations_;

    std::string algorithm_ = "Bubble Sort";
    float speed_ = 100.0f;
    float operationBudget_ = 0.0f;
    std::size_t opIndex_ = 0;
    bool running_ = false;
    bool finished_ = false;

    std::size_t compareA_ = static_cast<std::size_t>(-1);
    std::size_t compareB_ = static_cast<std::size_t>(-1);
    std::size_t swapA_ = static_cast<std::size_t>(-1);
    std::size_t swapB_ = static_cast<std::size_t>(-1);

    std::size_t comparisons_ = 0;
    std::size_t swaps_ = 0;
};

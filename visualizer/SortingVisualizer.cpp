#include "SortingVisualizer.h"

#include <algorithm>

SortingVisualizer::SortingVisualizer() {
    resetRandom(95);
}

void SortingVisualizer::setAlgorithm(const std::string& algorithmName) {
    if (algorithm_ == algorithmName) {
        return;
    }
    algorithm_ = algorithmName;
    reset();
}

void SortingVisualizer::resetRandom(std::size_t size) {
    originalData_ = generateRandomArray(size);
    reset();
}

void SortingVisualizer::setSpeed(float speed) {
    speed_ = speed;
}

void SortingVisualizer::start() {
    buildOperationsIfNeeded();
    running_ = true;
}

void SortingVisualizer::pause() {
    running_ = false;
}

void SortingVisualizer::resume() {
    if (!finished_) {
        buildOperationsIfNeeded();
        running_ = true;
    }
}

void SortingVisualizer::reset() {
    data_ = originalData_;
    sortedFlags_.assign(data_.size(), false);
    operations_.clear();
    opIndex_ = 0;
    operationBudget_ = 0.0f;
    running_ = false;
    finished_ = false;
    compareA_ = compareB_ = swapA_ = swapB_ = static_cast<std::size_t>(-1);
    comparisons_ = 0;
    swaps_ = 0;
}

void SortingVisualizer::update(float deltaSeconds) {
    if (!running_ || finished_) {
        return;
    }

    operationBudget_ += deltaSeconds * speed_;
    while (operationBudget_ >= 1.0f && !finished_) {
        applyNextOperation();
        operationBudget_ -= 1.0f;
    }
}

void SortingVisualizer::draw(sf::RenderTarget& target, const sf::FloatRect& area) const {
    if (data_.empty()) {
        return;
    }
    const int maxValue = *std::max_element(data_.begin(), data_.end());
    if (maxValue <= 0) {
        return;
    }

    const float usableWidth = area.width;
    const float usableHeight = area.height;
    const float barWidth = std::max(2.0f, usableWidth / static_cast<float>(data_.size()));

    for (std::size_t i = 0; i < data_.size(); ++i) {
        const float height = (static_cast<float>(data_[i]) / static_cast<float>(maxValue)) * (usableHeight - 20.0f);
        sf::RectangleShape bar;
        bar.setSize({barWidth - 1.0f, -height});
        bar.setPosition(area.left + i * barWidth, area.top + usableHeight);

        sf::Color color(245, 245, 245);
        if (sortedFlags_[i]) {
            color = sf::Color(104, 207, 129);
        } else if (i == swapA_ || i == swapB_) {
            color = sf::Color(233, 88, 74);
        } else if (i == compareA_ || i == compareB_) {
            color = sf::Color(243, 208, 84);
        }
        bar.setFillColor(color);
        target.draw(bar);
    }
}

bool SortingVisualizer::isRunning() const {
    return running_;
}

bool SortingVisualizer::isFinished() const {
    return finished_;
}

std::size_t SortingVisualizer::comparisonCount() const {
    return comparisons_;
}

std::size_t SortingVisualizer::swapCount() const {
    return swaps_;
}

std::size_t SortingVisualizer::stepCount() const {
    return opIndex_;
}

std::string SortingVisualizer::currentAlgorithm() const {
    return algorithm_;
}

void SortingVisualizer::buildOperationsIfNeeded() {
    if (!operations_.empty() || data_.empty()) {
        return;
    }
    if (algorithm_ == "Bubble Sort") {
        operations_ = bubbleSortOps(data_);
    } else if (algorithm_ == "Selection Sort") {
        operations_ = selectionSortOps(data_);
    } else if (algorithm_ == "Insertion Sort") {
        operations_ = insertionSortOps(data_);
    } else if (algorithm_ == "Merge Sort") {
        operations_ = mergeSortOps(data_);
    } else {
        operations_ = quickSortOps(data_);
    }
}

void SortingVisualizer::applyNextOperation() {
    compareA_ = compareB_ = swapA_ = swapB_ = static_cast<std::size_t>(-1);
    if (opIndex_ >= operations_.size()) {
        running_ = false;
        finished_ = true;
        std::fill(sortedFlags_.begin(), sortedFlags_.end(), true);
        return;
    }

    const SortOperation& op = operations_[opIndex_++];
    switch (op.type) {
        case SortOpType::Compare:
            compareA_ = op.i;
            compareB_ = op.j;
            ++comparisons_;
            break;
        case SortOpType::Swap:
            std::swap(data_[op.i], data_[op.j]);
            swapA_ = op.i;
            swapB_ = op.j;
            ++swaps_;
            break;
        case SortOpType::Overwrite:
            data_[op.i] = op.value;
            swapA_ = op.i;
            break;
        case SortOpType::MarkSorted:
            if (op.i < sortedFlags_.size()) {
                sortedFlags_[op.i] = true;
            }
            break;
    }
}

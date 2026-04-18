#pragma once

#include <string>
#include <vector>

enum class SortOpType {
    Compare,
    Swap,
    Overwrite,
    MarkSorted
};

struct SortOperation {
    SortOpType type;
    std::size_t i = 0;
    std::size_t j = 0;
    int value = 0;
};

using SortOps = std::vector<SortOperation>;

std::vector<int> generateRandomArray(std::size_t size, int minValue = 10, int maxValue = 450);

SortOps bubbleSortOps(const std::vector<int>& input);
SortOps selectionSortOps(const std::vector<int>& input);
SortOps insertionSortOps(const std::vector<int>& input);
SortOps mergeSortOps(const std::vector<int>& input);
SortOps quickSortOps(const std::vector<int>& input);

std::string sortingComplexity(const std::string& algorithm);

#include "sorting_algorithms.h"

#include <algorithm>
#include <functional>
#include <random>

std::vector<int> generateRandomArray(std::size_t size, int minValue, int maxValue) {
    std::vector<int> data(size);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(minValue, maxValue);
    for (int& value : data) {
        value = dist(rng);
    }
    return data;
}

SortOps bubbleSortOps(const std::vector<int>& input) {
    std::vector<int> arr = input;
    SortOps ops;
    const std::size_t n = arr.size();
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j + 1 < n - i; ++j) {
            ops.push_back({SortOpType::Compare, j, j + 1, 0});
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                ops.push_back({SortOpType::Swap, j, j + 1, 0});
            }
        }
        ops.push_back({SortOpType::MarkSorted, n - i - 1, 0, 0});
    }
    return ops;
}

SortOps selectionSortOps(const std::vector<int>& input) {
    std::vector<int> arr = input;
    SortOps ops;
    const std::size_t n = arr.size();
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t minIndex = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            ops.push_back({SortOpType::Compare, minIndex, j, 0});
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
            ops.push_back({SortOpType::Swap, i, minIndex, 0});
        }
        ops.push_back({SortOpType::MarkSorted, i, 0, 0});
    }
    return ops;
}

SortOps insertionSortOps(const std::vector<int>& input) {
    std::vector<int> arr = input;
    SortOps ops;
    const std::size_t n = arr.size();
    if (n > 0) {
        ops.push_back({SortOpType::MarkSorted, 0, 0, 0});
    }
    for (std::size_t i = 1; i < n; ++i) {
        std::size_t j = i;
        while (j > 0) {
            ops.push_back({SortOpType::Compare, j - 1, j, 0});
            if (arr[j - 1] <= arr[j]) {
                break;
            }
            std::swap(arr[j - 1], arr[j]);
            ops.push_back({SortOpType::Swap, j - 1, j, 0});
            --j;
        }
    }
    for (std::size_t i = 0; i < n; ++i) {
        ops.push_back({SortOpType::MarkSorted, i, 0, 0});
    }
    return ops;
}

SortOps mergeSortOps(const std::vector<int>& input) {
    std::vector<int> arr = input;
    SortOps ops;

    std::function<void(int, int)> mergeSort = [&](int left, int right) {
        if (left >= right) {
            return;
        }
        const int mid = left + (right - left) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);

        std::vector<int> temp;
        temp.reserve(static_cast<std::size_t>(right - left + 1));

        int i = left;
        int j = mid + 1;
        while (i <= mid && j <= right) {
            ops.push_back({SortOpType::Compare, static_cast<std::size_t>(i), static_cast<std::size_t>(j), 0});
            if (arr[i] <= arr[j]) {
                temp.push_back(arr[i++]);
            } else {
                temp.push_back(arr[j++]);
            }
        }
        while (i <= mid) {
            temp.push_back(arr[i++]);
        }
        while (j <= right) {
            temp.push_back(arr[j++]);
        }

        for (int k = left; k <= right; ++k) {
            arr[k] = temp[static_cast<std::size_t>(k - left)];
            ops.push_back({SortOpType::Overwrite, static_cast<std::size_t>(k), 0, arr[k]});
        }
    };

    if (!arr.empty()) {
        mergeSort(0, static_cast<int>(arr.size()) - 1);
        for (std::size_t i = 0; i < arr.size(); ++i) {
            ops.push_back({SortOpType::MarkSorted, i, 0, 0});
        }
    }
    return ops;
}

SortOps quickSortOps(const std::vector<int>& input) {
    std::vector<int> arr = input;
    SortOps ops;

    std::function<void(int, int)> quickSort = [&](int low, int high) {
        if (low > high) {
            return;
        }
        if (low == high) {
            ops.push_back({SortOpType::MarkSorted, static_cast<std::size_t>(low), 0, 0});
            return;
        }

        const int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            ops.push_back({SortOpType::Compare, static_cast<std::size_t>(j), static_cast<std::size_t>(high), 0});
            if (arr[j] < pivot) {
                ++i;
                if (i != j) {
                    std::swap(arr[i], arr[j]);
                    ops.push_back({SortOpType::Swap, static_cast<std::size_t>(i), static_cast<std::size_t>(j), 0});
                }
            }
        }
        if (i + 1 != high) {
            std::swap(arr[i + 1], arr[high]);
            ops.push_back({SortOpType::Swap, static_cast<std::size_t>(i + 1), static_cast<std::size_t>(high), 0});
        }

        const int pivotIndex = i + 1;
        ops.push_back({SortOpType::MarkSorted, static_cast<std::size_t>(pivotIndex), 0, 0});

        quickSort(low, pivotIndex - 1);
        quickSort(pivotIndex + 1, high);
    };

    if (!arr.empty()) {
        quickSort(0, static_cast<int>(arr.size()) - 1);
        for (std::size_t i = 0; i < arr.size(); ++i) {
            ops.push_back({SortOpType::MarkSorted, i, 0, 0});
        }
    }
    return ops;
}

std::string sortingComplexity(const std::string& algorithm) {
    if (algorithm == "Bubble Sort") {
        return "Time: O(n^2), Space: O(1)";
    }
    if (algorithm == "Selection Sort") {
        return "Time: O(n^2), Space: O(1)";
    }
    if (algorithm == "Insertion Sort") {
        return "Time: O(n^2), Space: O(1)";
    }
    if (algorithm == "Merge Sort") {
        return "Time: O(n log n), Space: O(n)";
    }
    if (algorithm == "Quick Sort") {
        return "Time: O(n log n) avg, O(n^2) worst, Space: O(log n)";
    }
    return "Time: -, Space: -";
}

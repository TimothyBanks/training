//
//  quick_sort.hpp
//  robinhood
//
//  Created by Timothy Banks on 4/27/21.
//

#ifndef quick_sort_h
#define quick_sort_h

#include <vector>

namespace quick_sort {

namespace details {
template <typename T>
size_t partition(std::vector<T>& v, size_t begin, size_t end) {
    // We can choose any value in the array to be the partition.
    // Choosing the "right" partition allows for this algorithm to execute in its best case (N LOG N) runtime.
    // Choosing the "wrong" partition makes this algorithm run in its worse case (N^2) runtime.
    // For now just choose the value at the end.
    auto partition_value = v[end - 1];
    auto start = begin;
    for (size_t i = begin; i < end - 1; ++i) {
        if (v[i] < partition_value) {
            std::swap(v[i], v[start++]);
        }
    }
    // Move the partition value into its final ordered position.
    std::swap(v[start], v[end - 1]);
    return begin;
}

template <typename T>
void sort(std::vector<T>& v, size_t begin, size_t end) {
    if (begin == end) {
        return;
    }
    auto partition_index = partition(v, begin, end);
    sort(v, 0, partition_index);
    sort(v, partition_index + 1, end);
}
}

template <typename T>
void sort(std::vector<T>& v) {
    details::sort(v, 0, v.size());
}

void run_test() {
    auto values = std::vector<int16_t>{8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
    sort(values);
}
    
}

#endif /* quick_sort_h */

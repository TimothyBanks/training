//
//  merge_sort.hpp
//  robinhood
//
//  Created by Timothy Banks on 4/27/21.
//

#ifndef merge_sort_h
#define merge_sort_h

#include <vector>

namespace merge_sort {
    
template <typename T>
void sort(std::vector<T>& v) {
    if (v.size() <= 1) {
        return;
    }
    
    auto mid = v.size() / 2;
    auto left = std::vector<T>{std::begin(v), std::begin(v) + mid};
    auto right = std::vector<T>{std::begin(v) + mid, std::end(v)};
    
    sort(left);
    sort(right);
    
    auto lindex = size_t{0};
    auto rindex = size_t{0};
    auto index = size_t{0};
    while (lindex < left.size() && rindex < right.size()) {
        v[index++] = left[lindex] < right[rindex] ? left[lindex++] : right[rindex++];
    }
    while (lindex < left.size()) {
        v[index++] = left[lindex++];
    }
    while (rindex < right.size()) {
        v[index++] = right[rindex++];
    }
}

void run_test() {
    auto values = std::vector<int16_t>{8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
    sort(values);
}
    
}

#endif /* merge_sort_h */

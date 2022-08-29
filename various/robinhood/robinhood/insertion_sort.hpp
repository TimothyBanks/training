//
//  insertion_sort.hpp
//  robinhood
//
//  Created by Timothy Banks on 4/27/21.
//

#ifndef insertion_sort_h
#define insertion_sort_h

#include <vector>

namespace insertion_sort {

template <typename T>
void sort(std::vector<T>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        for (int64_t j = i; j >= 0 && v[j-1] > v[j]; --j) {
            std::swap(v[j-1], v[j]);
        }
    }
}

void run_test() {
    auto values = std::vector<int16_t>{8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
    sort(values);
}

}

#endif /* insertion_sort_h */

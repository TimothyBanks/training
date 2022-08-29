//
//  selection_sort.hpp
//  robinhood
//
//  Created by Timothy Banks on 4/27/21.
//

#ifndef selection_sort_h
#define selection_sort_h

#include <vector>

namespace selection_sort {
    
template <typename T>
void sort(std::vector<T>& v) {
    for (size_t i = 0; i < v.size() - 1; ++i) {
        auto min_index = i;
        for (size_t j = i + 1; j < v.size(); ++j) {
            if (v[j] < v[min_index]) {
                min_index = j;
            }
        }
        std::swap(v[i], v[min_index]);
    }
}

void run_test() {
    auto values = std::vector<int16_t>{8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
    sort(values);
}
    
}

#endif /* selection_sort_h */

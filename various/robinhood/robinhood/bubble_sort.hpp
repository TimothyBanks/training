//
//  bubble_sort.hpp
//  robinhood
//
//  Created by Timothy Banks on 4/27/21.
//

#ifndef bubble_sort_h
#define bubble_sort_h

#include <vector>

namespace bubble_sort {
    
template <typename T>
void sort(std::vector<T>& v) {
    auto sorted = false;
    while (!sorted) {
        sorted = true;
        for (size_t i = 1; i < v.size(); ++i) {
            if (v[i-1] > v[i]) {
                sorted = false;
                std::swap(v[i-1], v[i]);
            }
        }
    }
}

void run_test() {
    auto values = std::vector<int16_t>{8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
    sort(values);
}
    
}

#endif /* bubble_sort_h */

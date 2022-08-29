//
//  array.hpp
//  robinhood
//
//  Created by Timothy Banks on 4/27/21.
//

#ifndef array_h
#define array_h

#include <array>
#include <string>
#include <vector>

namespace array_{

/// Given an array that contains values between 1 and N, return true if the array contains duplicates.
template <uint64_t N, typename T>
bool contains_duplicates(const std::vector<T>& v) {
    static constexpr auto sum = (N * (N + 1)) / 2;
    
    auto actual = uint64_t{0};
    for (const auto& i : v) {
        actual += i;
    }
    
    return actual != sum;
}

/// Given a sorted array, remove duplicates from it.
template <typename T>
void remove_duplicates_sorted(std::vector<T>& v) {
    auto start = size_t{0};
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] != v[start]) {
            v[++start] = v[i];
        }
    }
    v.erase(std::begin(v) + start + 1, std::end(v));
}

/// Given an array that contains values between 1 and K, use a scratch buffer to sort the array.
template <uint64_t K, typename T>
void scratch_sort(std::vector<T>& v) {
    auto scratch = std::array<T, K>{};
    scratch.fill(0);
    for (const auto& i : v) {
        ++scratch[i];
    }
    v.clear();
    for (size_t i = 0; i < scratch.size(); ++i) {
        for (size_t j = 0; j < scratch[i]; ++j) {
            v.push_back(i);
        }
    }
}

namespace details {
void reverse(std::string& s, size_t begin, size_t end) {
    auto mid = (end - begin) / 2;
    for (size_t i = 0; i <= mid; ++i) {
        std::swap(s[begin + i], s[end - i - 1]);
    }
}
}

/// Given a string, reverse it.
void reverse(std::string& s) {
    details::reverse(s, 0, s.size());
}

/// Given a string, reverse the words in it.
void reverse_words(std::string& s) {
    auto start = size_t{0};
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == ' ') {
            details::reverse(s, start, i);
            start = i + 1;
        }
    }
    details::reverse(s, start, s.size());
    details::reverse(s, 0, s.size());
}

void run_tests() {
    auto values = std::vector<uint16_t>{1, 1, 2, 3, 4, 5, 5, 6, 6, 7, 8, 9, 10, 10};
    auto result = contains_duplicates<10>(values);
    remove_duplicates_sorted(values);
    values = std::vector<uint16_t>{9, 7, 5, 3, 1, 2, 4, 6, 8, 10};
    scratch_sort<11>(values);
    
    auto s = std::string{"hello world"};
    reverse_words(s);
}

}

#endif /* array_h */

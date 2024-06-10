#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace training {
/* Various dynamic programming problems */

enum solve_by { brute_force, memoization, bottom_up };

namespace longest_common_subsequence {
namespace details {
/*
  https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/?ref=lbp
*/
template <typename T>
int32_t brute_force(const std::vector<T> &left, size_t lend,
                    const std::vector<T> &right, size_t rend) {
  if (lend == 0 || rend == 0) {
    return 0;
  } else if (left[lend - 1] == right[rend - 1]) {
    return 1 + brute_force(left, lend - 1, right, rend - 1);
  } else {
    return std::max(brute_force(left, lend, right, rend - 1),
                    brute_force(left, lend - 1, right, rend));
  }
}

using lookup_table =
    std::unordered_map<size_t, std::unordered_map<size_t, int32_t>>;
template <typename T>
int32_t memoization(const std::vector<T> &left, size_t lend,
                    const std::vector<T> &right, size_t rend,
                    lookup_table &table) {
  if (lend == 0 || rend == 0) {
    return 0;
  }

  auto it1 = table.find(lend - 1);
  if (it1 != std::end(table)) {
    auto it2 = it1->second.find(rend - 1);
    if (it2 != std::end(it1->second)) {
      return it2->second;
    }
  }

  auto result = int32_t{0};
  if (left[lend - 1] == right[rend - 1]) {
    result = 1 + memoization(left, lend - 1, right, rend - 1, table);
  } else {
    result = std::max(memoization(left, lend, right, rend - 1, table),
                      memoization(left, lend - 1, right, rend, table));
  }
  table[lend - 1][rend - 1] = result;
  return result;
}

template <typename T>
int32_t bottom_up(const std::vector<T> &left, const std::vector<T> &right) {
  auto lookup = lookup_table{};

  for (size_t l = 0; l <= left.size(); ++l) {
    for (size_t r = 0; r <= right.size(); ++r) {
      if (l == 0 || r == 0) {
        lookup[l][r] = 0;
        continue;
      }
      if (left[l - 1] == right[r - 1]) {
        lookup[l][r] = 1 + lookup[l - 1][r - 1];
        continue;
      }
      lookup[l][r] = std::max(lookup[l - 1][r], lookup[l][r - 1]);
    }
  }

  return lookup[left.size()][right.size()];
}
} // namespace details

template <typename T>
int32_t solve(solve_by by, const std::vector<T> &left,
              const std::vector<T> &right) {
  switch (by) {
  case solve_by::brute_force:
    return details::brute_force(left, left.size(), right, right.size());
  case solve_by::bottom_up:
    return details::bottom_up(left, right);
  case solve_by::memoization: {
    auto lookup = details::lookup_table{};
    return details::memoization(left, left.size(), right, right.size(), lookup);
  } break;
  }
}
} // namespace longest_common_subsequence

namespace longest_increasing_subsequence {
/* https://www.geeksforgeeks.org/longest-increasing-subsequence-dp-3/?ref=lbp */
namespace details {
template <typename T>
int32_t brute_force(const std::vector<T> &v, size_t index) {
  if (index == 0) {
    return 1;
  }
  auto max_count = int32_t{1};
  for (int32_t i = index - 1; i >= 0; --i) {
    auto result = brute_force(v, i);
    if (v[index] >= v[i]) {
      max_count = std::max(max_count, result + 1);
    }
  }
  return max_count;
}

using lookup_table = std::unordered_map<size_t, int32_t>;
template <typename T>
int32_t memoization(const std::vector<T> &v, size_t index,
                    lookup_table &lookup) {
  if (index == 0) {
    return 1;
  }

  auto it = lookup.find(index);
  if (it != std::end(lookup)) {
    return it->second;
  }

  auto max_count = int32_t{1};
  for (int32_t i = index - 1; i >= 0; --i) {
    auto result = brute_force(v, i);
    if (v[index] >= v[i]) {
      max_count = std::max(max_count, result + 1);
    }
  }
  lookup[index] = max_count;
  return max_count;
}

template <typename T> int32_t bottom_up(const std::vector<T> &v) {
  auto lookup = std::vector<int32_t>(v.size(), 0);
  lookup[0] = 1;
  auto max = int32_t{1};
  for (size_t i = 1; i < v.size(); ++i) {
    lookup[i] = 1;
    for (size_t j = 0; j < i; ++j) {
      if (v[i] > v[j]) {
        lookup[i] = std::max(lookup[i], lookup[j] + 1);
        max = std::max(max, lookup[i]);
      }
    }
  }
  return max;
}
} // namespace details

template <typename T> int32_t solve(solve_by by, const std::vector<T> &v) {
  switch (by) {
  case solve_by::brute_force:
    return details::brute_force(v, v.size() - 1);
  case solve_by::bottom_up:
    return details::bottom_up(v);
  case solve_by::memoization: {
    auto lookup = details::lookup_table{};
    return details::memoization(v, v.size() - 1, lookup);
  } break;
  }
}
} // namespace longest_increasing_subsequence

namespace make_coins {
/* https://leetcode.com/problems/coin-change/ */

namespace details {
template <typename T>
int32_t brute_force(const std::vector<T> &coins, size_t index, T amount) {
  if (amount == 0) {
    return 0;
  }

  auto min_count = std::numeric_limits<int32_t>::max();
  for (auto i = index; i < coins.size(); ++i) {
    auto &coin = coins[i];
    if (amount < coin) {
      continue;
    }
    auto result = brute_force(coins, i, amount - coin);
    if (result > -1) {
      min_count = std::min(min_count, result + 1);
    }
  }
  return min_count == std::numeric_limits<int32_t>::max() ? -1 : min_count;
}

using lookup_table = std::unordered_map<size_t, int32_t>;
template <typename T>
int32_t memoization(const std::vector<T> &coins, size_t index, T amount,
                    lookup_table &table) {
  if (amount == 0) {
    return 0;
  }

  auto it = table.find(amount);
  if (it != std::end(table)) {
    return it->second;
  }

  auto min_count = std::numeric_limits<int32_t>::max();
  for (auto i = index; i < coins.size(); ++i) {
    auto &coin = coins[i];
    if (amount < coin) {
      continue;
    }
    auto result = memoization(coins, i, amount - coin, table);
    if (result > -1) {
      min_count = std::min(min_count, result + 1);
    }
  }
  table[amount] = min_count;
  return min_count == std::numeric_limits<int32_t>::max() ? -1 : min_count;
}

template <typename T> int32_t bottom_up(const std::vector<T> &coins, T amount) {
  auto table = std::vector<T>(amount + 1, std::numeric_limits<T>::max());
  table[0] = 0;

  for (T i = 1; i <= amount; ++i) {
    for (size_t j = 0; j < coins.size(); ++j) {
      auto &coin = coins[j];
      if (i < coin) {
        continue;
      }
      table[i] = std::min(table[i], table[i - coin] + 1);
    }
  }

  return table[amount] > amount ? -1 : table[amount];
}
} // namespace details

template <typename T>
int32_t solve(solve_by by, const std::vector<T> &coins, T amount) {
  switch (by) {
  case solve_by::brute_force:
    return details::brute_force(coins, 0, amount);
  case solve_by::bottom_up:
    return details::bottom_up(coins, amount);
  case solve_by::memoization: {
    auto lookup = details::lookup_table{};
    return details::memoization(coins, 0, amount, lookup);
  } break;
  }
}
} // namespace make_coins

namespace minimum_product {
/* https://www.geeksforgeeks.org/minimum-product-k-integers-array-positive-integers/?ref=lbp
 */
/* Can also solve by using min heap and popping first k elements off of min heap
 */
namespace details {
template <typename T> int32_t min_heap(const std::vector<T> &v, int32_t k) {
  auto heap = std::priority_queue<T, std::vector<T>, std::greater<T>>{};

  for (const auto &e : v) {
    heap.push(e);
  }

  auto sum = int32_t{1};
  for (auto i = 0; i < k; ++i) {
    sum *= heap.top();
    heap.pop();
  }
  return sum;
}

template <typename T>
int32_t brute_force(const std::vector<T> &v, size_t index, int32_t k) {
  if (k == 0) {
    return 1;
  }
  auto min_product = std::numeric_limits<T>::max();
  for (auto i = index; i < v.size() - k; ++i) {
    auto product = v[i] * brute_force(v, i + 1, k - 1);
    min_product = std::min(min_product, product);
  }
  return min_product;
}

using lookup_table =
    std::unordered_map<size_t, std::unordered_map<size_t, int32_t>>;
template <typename T>
int32_t memoization(const std::vector<T> &v, size_t index, int32_t k,
                    lookup_table &table) {
  if (k == 0) {
    return 1;
  }
  auto it1 = table.find(index);
  if (it1 != std::end(table)) {
    auto it2 = it1->second.find(k);
    if (it2 != std::end(it1->second)) {
      return it2->second;
    }
  }
  auto min_product = std::numeric_limits<T>::max();
  for (auto i = index; i < v.size() - k; ++i) {
    auto product = v[i] * memoization(v, i + 1, k - 1, table);
    min_product = std::min(min_product, product);
  }
  table[index][k] = min_product;
  return min_product;
}

template <typename T> int32_t bottom_up(const std::vector<T> &v, int32_t k) {
  auto mins =
      std::unordered_map<int32_t, std::pair<T, std::unordered_set<size_t>>>{};
  mins[0] = std::pair(1, std::unordered_set<size_t>{});

  for (size_t i = 1; i <= k; ++i) {
    auto &global_min = mins[i - 1];
    auto min = std::pair(std::numeric_limits<T>::max(), global_min.second);

    for (size_t j = 0; j < v.size(); ++j) {
      auto local_min = v[j] * global_min.first;
      auto it = global_min.second.find(j);
      if (it == std::end(global_min.second) && local_min < min.first) {
        min = std::pair(local_min, global_min.second);
        min.second.emplace(j);
      }
    }

    mins[i] = min;
  }

  return mins[k].first;
}
} // namespace details

template <typename T>
int32_t solve(solve_by by, const std::vector<T> &v, int32_t k) {
  switch (by) {
  case solve_by::brute_force:
    return details::brute_force(v, 0, k);
  case solve_by::bottom_up:
    return details::bottom_up(v, k);
  case solve_by::memoization: {
    auto lookup = details::lookup_table{};
    return details::memoization(v, 0, k, lookup);
  } break;
  }
}
} // namespace minimum_product

namespace kth_largest_sum {
/* https://www.geeksforgeeks.org/k-th-largest-sum-contiguous-subarray/?ref=lbp
 */
namespace details {
template <typename T>
int32_t brute_force(const std::vector<T> &v, size_t index, int32_t k) {}

using lookup_table =
    std::unordered_map<size_t, std::unordered_map<size_t, int32_t>>;
template <typename T>
int32_t memoization(const std::vector<T> &v, size_t index, int32_t k,
                    lookup_table &table) {}

template <typename T> int32_t bottom_up(const std::vector<T> &v, int32_t k) {}
} // namespace details

template <typename T>
int32_t solve(solve_by by, const std::vector<T> &v, int32_t k) {
  switch (by) {
  case solve_by::brute_force:
    return details::brute_force(v, 0, k);
  case solve_by::bottom_up:
    return details::bottom_up(v, k);
  case solve_by::memoization: {
    auto lookup = details::lookup_table{};
    return details::memoization(v, 0, k, lookup);
  } break;
  }
}
} // namespace kth_largest_sum

void run_tests() {
  auto left = std::vector<char>{'A', 'G', 'G', 'T', 'A', 'B'};
  auto right = std::vector<char>{'G', 'X', 'T', 'X', 'A', 'Y', 'B'};
  std::cout << "LCS brute force "
            << std::to_string(longest_common_subsequence::solve(
                   solve_by::brute_force, left, right))
            << std::endl;
  std::cout << "LCS memoization"
            << std::to_string(longest_common_subsequence::solve(
                   solve_by::memoization, left, right))
            << std::endl;
  std::cout << "LCS bottom up/tabulation"
            << std::to_string(longest_common_subsequence::solve(
                   solve_by::bottom_up, left, right))
            << std::endl;

  auto sequence1 = std::vector<int32_t>{10, 22, 9, 33, 21, 50, 41, 60, 80}; // 6
  auto sequence2 = std::vector<int32_t>{3, 10, 2, 1, 20};                   // 3
  auto sequence3 = std::vector<int32_t>{50, 3, 10, 7, 40, 80};              // 4
  std::cout << "LIS brute force "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::brute_force, sequence1))
            << std::endl;
  std::cout << "LIS brute force "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::brute_force, sequence2))
            << std::endl;
  std::cout << "LIS brute force "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::brute_force, sequence3))
            << std::endl;
  std::cout << "LIS memoization "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::memoization, sequence1))
            << std::endl;
  std::cout << "LIS memoization "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::memoization, sequence2))
            << std::endl;
  std::cout << "LIS memoization "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::memoization, sequence3))
            << std::endl;
  std::cout << "LIS bottom up/tabulation "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::bottom_up, sequence1))
            << std::endl;
  std::cout << "LIS bottom up/tabulation "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::bottom_up, sequence2))
            << std::endl;
  std::cout << "LIS bottom up/tabulation "
            << std::to_string(longest_increasing_subsequence::solve(
                   solve_by::bottom_up, sequence3))
            << std::endl;

  auto coins = std::vector<int32_t>{1, 2, 5};
  std::cout << "COINS brute force "
            << std::to_string(
                   make_coins::solve(solve_by::brute_force, coins, 11))
            << std::endl;
  std::cout << "COINS memoization "
            << std::to_string(
                   make_coins::solve(solve_by::memoization, coins, 11))
            << std::endl;
  std::cout << "COINS bottom up/tabulation "
            << std::to_string(make_coins::solve(solve_by::bottom_up, coins, 11))
            << std::endl;

  auto sequence4 = std::vector<int32_t>{198, 76, 544, 123, 154, 675}; // 9348
  auto sequence5 = std::vector<int32_t>{11, 8, 5, 7, 5, 100};         // 1400
  std::cout << "Min Product brute force "
            << std::to_string(
                   minimum_product::solve(solve_by::brute_force, sequence4, 2))
            << std::endl;
  std::cout << "Min Product brute force "
            << std::to_string(
                   minimum_product::solve(solve_by::brute_force, sequence5, 4))
            << std::endl;
  std::cout << "Min Product memoization "
            << std::to_string(
                   minimum_product::solve(solve_by::memoization, sequence4, 2))
            << std::endl;
  std::cout << "Min Product memoization "
            << std::to_string(
                   minimum_product::solve(solve_by::memoization, sequence5, 4))
            << std::endl;
  std::cout << "Min Product bottom up/tabulation "
            << std::to_string(
                   minimum_product::solve(solve_by::bottom_up, sequence4, 2))
            << std::endl;
  std::cout << "Min Product bottom up/tabulation "
            << std::to_string(
                   minimum_product::solve(solve_by::bottom_up, sequence5, 4))
            << std::endl;
  std::cout << "Min Product heap "
            << std::to_string(minimum_product::details::min_heap(sequence4, 2))
            << std::endl;
  std::cout << "Min Product heap "
            << std::to_string(minimum_product::details::min_heap(sequence5, 4))
            << std::endl;
}

} // namespace training
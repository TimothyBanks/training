#pragma once
#include <vector>

namespace bullish {

struct transaction {
  uint64_t gas_price;
  uint64_t gas_limit;
  uint64_t extra_reward;
};

namespace details {
namespace tabulation {
uint64_t calculate_mev(const std::vector<transaction>& transactions,
                       uint64_t block_gas_limit) {
  auto table = std::vector<std::vector<uint64_t>>(
      transactions.size() + 1, std::vector<uint64_t>(block_gas_limit + 1, 0));

  for (size_t i = 1; i <= transactions.size(); ++i) {
    for (size_t j = 1; j <= block_gas_limit; ++j) {
      if (i == transactions.size() && j == block_gas_limit) {
        volatile auto x = 0;
      }

      if (transactions[i - 1].gas_limit > j) {
        table[i][j] = table[i - 1][j];
      } else {
        auto without = table[i - 1][j];
        auto value =
            transactions[i - 1].gas_price * transactions[i - 1].gas_limit +
            transactions[i - 1].extra_reward;
        auto value2 = table[i - 1][j - transactions[i - 1].gas_limit];
        auto with = value2 + value;
        table[i][j] = std::max(without, with);
      }
    }
  }

  return table[transactions.size()][block_gas_limit];
}
}  // namespace tabulation

namespace recursive {
uint64_t calculate_mev(const std::vector<transaction>& transactions,
                       uint64_t block_gas_limit,
                       size_t i,
                       uint64_t current_reward,
                       uint64_t used_gas) {
  if (i > transactions.size()) {
    return current_reward;
  }

  if (block_gas_limit == used_gas) {
    return current_reward;
  }

  if (used_gas > block_gas_limit) {
    return 0;
  }

  auto max_reward = current_reward;
  for (size_t j = i; j < transactions.size(); ++j) {
    auto with = calculate_mev(
        transactions, block_gas_limit, j + 1,
        current_reward + transactions[j].gas_price * transactions[j].gas_limit +
            transactions[j].extra_reward,
        used_gas + transactions[j].gas_limit);
    auto without = calculate_mev(transactions, block_gas_limit, j + 1,
                                 current_reward, used_gas);
    max_reward = std::max(max_reward, std::max(with, without));
  }
  return max_reward;
}
}  // namespace recursive
}  // namespace details

uint64_t calculate_mev(const std::vector<transaction>& transactions,
                       uint64_t block_gas_limit) {
  return details::tabulation::calculate_mev(transactions, block_gas_limit);
  // return details::recursive::calculate_mev(transactions, block_gas_limit, 0,
  // 0, 0);
}

}  // namespace bullish
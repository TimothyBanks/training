#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Data {
  std::unordered_map<std::string, double> m_table;
  double m_threshold = 0.0;

  double balance(const std::string &address) {
    auto it = m_table.find(address);
    if (it == std::end(m_table)) {
      return 0.0;
    }
    return it->second;
  }

  void set_balance(const std::string &address, double amount) {
    auto it = m_table.find(address);
    if (it == std::end(m_table)) {
      m_table.emplace(address, amount);
      return;
    }
    it->second = amount;
  }

  double threshold() { return m_threshold; }

  void set_threshold(double threshold) { m_threshold = threshold; }

  static Data &instance() {
    static auto d = Data{};
    return d;
  }
};

inline void autoflush(const std::string &address_a,
                      const std::string &address_b) {
  auto balance_a = Data::instance().balance(address_a);
  auto balance_b = Data::instance().balance(address_b);

  if (balance_a <= 0.0) {
    return;
  }

  if (balance_a > Data::instance().threshold() || balance_b / balance_a < .5) {
    Data::instance().set_balance(address_a, balance_a + balance_b);
    Data::instance().set_balance(address_b, 0.0);
  }
}

int main() {
  auto address_a = std::string{"0x123456"};
  auto address_b = std::string{"0x654321"};
  Data::instance().set_threshold(100.0);
  Data::instance().set_balance(address_a, 101.00);
  Data::instance().set_balance(address_b, 10.00);
  autoflush(address_a, address_b);

  auto print = []() {
    for (const auto &p : Data::instance().m_table) {
      std::cout << p.first << ": " << p.second << std::endl;
    }
  };

  print();

  Data::instance().set_balance(address_a, 50.00);
  Data::instance().set_balance(address_b, 10.00);
  autoflush(address_a, address_b);

  print();

  Data::instance().set_balance(address_a, 50.00);
  Data::instance().set_balance(address_b, 24.00);
  autoflush(address_a, address_b);

  print();

  return 0;
}
// Your old code in text has been preserved below.

// - What are ERC20 tokens?
// - What are hash functions?
// - What is an RPC?

// # Memory improvement
// - Which struct consumes more space, and why?
// struct MyStructA {
//     unit8 param1
//     bytes31 param2

//     bytes32 param3
// }

// struct MyStructB {
//     bytes32 param3
//     bytes32 param2
//     unit8 param1
// }

// # Interaction with smart contracts
// Write a function that accepts two addresses, compares their balances, and
// moves funds from address_a to address_b if the balance of address_a is either
// more than 50% of the address_b or higher than threshold_size. Use your
// framework of choice (ethers.js preferred). auto-flush

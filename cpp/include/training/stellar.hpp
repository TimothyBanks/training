#include <iostream>
#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

struct result {
  std::vector<std::string> path;
  double rate{0.0};
};

using assets = std::unordered_map<std::string, std::unordered_map<std::string, double>>;
using visited = std::unordered_set<std::string>;

result find_best(const std::string& start, const std::string& end, const assets& lookup, visited& v)
{
  auto it = lookup.find(start);
  if (v.find(start) != std::end(v) || it == std::end(lookup)) {
    return {};
  }
  
  if (start == end) {
    return {.path = {start}, .rate = 1};
  }
  
  auto global_result = result{.rate = 0};
  v.emplace(start);
  for (auto p = std::begin(it->second); p != std::end(it->second); ++p) {
    auto local_result = find_best(p->first, end, lookup, v);
    if (!local_result.path.empty() && local_result.rate * p->second > global_result.rate) {
      global_result.rate = local_result.rate * p->second;
      global_result.path = local_result.path;
      global_result.path.insert(std::begin(global_result.path), start);
    }
  }
  
  return global_result;
}

result find_best(const std::string& start, const std::string& end, const assets& lookup)
{
  auto v = visited{};
  return find_best(start, end, amount, lookup, v);
}

int main() {
  auto lookup = assets{
    {"X", {{"Z", 2}, {"F", 1.5}}},
    {"Z", {{"Y", 3}, {"X", 0.3}}},
    {"F", {{"Z", 2}}},
    {"Y", {{"Z", 0.3}}},
    {"W", {{"U", 2}}},
  };
 
  auto print = [](const auto& r) {
    std::cout << "Rate: " << std::to_string(r.rate) << std::endl;
    std::cout << "Path: ";
    for (const auto& p : r.path) {
      std::cout << p << " -> ";
    }
    std::cout << std::endl;
  };
  
  std::cout << "X -> Y" << std::endl;
  print(find_best("X", "Y", lookup));
  std::cout << "Y -> X" << std::endl;
  print(find_best("Y", "X", lookup));
  std::cout << "X -> F" << std::endl;
  print(find_best("X", "F", lookup));
  std::cout << "X -> X" << std::endl;
  print(find_best("X", "X", lookup));
  std::cout << "XX -> Y" << std::endl;
  print(find_best("XX", "Y", lookup));
  std::cout << "X -> YY" << std::endl;
  print(find_best("X", "YY", lookup));
  std::cout << "XX -> YY" << std::endl;
  print(find_best("XX", "YY", lookup));
  std::cout << "XX -> XX" << std::endl;
  print(find_best("XX", "XX", lookup));
  std::cout << "X -> W" << std::endl;
  print(find_best("X", "W", lookup));
  
  return 0;
}

// Source: X, Destination: Y
// Prices:
// X -> Z: 2
// Z -> Y: 3
// X -> F: 1.5
// F -> Z: 2
// Y -> Z: 0.3
// Z -> X: 0.3

// // 1.a
// assetDataStructure = ...

// // 1.b
// findBest("X", "Y", assetDataStructure)  // X->Y should return 9
// X -> F: 1.5
// F -> Z: 2
// Z -> Y: 3

// findBest("Y", "X", assetDataStructure)  // Y->X should return .09
// Y -> Z: 0.3
// Z -> X: 0.3

/*
Source: X, Destination: Y, amount: 3
Price,amount per market:
X -> Y: Price=2, Amount=1
X -> Z: Price=1.5, Amount=4
Z -> Y: Price=3, Amount=30
Y -> Z: Price=0.3, Amount=4
Z -> X: Price=0.2, Amount=1


X -> Z  
Z -> Y
*/

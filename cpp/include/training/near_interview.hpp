// Implement a simple calculator that takes an arithmetic formula as input and
// evaluates it. Operations include +,-,*,/ and () Input: (1 + 2) * 3 Output: 9
// Input: (1 + 2 * 3) * 3 Output: 21

//[(, 1, +, 21, *, 3, )]
//[(, 1, +, 2, ), *, 3]
// 1+ 2 * 3 -> 7
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

template <typename Functor>
void drain_until(std::stack<std::string> &s, Functor &&f) {
  auto keep_going = f(s);
  while (keep_going) {
    auto right_side = std::atoi(s.top().c_str());
    s.pop();
    auto op = s.top();
    s.pop();
    auto left_side = std::atoi(s.top().c_str());
    s.pop();

    keep_going = f(s);
    if (op == "+") {
      s.push(std::to_string(right_side + left_side));
    } else if (op == "-") {
      s.push(std::to_string(left_side - right_side));
    }
  }
}

int64_t evaluate(const std::vector<std::string> &tokens) {
  auto s = std::stack<std::string>{};

  for (auto i = 0; i < tokens.size();) {
    const auto &token = tokens[i];

    if (token == ")") {
      drain_until(s, [](auto &s) {
        auto peek = s.top();
        if (peek == "(") {
          s.pop();
          return false;
        }
        return true;
      });
      ++i;
    } else if (token == "*") {
      auto left_side = std::atoi(s.top().c_str());
      s.pop();
      auto right_side = std::atoi(tokens[i + 1].c_str());
      s.push(std::to_string(left_side * right_side));
      i += 2;
    } else if (token == "/") {
      auto left_side = std::atoi(s.top().c_str());
      s.pop();
      auto right_side = std::atoi(tokens[i + 1].c_str());
      s.push(std::to_string(left_side / right_side));
      i += 2;
    } else {
      s.push(token);
      ++i;
    }
  }

  drain_until(s, [](auto &s) { return s.size() > 1; });

  return std::atoi(s.top().c_str());
}

int64_t evaluate2(const std::vector<std::string> &tokens, size_t &start,
                  bool short_circuit) {
  auto s = std::stack<std::string>{};

  for (; start < tokens.size();) {
    const auto &token = tokens[start];

    if (token == ")") {
      drain_until(s, [](auto &s) {
        auto peek = s.top();
        if (peek == "(") {
          s.pop();
          return false;
        }
        return true;
      });
      ++start;
    } else if (token == "*") {
      auto left_side = std::atoi(s.top().c_str());
      s.pop();
      ++start;
      auto right_side = evaluate2(tokens, start, true);
      s.push(std::to_string(left_side * right_side));
    } else if (token == "/") {
      auto left_side = std::atoi(s.top().c_str());
      s.pop();
      ++start;
      auto right_side = evaluate2(tokens, start, true);
      s.push(std::to_string(left_side / right_side));
    } else {
      s.push(token);
      ++start;
    }

    if (short_circuit && s.size() == 1 &&
        s.top().find_first_of("()*/+-") == std::string::npos) {
      break;
    }
  }

  drain_until(s, [](auto &s) { return s.size() > 1; });

  return std::atoi(s.top().c_str());
}

int64_t evaluate2(const std::vector<std::string> &tokens) {
  auto start = size_t{0};
  return evaluate2(tokens, start, false);
}

// To execute C++, please define "int main()"
inline void test_calculator() {
  //[(, 1, +, 21, *, 3, )]
  //[(, 1, +, 2, ), *, 3]
  // 1+ 2 * 3 -> 7

  auto tokens = std::vector<std::string>{"(", "1", "+", "21", "*", "3", ")"};
  std::cout << evaluate(tokens) << std::endl;
  tokens = {"(", "1", "+", "2", ")", "*", "3"};
  std::cout << evaluate(tokens) << std::endl;
  tokens = {"1", "+", "2", "*", "3"};
  std::cout << evaluate(tokens) << std::endl;
  tokens = {"1"};
  std::cout << evaluate(tokens) << std::endl;
  tokens = {"(", "1", "+", "(", "2", "+", "2", ")", ")", "*", "3"};
  std::cout << evaluate(tokens) << std::endl;
  // tokens = { "(", "1", "+", "(", "2", "+", "2", ")", ")", "*", "(", "3", "+",
  // "1", ")"}; std::cout << evaluate(tokens) << std::endl;

  tokens = std::vector<std::string>{"(", "1", "+", "21", "*", "3", ")"};
  std::cout << evaluate2(tokens) << std::endl;
  tokens = {"(", "1", "+", "2", ")", "*", "3"};
  std::cout << evaluate2(tokens) << std::endl;
  tokens = {"1", "+", "2", "*", "3"};
  std::cout << evaluate2(tokens) << std::endl;
  tokens = {"1"};
  std::cout << evaluate2(tokens) << std::endl;
  tokens = {"(", "1", "+", "(", "2", "+", "2", ")", ")", "*", "3"};
  std::cout << evaluate2(tokens) << std::endl;
  tokens = {"(", "1", "+", "(", "2", "+", "2", ")",
            ")", "*", "(", "3", "+", "1", ")"};
  std::cout << evaluate2(tokens) << std::endl;
  tokens = {"(", "1", "+", "(", "2", "+", "2", ")", ")", "*", "(", "(",
            "3", "*", "(", "5", "-", "4", ")", ")", "+", "1", ")"};
  std::cout << evaluate2(tokens) << std::endl;
}

// #include <iostream>
// #include <chrono>
// #include <cstdlib>
// #include <thread>
// #include <vector>

// // #include <training/huffman.hpp>
// #include <training/bounded_buffer.hpp>
// #include <training/dp.hpp>
// #include <training/fibonacci.hpp>
// #include <training/sort.hpp>
// #include <training/tree.hpp>
// #include <training/moving_average.hpp>
// #include <training/near_interview.hpp>
// #include <training/anchorage.hpp>

// #include <training/range_response_processor.hpp>

// #include <training/mev.hpp>

// #include <training/millennium.hpp>

// template <typename T>
// std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
// {
//   out << "[";
//   for (const auto& e : v) {
//     out << std::to_string(e) << ", ";
//   }
//   out << "]";
//   return out;
// }

// void run_tree_tests()
// {
//   std::cout << "Running tree tests...\n";
//   auto values = std::vector<char>{'F', 'B', 'G', 'A', 'D', '\0', 'I', '\0',
//   '\0', 'C', 'E', 'H', '\0'}; auto tree = training::tree<char>{values}; auto
//   print = [](auto& data) {
//     std::cout << data;
//     return false;
//   };
//   std::cout << "Breadth First: ";
//   tree.traverse(training::tree<char>::traversal::breadth_first, print);
//   std::cout << "\nDepth First, Pre Order: ";
//   tree.traverse(training::tree<char>::traversal::depth_first_pre_order,
//   print); std::cout << "\nDepth First, In Order: ";
//   tree.traverse(training::tree<char>::traversal::depth_first_in_order,
//   print); std::cout << "\nDepth First, Post Order: ";
//   tree.traverse(training::tree<char>::traversal::depth_first_post_order,
//   print); std::cout << "\nFinished tree tests...\n";
// }

// void run_fibonnaci_tests()
// {
//   std::cout << "Running Fibonacci tests...\n";
//   std::cout << std::to_string(training::fibonacci<5>()) << "\n";
//   std::cout << std::to_string(training::fibonacci(5)) << "\n";
//   std::cout << std::to_string(training::fibonacci2(5)) << "\n";
//   std::cout << std::to_string(training::fibonacci3(5)) << "\n";
//   std::cout << std::to_string(training::fibonacci<10>()) << "\n";
//   std::cout << std::to_string(training::fibonacci(10)) << "\n";
//   std::cout << std::to_string(training::fibonacci2(10)) << "\n";
//   std::cout << std::to_string(training::fibonacci3(10)) << "\n";
//   std::cout << std::to_string(training::fibonacci<12>()) << "\n";
//   std::cout << std::to_string(training::fibonacci(12)) << "\n";
//   std::cout << std::to_string(training::fibonacci2(12)) << "\n";
//   std::cout << std::to_string(training::fibonacci3(12)) << "\n";
//   std::cout << std::to_string(training::fibonacci<50>()) << "\n";
//   std::cout << std::to_string(training::fibonacci(50)) << "\n";
//   // Significant time to compute
//   // std::cout << std::to_string(training::fibonacci2(50)) << "\n";
//   std::cout << std::to_string(training::fibonacci3(50)) << "\n";
//   std::cout << "Finished Fibonacci tests...\n";
// }

// void run_bubble_sort_tests()
// {
//   std::cout << "Running BubbleSort tests...\n";
//   auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
//   std::cout << "Unsorted: " << s << "\n";
//   training::sort::bubble::sort(s);
//   std::cout << "Sorted: " << s << "\n";
//   std::cout << "Finished BubbleSort tests...\n";
// }

// void run_selection_sort_tests()
// {
//   std::cout << "Running SelectionSort tests...\n";
//   auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
//   std::cout << "Unsorted: " << s << "\n";
//   training::sort::selection::sort(s);
//   std::cout << "Sorted: " << s << "\n";
//   std::cout << "Finished SelectionSort tests...\n";
// }

// void run_insertion_sort_tests()
// {
//   std::cout << "Running InsertionSort tests...\n";
//   auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
//   std::cout << "Unsorted: " << s << "\n";
//   training::sort::insertion::sort(s);
//   std::cout << "Sorted: " << s << "\n";
//   std::cout << "Finished InsertionSort tests...\n";
// }

// void run_quick_sort_tests()
// {
//   std::cout << "Running QuickSort tests...\n";
//   auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
//   std::cout << "Unsorted: " << s << "\n";
//   training::sort::quick::sort(s);
//   std::cout << "Sorted: " << s << "\n";
//   std::cout << "Finished QuickSort tests...\n";
// }

// void run_merge_sort_tests()
// {
//   std::cout << "Running MergeSort tests...\n";
//   auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
//   std::cout << "Unsorted: " << s << "\n";
//   training::sort::merge::sort(s);
//   std::cout << "Sorted: " << s << "\n";
//   std::cout << "Finished MergeSort tests...\n";
// }

// void run_moving_average_tests()
// {
//   auto mv = training::moving_average{4};

//   std::cout << std::to_string(mv(1)(2)(3)(4).average()) << std::endl;
//   std::cout << std::to_string(mv(5)(6)(7)(8).average()) << std::endl;
// }

// #include <training/pinestreet.hpp>

// int main (int argc, char *argv[])
// {
//   anchorage::tests::run_tests();
//   // millennium::tests::run_tests();

//   // auto result = bullish::calculate_mev({
//   //   {2, 50, 6},
//   //   {1, 70, 8},
//   //   {3, 40, 5},
//   //   {2, 60, 10}
//   // }, 100);
//   // std::cout << std::to_string(result) << "\n";

//   // pinestreet::tests::run_tests();
//   // huffman::tests::run_test();

//   // test_calculator();
//   // run_bounded_buffer_test();

//   // run_moving_average_tests();
//   // training::run_tests();

//   // run_tree_tests();
//   // run_fibonnaci_tests();
//   // run_bubble_sort_tests();
//   // run_insertion_sort_tests();
//   // run_selection_sort_tests();
//   // run_quick_sort_tests();
//   // run_merge_sort_tests();
//   return EXIT_SUCCESS;
// }

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
//#include <training/hash_map.hpp>
//#include <training/peque.hpp>
//#include <training/small_vector.hpp>
#include <training/asml/polygon.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

template <typename T, typename Enable = void>
struct less_;

template <typename T, typename Enable = void>
struct greater_;

template <typename T>
struct less_<T, std::enable_if_t<std::is_integral_v<T>>> {
  bool operator()(const T& l, const T& r) const { return l < r; }
};

template <typename T>
struct greater_<T, std::enable_if_t<std::is_integral_v<T>>> {
  bool operator()(const T& l, const T& r) const { return l > r; }
};

struct my_type {
  uint32_t foo;
  std::string bar;

  my_type() = default;
  my_type(const my_type&) = default;
  my_type(my_type&&) = default;
  my_type(uint32_t foo_, std::string bar_) : foo{foo_}, bar{std::move(bar_)} {}

  my_type& operator=(const my_type&) = default;
  my_type& operator=(my_type&&) = default;
};


std::tuple<int, std::string, int> parse_log(std::string& log) {
  std::stringstream ss(log);
  std::string id, type, time;
  std::getline(ss, id, ':');
  std::getline(ss, type, ':');
  std::getline(ss, time, ':');
  return std::make_tuple(std::stoi(id), type, std::stoi(time));
}

auto solve(int n, std::vector<std::string>& logs) {
  auto result = std::vector<int>(n, 0);
  auto stk = std::stack<std::pair<int, int>>{}; // <id, start_time>
  for (auto& log : logs) {
    auto [id, type, time] = parse_log(log);

    if (stk.empty() && type == "start") {
      stk.push({id, time});
    } else if (!stk.empty()) {
      auto p = stk.top();
      if (type == "start") {
        // update stack top
        result[p.first] += time - p.second;
        stk.push(std::make_pair(id, time));
      }
      else if (type == "end") {
        result[id] += time - p.second + 1;
        stk.pop();
        if (!stk.empty()) {
          stk.top().second = time + 1;
        }
      }
    }
  }
  return result;
}

bool bMatch(const std::string& pattern, const std::string& word) {
    int pos = 0; // Current position in the word

    for (char c : pattern) {
        if (c >= 'a' && c <= 'z') {
            // Ensure the current position in the word is valid
            if (pos >= word.size() || word[pos] != c) {
                return false;
            }
            ++pos; // Move to the next position in the word
        } else if (c >= '0' && c <= '9') {
            // Skip the specified number of characters
            pos += c - '0';
        } else {
            // Invalid character in pattern
            return false;
        }
    }

    // Check if we have used the entire word length
    return pos == word.size();
}

auto format_time(int seconds) {
  int hours = seconds / 3600;
  int remainder = seconds % 3600;
  int minutes = remainder / 60;
  int secs = remainder % 60;

  // Create a tm struct and populate it
  std::tm time_struct = {};
  time_struct.tm_hour = hours;
  time_struct.tm_min = minutes;
  time_struct.tm_sec = secs;

  // Use a string stream to format the time
  std::ostringstream ss;
  ss << std::put_time(&time_struct, "%H:%M:%S");
  return ss.str();
}

extern void PolygonTest();

int main(int argc, char* argv[]) {
  PolygonTest();

  return 0;

  std::cout << bMatch("2b2e", "  base") << std::endl;

  std::vector<std::vector<int>> data = {{1,60,23}, {2,23,3}, {31, 180, 0}, {3,12,0}, {23,30,31}};
  std::map<int, int> mp;
  std::unordered_map<int, int> unmp;
  std::vector<std::vector<int>> res;
  for (auto& line : data)
    mp.insert({line[0], line[2]});
  for (auto& line : data)
    unmp[line[0]] = line[1];
  while (!mp.empty()) {
    auto it = mp.begin();
    int totalTime = 0, startID = it->first, lastID = it->first, cnts = 0;
    while (true) {
      totalTime += unmp[it->first];
      if (it->second == 0) {
        mp.erase(it);
        cnts++;
        break;
      }
      auto next = mp.find(it->second);
      if (next == mp.end()) {
        std::cout << "Malformed Input" << std::endl;
        return 0;
      }
      mp.erase(it);
      it = next;
      lastID = next->first;
      cnts++;
    }
    res.push_back({startID, lastID, totalTime, cnts});
  }
  std::sort(res.begin(), res.end(), [](auto& a, auto& b) { return a[2] > b[2]; });
  for (auto& line : res) {
    std::cout << "-" << std::endl;
    std::cout << "start_job: " << line[0] << std::endl;
    std::cout << "last_job: " << line[1] << std::endl;
    std::cout << "number_of_jobs: " << line[3] << std::endl;
    std::cout << "job_chain_runtime: " << format_time(line[2]) << std::endl;
    std::cout << "average_job_time: " << format_time(line[2] / line[3]) << std::endl;
    std::cout << "-" << std::endl;
  }



  // {
  // auto logs = std::vector<std::string>{"0:start:0", "2:start:4", "2:end:5", "1:start:7", "1:end:10", "0:end:11"};
  // auto result = solve(3, logs);
  // for (auto& r : result)
  //   std::cout << r << ", ";
  // }
  // {
  //   std::cout << std::endl;
  //   auto logs = std::vector<std::string>{"0:start:2", "0:end:11"};
  //   auto result = solve(1, logs);
  //   for (auto& r : result)
  //     std::cout << r << ", ";
  // }
  // {
  //   std::cout << std::endl;
  //   auto logs = std::vector<std::string>{"0:start:0", "2:start:4", "2:end:5", "1:start:7", "0:end:8", "1:end:11"};
  //   auto result = solve(3, logs);
  //   for (auto& r : result)
  //     std::cout << r << ", ";
  // }

  return 0;

  // auto my_v = small_vector<my_type, 5>{};

  // for (auto i = size_t{0}; i < 16; ++i) {
  //   my_v.emplace_back(i, std::to_string(i));
  // }

  // std::cout << my_v.size() << std::endl;
  // for (auto i = size_t{0}; i < my_v.size(); ++i) {
  //   auto& v = my_v[i];
  //   std::cout << v.bar << ", " << v.foo << std::endl;
  // }

  // std::cout << my_v.size() << std::endl;
  // while (!my_v.empty()) {
  //   std::cout << my_v.size() << std::endl;
  //   my_v.pop_back();
  // }
  // std::cout << my_v.size() << std::endl;

  // auto my_hash = hash_map<uint64_t, uint64_t>{};
  // my_hash.insert(1, 20);
  // my_hash.insert(2, 21);
  // my_hash.insert(3, 22);
  // my_hash.insert(4, 23);
  // my_hash.insert(5, 24);
  // my_hash.insert(6, 25);
  // my_hash.insert(7, 26);
  // my_hash.insert(8, 27);
  // my_hash.insert(9, 28);
  // my_hash.insert(10, 29);
  // my_hash.insert(11, 30);
  // my_hash.print();

  // auto test_peque = [](auto compare) {
  //   std::cout << "Entering test..." << std::endl;
  //   auto p = peque<uint32_t, std::decay_t<decltype(compare)>>{};
  //   std::cout << "Pushing..." << std::endl;
  //   p.push(10);
  //   p.push(8);
  //   p.push(6);
  //   p.push(4);
  //   p.push(3);
  //   p.push(5);
  //   p.push(7);
  //   p.push(9);
  //   p.push(1);
  //   p.push(2);

  //   std::cout << "Printing..." << std::endl;
  //   while (!p.empty()) {
  //     auto front = p.front();
  //     p.pop();
  //     std::cout << front << std::endl;
  //   }
  //   std::cout << "Exiting test..." << std::endl;
  // };

  // std::cout << "less..." << std::endl;
  // test_peque(less_<uint32_t>{});
  // std::cout << "greater..." << std::endl;
  // test_peque(greater_<uint32_t>{});

  // return 0;

  // auto t = std::map<std::string, uint32_t>{
  //     {"z", 0}, {"y", 0}, {"x", 0}, {"w", 0}, {"v", 0},         {"u", 0},
  //     {"t", 0}, {"s", 0}, {"r", 0}, {"q", 0}, {"ddddddddd", 0}, {"o", 0},
  //     {"n", 0}, {"m", 0}, {"l", 0}, {"k", 0}, {"aaaaaa", 0},    {"i", 0},
  //     {"h", 0}, {"g", 0}, {"f", 0}, {"e", 0}, {"d", 0},         {"c", 0},
  //     {"b", 0}, {"a", 0},
  // };

  // for (auto& it : t) {
  //   std::cout << it.first << std::endl;
  // }
}

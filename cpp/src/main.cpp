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
//   auto values = std::vector<char>{'F', 'B', 'G', 'A', 'D', '\0', 'I', '\0', '\0', 'C', 'E', 'H', '\0'};
//   auto tree = training::tree<char>{values};
//   auto print = [](auto& data) {
//     std::cout << data;
//     return false;
//   };
//   std::cout << "Breadth First: ";
//   tree.traverse(training::tree<char>::traversal::breadth_first, print);
//   std::cout << "\nDepth First, Pre Order: ";
//   tree.traverse(training::tree<char>::traversal::depth_first_pre_order, print);
//   std::cout << "\nDepth First, In Order: ";
//   tree.traverse(training::tree<char>::traversal::depth_first_in_order, print);
//   std::cout << "\nDepth First, Post Order: ";
//   tree.traverse(training::tree<char>::traversal::depth_first_post_order, print);
//   std::cout << "\nFinished tree tests...\n";
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
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <type_traits>

template <typename T, typename Enable = void>
struct less_;

template <typename T, typename Enable = void>
struct greater_;

template <typename T>
struct less_<T, std::enable_if_t<std::is_integral_v<T>>> {
  bool operator()(const T& l, const T& r) const {
    return l < r;
  }
};


template <typename T>
struct greater_<T, std::enable_if_t<std::is_integral_v<T>>> {
  bool operator()(const T& l, const T& r) const {
    return l > r;
  }
};

template <typename T, typename Compare>
struct peque {
  using type = T;
  using compare = Compare;

private:
  std::vector<T> container;

  void heapify_down(size_t index) {
    auto target_index = index;
    auto left = 2 * index + 1;
    auto right = left + 1;

    auto adjust = [&](auto index) {   
      if (index < container.size() && compare{}(container[index], container[target_index])) {
        target_index = index;
      }
    };

    adjust(left);
    adjust(right);

    if (index != target_index) {
      std::swap(container[index], container[target_index]);
      heapify_down(target_index);
    }
    // else the item at index is correctly ordered.
  }

  void heapify_up(size_t index) {
    while (index > 0) {
      auto parent = (index - 1) / 2;
      if (compare{}(container[index], container[parent])) {
        std::swap(container[index], container[parent]);
        index = parent;
      } else {
        return;
      }
    }
  }

public:
  T& front() {
    if (container.empty()) {
      throw "empty peque";
    }
    return container.front();
  }

  const T& front() const {
    if (container.empty()) {
      throw "empty peque";
    }
    return container.front();
  }

  bool empty() const {
    return container.empty();
  }

  void pop() {
    if (container.empty()) {
      return;
    }

    if (container.size() == 1) {
      container.pop_back();
      return;
    }

    std::swap(container[0], container[container.size() - 1]);
    container.pop_back();

    heapify_down(0);
  }

  void push(T value) {
    container.push_back(std::move(value));
    heapify_up(container.size() - 1);
  }
};

template <typename T>
struct my_vector {
  using type = T;

private:
  size_t length{0};
  size_t reserved{16};
  std::unique_ptr<T[]> backing{new T[reserved]};

public:
  T& front() {
    if (length == 0) {
      throw "empty";
    }
    return backing.get()[0];
  }

  const T& front() const {
    if (length == 0) {
      throw "empty";
    }
    return backing.get()[0];
  }

  T& back() {
    if (length == 0) {
      throw "empty";
    }
    return backing.get()[length - 1];
  }
  
  const T& back() const {
    if (length == 0) {
      throw "empty";
    }
    return backing.get()[length - 1];
  }

  T& at(size_t index) {
    if (length == 0) {
      throw "empty";
    } 
    if (index >= length) {
      throw "out of bounds";
    }
    return backing.get()[index];
  }
  
  const T& at(size_t index) const {
    if (length == 0) {
      throw "empty";
    }
    if (index >= length) {
      throw "out of bounds";
    }
    return backing.get()[index];
  }

  size_t size() const { return length; }
  bool is_empty() const { return length == 0; }

  void push(T value) {
    if (length + 1 >= reserved) {
      reserved *= 2;
      auto new_backing = std::unique_ptr<T[]>{new T[reserved]};
      std::memcpy(new_backing.get(), backing.get(), length * sizeof(T));
      backing = std::move(new_backing);
    }

    ++length;
    backing.get()[length - 1] = std::move(value);
  }
};


int main (int argc, char *argv[])
{
  auto my_v = my_vector<uint32_t>{};
  for (auto i = size_t{0}; i < 73; ++i) {
    my_v.push(i);
  }
  for (auto i = size_t{0}; i < my_v.size(); ++i) {
    std::cout << my_v.at(i) << std::endl;
  }
  std::cout << std::endl << std::endl;


  auto test_peque = [](auto compare) {  
    std::cout << "Entering test..." << std::endl;
    auto p = peque<uint32_t, std::decay_t<decltype(compare)>>{};
    std::cout << "Pushing..." << std::endl;
    p.push(10);
    p.push(8);
    p.push(6);
    p.push(4);
    p.push(3);
    p.push(5);
    p.push(7);
    p.push(9);
    p.push(1);
    p.push(2);

    std::cout << "Printing..." << std::endl;
    while (!p.empty()) {
      auto front = p.front();
      p.pop();
      std::cout << front << std::endl;
    }
    std::cout << "Exiting test..." << std::endl;
  };

  std::cout << "less..." << std::endl;
  test_peque(less_<uint32_t>{});
  std::cout << "greater..." << std::endl;
  test_peque(greater_<uint32_t>{});

  return 0;


  auto t = std::map<std::string, uint32_t>{
    {"z", 0},
    {"y", 0},
    {"x", 0},
    {"w", 0},
    {"v", 0},
    {"u", 0},
    {"t", 0},
    {"s", 0},
    {"r", 0},
    {"q", 0},
    {"ddddddddd", 0},
    {"o", 0},
    {"n", 0},
    {"m", 0},
    {"l", 0},
    {"k", 0},
    {"aaaaaa", 0},
    {"i", 0},
    {"h", 0},
    {"g", 0},
    {"f", 0},
    {"e", 0},
    {"d", 0},
    {"c", 0},
    {"b", 0},
    {"a", 0},
  };

  for (auto& it : t) {
    std::cout << it.first << std::endl;
  }
}
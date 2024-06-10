#include <iostream>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <vector>

// #include <training/huffman.hpp>
#include <training/bounded_buffer.hpp>
#include <training/dp.hpp>
#include <training/fibonacci.hpp>
#include <training/sort.hpp>
#include <training/tree.hpp>
#include <training/moving_average.hpp>
#include <training/near_interview.hpp>

#include <training/range_response_processor.hpp>

#include <training/mev.hpp>

#include <training/millennium.hpp>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
  out << "[";
  for (const auto& e : v) {
    out << std::to_string(e) << ", ";
  }
  out << "]";
  return out;
}

void run_tree_tests()
{
  std::cout << "Running tree tests...\n"; 
  auto values = std::vector<char>{'F', 'B', 'G', 'A', 'D', '\0', 'I', '\0', '\0', 'C', 'E', 'H', '\0'};
  auto tree = training::tree<char>{values};
  auto print = [](auto& data) {
    std::cout << data;
    return false;
  };
  std::cout << "Breadth First: ";
  tree.traverse(training::tree<char>::traversal::breadth_first, print);
  std::cout << "\nDepth First, Pre Order: ";
  tree.traverse(training::tree<char>::traversal::depth_first_pre_order, print);
  std::cout << "\nDepth First, In Order: ";
  tree.traverse(training::tree<char>::traversal::depth_first_in_order, print);
  std::cout << "\nDepth First, Post Order: ";
  tree.traverse(training::tree<char>::traversal::depth_first_post_order, print);
  std::cout << "\nFinished tree tests...\n";
}

void run_fibonnaci_tests()
{
  std::cout << "Running Fibonacci tests...\n";
  std::cout << std::to_string(training::fibonacci<5>()) << "\n";
  std::cout << std::to_string(training::fibonacci(5)) << "\n";
  std::cout << std::to_string(training::fibonacci2(5)) << "\n";
  std::cout << std::to_string(training::fibonacci3(5)) << "\n";
  std::cout << std::to_string(training::fibonacci<10>()) << "\n";
  std::cout << std::to_string(training::fibonacci(10)) << "\n";
  std::cout << std::to_string(training::fibonacci2(10)) << "\n";
  std::cout << std::to_string(training::fibonacci3(10)) << "\n";
  std::cout << std::to_string(training::fibonacci<12>()) << "\n";
  std::cout << std::to_string(training::fibonacci(12)) << "\n";
  std::cout << std::to_string(training::fibonacci2(12)) << "\n";
  std::cout << std::to_string(training::fibonacci3(12)) << "\n";
  std::cout << std::to_string(training::fibonacci<50>()) << "\n";
  std::cout << std::to_string(training::fibonacci(50)) << "\n";
  // Significant time to compute
  // std::cout << std::to_string(training::fibonacci2(50)) << "\n";
  std::cout << std::to_string(training::fibonacci3(50)) << "\n";
  std::cout << "Finished Fibonacci tests...\n";
}

void run_bubble_sort_tests()
{
  std::cout << "Running BubbleSort tests...\n";
  auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
  std::cout << "Unsorted: " << s << "\n";
  training::sort::bubble::sort(s);
  std::cout << "Sorted: " << s << "\n";
  std::cout << "Finished BubbleSort tests...\n";
}

void run_selection_sort_tests()
{
  std::cout << "Running SelectionSort tests...\n";
  auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
  std::cout << "Unsorted: " << s << "\n";
  training::sort::selection::sort(s);
  std::cout << "Sorted: " << s << "\n";
  std::cout << "Finished SelectionSort tests...\n";
}

void run_insertion_sort_tests()
{
  std::cout << "Running InsertionSort tests...\n";
  auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
  std::cout << "Unsorted: " << s << "\n";
  training::sort::insertion::sort(s);
  std::cout << "Sorted: " << s << "\n";
  std::cout << "Finished InsertionSort tests...\n";
}

void run_quick_sort_tests()
{
  std::cout << "Running QuickSort tests...\n";
  auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
  std::cout << "Unsorted: " << s << "\n";
  training::sort::quick::sort(s);
  std::cout << "Sorted: " << s << "\n";
  std::cout << "Finished QuickSort tests...\n";
}

void run_merge_sort_tests()
{
  std::cout << "Running MergeSort tests...\n";
  auto s = std::vector<int32_t>{1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
  std::cout << "Unsorted: " << s << "\n";
  training::sort::merge::sort(s);
  std::cout << "Sorted: " << s << "\n";
  std::cout << "Finished MergeSort tests...\n";
}

void run_moving_average_tests() 
{
  auto mv = training::moving_average{4};

  std::cout << std::to_string(mv(1)(2)(3)(4).average()) << std::endl;
  std::cout << std::to_string(mv(5)(6)(7)(8).average()) << std::endl;
}

#include <training/pinestreet.hpp>


int main (int argc, char *argv[])
{
  millennium::tests::run_tests();
  return 0;

  auto result = bullish::calculate_mev({
    {2, 50, 6},
    {1, 70, 8},
    {3, 40, 5},
    {2, 60, 10}
  }, 100);
  std::cout << std::to_string(result) << "\n";

  // pinestreet::tests::run_tests();
  // huffman::tests::run_test();

  // test_calculator();  
  // run_bounded_buffer_test();

  // run_moving_average_tests();
  // training::run_tests();

  // run_tree_tests();
  // run_fibonnaci_tests();
  // run_bubble_sort_tests();
  // run_insertion_sort_tests();
  // run_selection_sort_tests();
  // run_quick_sort_tests();
  // run_merge_sort_tests();
  return EXIT_SUCCESS;
}
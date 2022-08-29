#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>

#include <dapperlabs/range_response_processor.hpp>

// Command line arguments
// -n Number of of available nodes.
// -t The threshold for confirming blocks.
// -i The input data file.
// -h Display the help.
int main(int argc, char* argv[]) {
   if (argc < 2 || argc % 2 == 0) {
      std::cout << "Please specify the -h argument to display the options." << std::endl;
      return EXIT_SUCCESS;
   }

   auto number_of_nodes = uint32_t{ 7 };
   auto threshold       = uint64_t{ 2000 };
   auto input_file      = std::vector<std::string>{};
   for (auto i = 1; i < argc; i += 2) {
      if (std::string{ argv[i] } == "-n") {
         number_of_nodes = std::atoi(argv[i + 1]);
      } else if (std::string{ argv[i] } == "-t") {
         threshold = std::atoi(argv[i + 1]);
      } else if (std::string{ argv[i] } == "-i") {
         input_file.emplace_back(argv[i + 1]);
      } else if (std::string{ argv[i] } == "-h") {
         std::cout << "Command Line Arguments\n" <<
                      "-n <The number of available nodes>\n" <<
                      "-t <The threshold for confirming blocks>\n" <<
                      "-i <The input data file, you can specify as many as these as you want>\n" <<
                      "-h <Display help>\n";
         return EXIT_SUCCESS;
      }
   }

   auto blocks = std::vector<std::vector<dapperlabs::block>>{};
   while (!input_file.empty()) {
      auto is = std::ifstream{ input_file.back() };
      input_file.pop_back();
      auto start = std::istream_iterator<std::string>{ is };
      auto end   = std::istream_iterator<std::string>{};
      blocks.emplace_back(start, end);
   }
   if (blocks.empty()) {
      blocks = std::vector<std::vector<dapperlabs::block>>{ { "a", "b", "c", "d", "e", "f", "g", "h", "i",
                                                              "j", "k", "l", "m", "n", "o", "p", "q", "r",
                                                              "s", "t", "u", "v", "w", "x", "y", "z" } };
   }

   auto write_mutex = std::mutex{};

   // The range response processor.  In this case, we are using the dispatch queue implementation.
   auto rrp = dapperlabs::range_response_processor<dapperlabs::dispatch_queue_policy>{
      threshold,
      [&](auto block_height, const auto& block) {
         auto lock = std::lock_guard<std::mutex>{ write_mutex };
         std::cout << "block " << block << " is confirmed at height " << std::to_string(block_height) << std::endl;
      }
   };
   
   // Thread functor for querying for the active range.
   auto print_active_range = [&]() {
      while (true) {
         auto range = rrp.get_active_range();
         {
            auto lock = std::lock_guard<std::mutex>{ write_mutex };
            std::cout << "min height: " << std::to_string(range.min) << " max height: " << std::to_string(range.max) << "\n";
         }
         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
   };

   // Thread functor for "processing a range".
   auto process_range = [&](const auto& block_data) {
      //while (true) {
         rrp.process_range(0, block_data);
         std::this_thread::sleep_for(std::chrono::milliseconds(300));
      //}
   };

   auto threads = std::vector<std::thread>{};
   threads.push_back(std::thread{ print_active_range });
   for (auto i = 0; i < number_of_nodes; ++i) {
      threads.push_back(std::thread{ process_range, blocks[i % blocks.size()] });
   }
   for (auto& thread : threads) { thread.join(); }

   return EXIT_SUCCESS;
}
#pragma once
#include <vector>

namespace anchorage {

// Given a binary grid[ ][ ] of size N*M, with each cell containing either 0 or 1, 
// where 1 represents an alive cell and the 0 represents a dead cell. 
// The task is to generate the next generation of cells based on the following rules:  
// 
// Any live cell with fewer than two live neighbors dies due to under-population.
// Any live cell with two or three live neighbors lives on to the next generation.
// Any live cell with more than three live neighbors dies due to overpopulation.
// Any dead cell with exactly three live neighbors becomes a live cell by reproduction.
struct game_of_life {

    using board = std::vector<std::vector<int16_t>>;

private:
    template <typename Functor>
    static void for_each(board& b, Functor f) {
        for (auto i = size_t{0}; i < b.size(); ++i) {
            auto& row = b[i];
            for (auto j = size_t{0}; j < row.size(); ++j) {
                f(b, i, j);
            }
        }
    }

    template <typename Functor>
    static void for_each(board& b, size_t i, size_t j, Functor f) {
        for (auto ii = -1; ii < 2; ++ii) {
            auto n_i = i + ii;
            if (n_i < 0 || n_i >= b.size()) {
                continue;
            }

            auto& row = b[n_i];
            for (auto jj = -1; jj < 2; ++jj) {
                auto n_j = j + jj;    
                if (n_j < 0 || n_j >= row.size()) {
                    continue;
                }   

                if (n_i == i && n_j == j) {
                    continue;
                }

                f(b, i, j, n_i, n_j);
            }
        }
    }

public:
    static void print(board& b) {
        for_each(b, [](auto& b, auto i, auto j){
            std::cout << b[i][j] << ", ";
            if (j == b[i].size() - 1) {
                std::cout << std::endl;
            }
        });
    }

    static board& iterate(board& b) {
        for_each(b, [](auto& b, auto i, auto j){
            for_each(b, i, j, [](auto& b, auto i, auto j, auto ni, auto nj) {
                if (b[ni][nj] < 1) {
                    // Dead
                    return;
                }

                auto& cell = b[i][j];
                if (cell > 0) {
                    // Keep counting up to indicate cell was originally alive.
                    ++cell;
                } else {
                    // Keep counting down to indicate cell was originally dead.
                    --cell;
                }
            });
        });

        for_each(b, [](auto& b, auto i, auto j) {
            auto& cell = b[i][j];

            if (cell > 0) {
                // Remember, the count start at 1, so we need to factor that in.
                if (cell < 3 || cell > 4) {
                    cell = 0;
                } else {
                    cell = 1;
                }
            } else if (cell == -3) {
                cell = 1;
            } else {
                cell = 0;
            }
        });

        return b;
    }

};

namespace tests {

void run_tests() {
    auto execute = [](auto board) {
        game_of_life::print(game_of_life::iterate(board));
        std::cout << std::endl;
        game_of_life::print(game_of_life::iterate(board));
        std::cout << std::endl;
        game_of_life::print(game_of_life::iterate(board));
        std::cout << std::endl;
    };

    execute(game_of_life::board {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    });

    execute(game_of_life::board {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0}, 
        {0, 0, 0, 1, 0}, 
        {0, 0, 0, 0, 0}, 
    });

    execute(game_of_life::board {
        {1, 0, 1, 1},
        {1, 1, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
    });

    /*
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 

0, 0, 0, 0, 0, 
0, 0, 1, 0, 0, 
0, 0, 1, 0, 0, 
0, 0, 0, 0, 0, 

0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 

0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 

1, 0, 1, 0, 
0, 0, 0, 1, 
0, 0, 0, 0, 
1, 1, 1, 0, 
0, 0, 0, 0, 

0, 0, 0, 0, 
0, 0, 0, 0, 
0, 1, 1, 0, 
0, 1, 0, 0, 
0, 1, 0, 0, 

0, 0, 0, 0, 
0, 0, 0, 0, 
0, 1, 1, 0, 
1, 1, 0, 0, 
0, 0, 0, 0, 
    */

// #include <fstream>
// #include <iostream>
// #include <array>
// #include <unordered_map>

// using namespace std;

// // To execute C++, please define "int main()"
// /*
// 1. We are creating a little virtual filesystem called SimpleFS. You don't have to worry about how it's laid out on disk, but you will have access to a series of blocks. Each block has a next pointer to the next block and can contain up to 8 bytes or characters of data. The filesystem has a fixed size. You can initialize it with 5 blocks, for example. **Create an instance of this filesystem.**
// 2. Write a Print() function to show it in its empty state. Use arrows to show the connections between the blocks. Don't print a trailing arrow. `[] -> [] -> []`
// 3. Create a Write(data string) function that adds to the end of the free space in the filesystem. It takes only strings that are **multiples of 8**. Handle reasonable errors:
//     1. empty write
//     2. write beyond the end
//     3. write a string that's not a multiple of 8 bytes
// 4. Add a filename parameter to Write, so it becomes Write(filename, data string). A file can be written to only once. Don’t implement the ability to append to a file.
// 5. Add a Read(filename string) function
// 6. Bonus: Implement a Delete(filename string) function; at this point Write() might not check length errors correctly
// */

// template <size_t N>
// struct simple_fs {
//   static const auto alignment = 8;
//   static_assert(N % alignment == 0, "N must be a multiple of 8");

// private:
//   std::array<char, N> buffer;
//   size_t position = 0;

//   std::unordered_map<std::string, std::pair<size_t, size_t>> files;

// public:
//   void print() {
//     for (auto i = size_t{0}; i < N; i += alignment) {
//       std::cout << "["; 
      
//       if (i < position) {
//         for(auto j = size_t{0}; j < alignment; ++j) {
//           std::cout << buffer[i + j];
//         }
//       }

//       std::cout << "]";
//       if (i != buffer.size() - alignment) {
//         std::cout << " -> ";
//       }
//     }
//     std::cout << std::endl;
//   }

//   size_t write(const std::string& filename, const std::string& data) {
//     auto it = files.find(filename);
//     if (it != std::end(files)) {
//       return 4;
//     }

//     auto current_position = position;
//     auto result = write(data);
//     if (result == 0) {
//       files.emplace(filename, std::make_pair(current_position, position));
//     }
//     return result;
//   }

//   size_t write(const std::string& data) {
//     if (data.empty()) {
//       return 1;
//     }

//     if (data.size() + position > N) {
//       return 2;
//     }

//     if (data.size() % alignment != 0) {
//       return 3;
//     }

//     for (auto i = size_t{0}; i < data.size(); ++i) {
//       buffer[position + i] = data[i];
//     }
//     position += data.size();

//     return 0;
//   }

//   std::string read(const std::string& filename) {    
//     auto it = files.find(filename);
//     if (it == std::end(files)) {
//       return {};
//     }

//     auto result = std::string{};
//     for (auto i = it->second.first; i < it->second.second; ++i) {
//       result += buffer[i];
//     }
//     return result;
//   }
// };

// int main() {
//   //auto bad_fs = simple_fs<9>{};

//   auto fs = simple_fs<48>{};
//   fs.print();

//   // auto result = fs.write({});
//   // std::cout << result << " == 1" << std::endl;
//   // result = fs.write("aaaaaaaaa");
//   // std::cout << result << " == 2" << std::endl;
//   // result = fs.write("aaaaaaa");
//   // std::cout << result << " == 3" << std::endl;
//   // result = fs.write("bbbbbbbb");
//   // std::cout << result << std::endl;
//   // result = fs.write("bbbbbbbb");
//   // std::cout << result << std::endl;
//   // fs.print();

//   // fs.write("aaaaaaaabbbbbbb")
//   auto result = fs.write("filename1", "aaaaaaaabbbbbbbb");
//   std::cout << result << " == 1" << std::endl;
//   fs.print();
//    result = fs.write("filename2", "ccccccccdddddddd");
//   std::cout << result << " == 1" << std::endl;
//    result = fs.write("filename3", "eeeeeeeeffffffff");
//   std::cout << result << " == 1" << std::endl;
//   auto b = fs.read("filename2");
//   std::cout << b << std::endl;
//   fs.print();
//   return 0;
// }

    
}
}
}

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
}
}
}
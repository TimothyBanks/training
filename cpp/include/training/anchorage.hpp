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

template <typename Functor>
static void for_each(board& b, Functor functor) {
    for (auto i = size_t{0}; i < b.size(); ++i) {
        auto& row = b[i];
        for (auto j = size_t{0}; j < row.size(); ++j) {
            functor(b, i, j);
        }
    }
};

static void print(board& b) {
    for_each(b, [](auto& bb, auto i, auto j){
        std::cout << bb[i][j] << ",";
        if (j == bb[i].size() - 1) {
            std::cout << std::endl;
        }
    });
}

static board& iterate(board& b) {
    auto for_each_neighbor = [](auto& bb, auto i, auto j, auto functor) {
        for (auto ii = -1; ii < 2; ++ii) {
            if (i + ii < 0 || i + ii >= bb.size()) {
                continue;
            }

            for (auto jj = -1; jj < 2; ++jj) {
                if (j + jj < 0 || j + jj >= bb[i].size()) {
                    continue;
                }

                if (ii == 0 && jj == 0) {
                    // This is the cell, not a neighbor
                    continue;
                }

                functor(bb, i + ii, j + jj);
            }
        }
    };

    for_each(b, [&for_each_neighbor](auto& bb, auto i, auto j) {
        for_each_neighbor(bb, i, j, [&](auto& bbb, auto ii, auto jj) {
            if (bbb[ii][jj] < 1) {
                return;
            }

            if (bbb[i][j] > 0) {
                ++(bbb[i][j]);
            } else {
                --(bbb[i][j]);
            }
        });
    });

    for_each(b, [](auto& bb, auto i, auto j) {
        if (bb[i][j] > 0 && bb[i][j] < 3) {
            bb[i][j] = 0;
        } else if(bb[i][j] > 0 && bb[i][j] <= 4) {
            bb[i][j] = 1;
        } else if (bb[i][j] > 0) {
            bb[i][j] = 0;
        } else if (bb[i][j] == -3) {
            bb[i][j] = 1;
        } else {
            bb[i][j] = 0;
        }
    });

    return b;
}

};

namespace tests {

void run_tests() {
    auto gof = game_of_life::board{
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    };

    game_of_life::print(game_of_life::iterate(gof));
    std::cout << std::endl;
    game_of_life::print(game_of_life::iterate(gof));
    std::cout << std::endl;
    game_of_life::print(game_of_life::iterate(gof));
    std::cout << std::endl;

    gof = game_of_life::board{
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0}, 
        {0, 0, 0, 1, 0}, 
        {0, 0, 0, 0, 0}, 
    };

    game_of_life::print(game_of_life::iterate(gof));
    std::cout << std::endl;
    game_of_life::print(game_of_life::iterate(gof));
    std::cout << std::endl;
    game_of_life::print(game_of_life::iterate(gof));
    std::cout << std::endl;
}
}
}
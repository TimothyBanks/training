/*
You've found yourself in a grid of cells with 
R
R rows and 
C
C columns. The cell in the 
i
ith row from the top and 
j
jth column from the left contains one of the following (indicated by the character 
G
i
,
j
G 
i,j
​
 ):
If 
G
i
,
j
G 
i,j
​
  = ".", the cell is empty.
If 
G
i
,
j
G 
i,j
​
  = "S", the cell contains your starting position. There is exactly one such cell.
If 
G
i
,
j
G 
i,j
​
  = "E", the cell contains an exit. There is at least one such cell.
If 
G
i
,
j
G 
i,j
​
  = "#", the cell contains a wall.
Otherwise, if 
G
i
,
j
G 
i,j
​
  is a lowercase letter (between "a" and "z", inclusive), the cell contains a portal marked with that letter.
Your objective is to reach any exit from your starting position as quickly as possible. Each second, you may take either of the following actions:
Walk to a cell adjacent to your current one (directly above, below, to the left, or to the right), as long as you remain within the grid and that cell does not contain a wall.
If your current cell contains a portal, teleport to any other cell in the grid containing a portal marked with the same letter as your current cell's portal.
Determine the minimum number of seconds required to reach any exit, if it's possible to do so at all. If it's not possible, return 
−
1
−1 instead.
Constraints
1
≤
R
,
C
≤
5
0
1≤R,C≤50
G
i
,
j
∈
G 
i,j
​
 ∈ {".", "S", "E", "#", "a"..."z"}
Sample test case #1
R = 3
C = 3
G = .E.
    .#E
    .S#
Expected Return Value = 4
Sample test case #2
R = 3
C = 4
G = a.Sa
    ####
    Eb.b
Expected Return Value = -1
Sample test case #3
R = 3
C = 4
G = aS.b
    ####
    Eb.a
Expected Return Value = 4
Sample test case #4
R = 1
C = 9
G = xS..x..Ex
Expected Return Value = 3
Sample Explanation
In the first case, you can reach an exit in 
4
4 seconds by walking left once, then up twice, and then finally right once.
In the second case, you can never reach the exit.
In the third case, you should walk right twice, then teleport to the cell in the 3rd row and 2nd column, and finally walk left once.
In the fourth case, you should walk left once, teleport to the cell in the last column, and walk left once more.
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

struct position {
  int32_t x = 0;
  int32_t y = 0;
  uint64_t secs = 0;
  position() = default;
  position(int32_t x_, int32_t y_) : x{x_}, y{y_} {}
  position(int32_t x_, int32_t y_, uint64_t secs_) : x{x_}, y{y_}, secs{secs_} {}
};

using board_type = std::vector<std::vector<char>>;
using visited_type = std::vector<std::vector<bool>>;
using queue_type = std::deque<position>;

#define make_move(x, y, seconds, board, q, visited)  \
  if (!visited[x][y] && board[x][y] != '#') { \
    q.emplace_back(x, y, seconds); \
  } 

int getSecondsRequired(int R, int C, vector<vector<char>> G) {
  // Write your code here
  static auto visited = visited_type(R, std::vector<bool>(C, false));
  static auto portals = std::vector<std::vector<position>>{26, std::vector<position>{}};
  static auto q       = queue_type{};
  
  auto x = int32_t{-1};
  auto y = int32_t{-1};
  
  for (auto i = size_t{0}; i < R; ++i) {
    for (auto j = size_t{0}; j < C; ++j) {
      if (G[i][j] >= 'a' && G[i][j] <= 'z') {
        portals[G[i][j] - 'a'].emplace_back(i, j);
      } else if (G[i][j] == 'S') {
        x = i; 
        y = j;
      }
    }
  }
  
  if (x == -1 || y == -1) {
    return -1;
  }
  
  auto min_seconds = static_cast<uint64_t>(-1);
  make_move(x, y, 0, G, q, visited);
  
  while (!q.empty()) {
    auto& front = q.front();
    auto& cell = G[front.x][front.y];
    if (cell == 'E') {
      min_seconds = std::min(min_seconds, front.secs);
      q.pop_front();
      continue;
    } 
    visited[front.x][front.y] = true;
    auto next_secs = front.secs + 1;
    if (front.x > 0) {
      make_move(front.x - 1, front.y, next_secs, G, q, visited);
    }
    if (front.y > 0) {
      make_move(front.x, front.y - 1, next_secs, G, q, visited);
    }
    if (front.x + 1 < R) {
      make_move(front.x + 1, front.y, next_secs, G, q, visited);
    }
    if (front.y + 1 < C) {
      make_move(front.x, front.y + 1, next_secs, G, q, visited);
    }
    if (cell >= 'a' && cell <= 'z') {
      for (auto& p : portals[cell - 'a']) {
        make_move(p.x, p.y, next_secs, G, q, visited);
      }
      // Clear this so we don't do redundant checks after this portal
      portals[cell - 'a'].clear();
    }
    q.pop_front();
  }
  
  return min_seconds == static_cast<size_t>(-1) ? -1 : min_seconds;
}

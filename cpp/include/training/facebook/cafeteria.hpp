/*
A cafeteria table consists of a row of 
N
N seats, numbered from 
1
1 to 
N
N from left to right. Social distancing guidelines require that every diner be seated such that 
K
K seats to their left and 
K
K seats to their right (or all the remaining seats to that side if there are fewer than 
K
K) remain empty.
There are currently 
M
M diners seated at the table, the 
i
ith of whom is in seat 
S
i
S 
i
​
 . No two diners are sitting in the same seat, and the social distancing guidelines are satisfied.
Determine the maximum number of additional diners who can potentially sit at the table without social distancing guidelines being violated for any new or existing diners, assuming that the existing diners cannot move and that the additional diners will cooperate to maximize how many of them can sit down.
Please take care to write a solution which runs within the time limit.
Constraints
1
≤
N
≤
1
0
1
5
1≤N≤10 
15
 
1
≤
K
≤
N
1≤K≤N
1
≤
M
≤
5
0
0
,
0
0
0
1≤M≤500,000
M
≤
N
M≤N
1
≤
S
i
≤
N
1≤S 
i
​
 ≤N
Sample test case #1
N = 10
K = 1
M = 2
S = [2, 6]
Expected Return Value = 3
Sample test case #2
N = 15
K = 2
M = 3
S = [11, 6, 14]
Expected Return Value = 1
Sample Explanation
In the first case, the cafeteria table has 
N
=
1
0
N=10 seats, with two diners currently at seats 
2
2 and 
6
6 respectively. The table initially looks as follows, with brackets covering the 
K
=
1
K=1 seat to the left and right of each existing diner that may not be taken.
  1 2 3 4 5 6 7 8 9 10
  [   ]   [   ]
Three additional diners may sit at seats 
4
4, 
8
8, and 
1
0
10 without violating the social distancing guidelines.
In the second case, only 
1
1 additional diner is able to join the table, by sitting in any of the first 
3
3 seats.
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

long long getMaxAdditionalDinersCount(long long N, long long K, int M, vector<long long> S) {
  // Write your code here
  std::sort(std::begin(S), std::end(S));
  
  auto additional = size_t{(S[0] - 1) / (K + 1)};
  
  for (auto i = size_t{1}; i < S.size(); ++i) {
    auto gap = S[i] - S[i - 1] - 1;
    if (gap <= 2 * K) {
      continue;
    }
    if (gap == 2 * K + 1) {
      additional += 1;
      continue;
    }
    additional += (gap - K) / (K + 1);
  }
  
  if (S.back() != N) {
    additional += (N - S.back()) / (K + 1);
  }
  
  return additional;
}

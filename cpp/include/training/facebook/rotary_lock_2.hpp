/* 
You're trying to open a lock. The lock comes with two wheels, each of which has the integers from 
1
1 to 
N
N arranged in a circle in order around it (with integers 
1
1 and 
N
N adjacent to one another). Each wheel is initially pointing at 
1
1.
For example, the following depicts the lock for 
N
=
1
0
N=10 (as is presented in the second sample case).

It takes 
1
1 second to rotate a wheel by 
1
1 unit to an adjacent integer in either direction, and it takes no time to select an integer once a wheel is pointing at it.
The lock will open if you enter a certain code. The code consists of a sequence of 
M
M integers, the 
i
ith of which is 
C
i
C 
i
​
 . For each integer in the sequence, you may select it with either of the two wheels. Determine the minimum number of seconds required to select all 
M
M of the code's integers in order.
Constraints
3
≤
N
≤
1
,
0
0
0
,
0
0
0
,
0
0
0
3≤N≤1,000,000,000
1
≤
M
≤
3
,
0
0
0
1≤M≤3,000
1
≤
C
i
≤
N
1≤C 
i
​
 ≤N
Sample test case #1
N = 3
M = 3
C = [1, 2, 3]
Expected Return Value = 2
Sample test case #2
N = 10
M = 4
C = [9, 4, 4, 8]
Expected Return Value = 6
Sample Explanation
In the first case, there are 
3
3 integers on the locks, and the sequence of integers to be selected is 
[
1
,
2
,
3
]
[1,2,3]. One optimal way to enter the code is: select 
1
1 on the first lock 
→
→ rotate the first lock to 
2
2 (
1
1 second) 
→
→ select 
2
2 
→
→ rotate the second lock from 
1
1 backwards to 
3
3 (
1
1 second) 
→
→ select 
3
3. The total time taken is 
1
+
1
=
2
1+1=2 seconds.
In the second case, the locks each consists of the integers 
1
1 through 
1
0
10, and the sequence to be selected is 
[
9
,
4
,
4
,
8
]
[9,4,4,8]. One optimal way to enter the code is: rotate the first lock from 
1
1 backwards to 
9
9 (
2
2 seconds) 
→
→ select 
9
9 
→
→ rotate the second lock forwards from 
1
1 to 
4
4 (
3
3 seconds) 
→
→ select 
4
4 twice 
→
→ rotate the first lock from 
9
9 backwards to 
8
8 (
1
1 second) 
→
→ select 
8
8. The total time taken is 
2
+
3
+
1
=
6
2+3+1=6 seconds.
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

long long getMinCodeEntryTime(int N, int M, vector<int> C) {
  if (C.empty()) {
    return 0;
  }
  
  auto cost = [&](auto dial, auto target) {
    int cwd = (target - dial + N) % N;
    int ccwd = (dial - target + N) % N;
    return std::min(cwd, ccwd);
  };
  
  // Store the current cost of each state.
  auto state_cost = vector<long long>{};

  // Base case: Generate state 0.
  state_cost.push_back(cost(1, C[0]));

  // Step 1 to M.
  for (int i = 1; i < M; ++i) {
      // At every step, a new state, state i is generated.
      // There are now a total of i + 1 states, and i ways of generating state i.
      // Compare all the possibilities of generating state i and use the cheapest way.

      // 1st way: Generate state i from state 0.
      state_cost.push_back(state_cost[0] + cost(1, C[i]));
      // 2nd to ith way: Generate state i from state 1 to state i-1.
      for (int j = 1; j < i; ++j) {
          state_cost[i] = std::min<long long>(state_cost[i], state_cost[j] + cost(C[j - 1], C[i]));
      }

      // For the i older states, we need to update their costs.
      for (int j = 0; j < i; ++j) {
          state_cost[j] += cost(C[i - 1], C[i]);
      }
  }

  return *std::min_element(state_cost.begin(), state_cost.end());
}

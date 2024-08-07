/*
Note: Chapter 2 is a harder version of this puzzle.
You're trying to open a lock. The lock comes with a wheel which has the integers from 
1
1 to 
N
N arranged in a circle in order around it (with integers 
1
1 and 
N
N adjacent to one another). The wheel is initially pointing at 
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
1 second to rotate the wheel by 
1
1 unit to an adjacent integer in either direction, and it takes no time to select an integer once the wheel is pointing at it.
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
 . Determine the minimum number of seconds required to select all 
M
M of the code's integers in order.
Please take care to write a solution which runs within the time limit.
Constraints
3
≤
N
≤
5
0
,
0
0
0
,
0
0
0
3≤N≤50,000,000
1
≤
M
≤
1
,
0
0
0
1≤M≤1,000
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
Expected Return Value = 11
Sample Explanation
In the first case, there are 
3
3 integers on the lock, and the sequence of integers to be selected is 
[
1
,
2
,
3
]
[1,2,3]. One optimal way to enter the code is: select 
1
1 
→
→ rotate to 
2
2 (
1
1 second) 
→
→ select 
2
2 
→
→ rotate to 
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
In the second case, the lock consists of the integers 
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
[9,4,4,8]. One optimal way to enter the code is: rotate from 
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
→ rotate forwards to 
4
4 (
5
5 seconds) 
→
→ select 
4
4 twice 
→
→ rotate forwards to 
8
8 (
4
4 seconds) 
→
→ select 
8
8. The total time taken is 
2
+
5
+
4
=
1
1
2+5+4=11 seconds.
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

long long getMinCodeEntryTime(int N, int M, vector<int> C) {
  // Write your code here
  if (C.empty()) {
    return 0;
  }
  
  auto current_position = 1;
  uint64_t count = 0;
  for(auto target : C) {
    auto clockwise_distance = std::abs((target - current_position + N) % N);
    auto counterclockwise_distance = std::abs((current_position - target + N) % N);
    
    count += std::min(clockwise_distance, counterclockwise_distance);
    current_position = target;
  }
  
  return count;
}

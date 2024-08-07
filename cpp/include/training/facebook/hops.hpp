/*
A family of frogs in a pond are traveling towards dry land to hibernate. They hope to do so by hopping across a trail of 
N
N lily pads, numbered from 
1
1 to 
N
N in order.
There are 
F
F frogs, numbered from 
1
1 to 
F
F. Frog 
i
i is currently perched atop lily pad 
P
i
P 
i
​
 . No two frogs are currently on the same lily pad. Lily pad 
N
N is right next to the shore, and none of the frogs are initially on lily pad 
N
N.
Each second, one frog may hop along the trail towards lily pad 
N
N. When a frog hops, it moves to the nearest lily pad after its current lily pad which is not currently occupied by another frog (hopping over any other frogs on intermediate lily pads along the way). If this causes it to reach lily pad 
N
N, it will immediately exit onto the shore. Multiple frogs may not simultaneously hop during the same second.
Assuming the frogs work together optimally when deciding which frog should hop during each second, determine the minimum number of seconds required for all 
F
F of them to reach the shore.
Constraints
2
≤
N
≤
1
0
1
2
2≤N≤10 
12
 
1
≤
F
≤
5
0
0
,
0
0
0
1≤F≤500,000
1
≤
P
i
≤
N
−
1
1≤P 
i
​
 ≤N−1
Sample test case #1
N = 3
F = 1
P = [1]
Expected Return Value = 2
Sample test case #2
N = 6
F = 3
P = [5, 2, 4]
Expected Return Value = 4
Sample Explanation
In the first case, there are 
3
3 lily pads and 
1
1 frog. The frog is initially atop lily pad 
1
1 and will take 
2
2 hops to reach lily pad 
3
3.
In the second case, there are 
6
6 lily pads, with frogs on lily pads 
5
5, 
2
2, and 
4
4. Initially the lily pads and frog numbers can be represented as .2.31.
One optimal sequence of hops is:
Frog 
2
2 hops forward to lily pad 
3
3: 
..231.
Frog 
2
2 hops over frogs 
1
1 and 
3
3, onto lily pad 
6
6 and exiting onto the shore: 
...31.
Frog 
3
3 hops over frog 
1
1, onto lily pad 
6
6 and exiting onto the shore: 
....1.
Frog 
1
1 hops onto lily pad 
6
6, exiting onto the shore.
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

long long getSecondsRequired(long long N, int F, vector<long long> P) {
  // Write your code here
  std::sort(std::begin(P), std::end(P));
 
  auto hops = static_cast<long long>(0);
  for (auto& p : P) {
    hops = std::max(hops, N - p);
  }
  
  return hops;
}

/*
You're having a grand old time clicking through the rabbit hole that is your favorite online encyclopedia.
The encyclopedia consists of 
N
N different web pages, numbered from 
1
1 to 
N
N. Each page 
i
i contains nothing but a single link to a different page 
L
i
L 
i
​
 .
A session spent on this website involves beginning on one of the 
N
N pages, and then navigating around using the links until you decide to stop. That is, while on page 
i
i, you may either move to page 
L
i
L 
i
​
 , or stop your browsing session.
Assuming you can choose which page you begin the session on, what's the maximum number of different pages you can visit in a single session? Note that a page only counts once even if visited multiple times during the session.
Constraints
2
≤
N
≤
5
0
0
,
0
0
0
2≤N≤500,000
1
≤
L
i
≤
N
1≤L 
i
​
 ≤N
L
i
≠
i
L 
i
​
 ≠i
Sample test case #1
N = 4
L = [4, 1, 2, 1]
Expected Return Value = 4
Sample test case #2
N = 5
L = [4, 3, 5, 1, 2]
Expected Return Value = 3
Sample test case #3
N = 5
L = [2, 4, 2, 2, 3]
Expected Return Value = 4
Sample Explanation
In the first case, you can visit all 
4
4 pages in a single browsing session if you begin on page 
3
3. For example, you can visit the sequence of pages 
3
→
2
→
1
→
4
3→2→1→4.
In the second case, you can only visit at most 
3
3 different pages 
−
− for example, the sequence of pages 
3
→
5
→
2
3→5→2.
In the third case, you can only visit at most 
4
4 different pages 
−
− for example, the sequence of pages 
5
→
3
→
2
→
4
5→3→2→4.
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

int getMaxVisitableWebpages(int N, vector<int> L) {
  // Write your code here
  auto dp = std::vector<int64_t>(N, 0);
  
  for (auto i = int{0}; i < N; ++i) {  
    if (dp[i] > 0) {
      continue;
    }
    
    auto next = i + 1;
    auto count = int64_t{0};
    auto visited = std::set<int>{};
    auto insertion_order = std::vector<int>{};
    
    while (true) {
      auto index = next - 1;
     
      if (dp[index] > 0) {
        // Already solved from here.  Use those results.
        count += dp[index];
        break;
      }
 
      insertion_order.emplace_back(index);
      if (visited.count(index)) {
        // This identifies the cycle.
        break;
      }
      
      if (insertion_order.size() >= N) {
        return N;
      }
      
      visited.emplace(index);
      ++count;
      next = L[index];
    }
    
    auto in_cycle = false;
    //std::cout << "------------------" << std::endl;
    for (auto j = 0; j < insertion_order.size(); ++j) {
      auto& v = insertion_order[j];
      if (v == insertion_order.back()) {
        in_cycle = true;
      }
      //std::cout << v + 1 << ", " << count << std::endl;
      dp[v] = count;
      if (!in_cycle) {
        --count;
      }
    }
  }
  
  return *std::max_element(std::begin(dp), std::end(dp));
}

/*
A positive integer is considered uniform if all of its digits are equal. For example, 
2
2
2
222 is uniform, while 
2
2
3
223 is not.
Given two positive integers 
A
A and 
B
B, determine the number of uniform integers between 
A
A and 
B
B, inclusive.
Please take care to write a solution which runs within the time limit.
Constraints
1
≤
A
≤
B
≤
1
0
1
2
1≤A≤B≤10 
12
 
Sample test case #1
A = 75
B = 300
Expected Return Value = 5
Sample test case #2
A = 1
B = 9
Expected Return Value = 9
Sample test case #3
A = 999999999999
B = 999999999999
Expected Return Value = 1
Sample Explanation
In the first case, the uniform integers between 
7
5
75 and 
3
0
0
300 are 
7
7
77, 
8
8
88, 
9
9
99, 
1
1
1
111, and 
2
2
2
222.
In the second case, all 
9
9 single-digit integers between 
1
1 and 
9
9 (inclusive) are uniform.
In the third case, the single integer under consideration (
9
9
9
,
9
9
9
,
9
9
9
,
9
9
9
999,999,999,999) is uniform.    
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

int getUniformIntegerCountInInterval(long long A, long long B) {
  // Write your code here
  auto numbers = std::vector<uint64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9};
  
  auto adjustment = [](auto n) {
    auto result = uint64_t{1};
    while (n / 10 > 0) {
      result = result * 10 + 1;
      n /= 10;
    }
    return result;
  };
  
  auto adjust = adjustment(A);
  numbers[0] = adjust;
  for (auto i = size_t{1}; i < numbers.size(); ++i) {
    numbers[i] = (i + 1) * numbers[0];
   // std::cout << numbers[i] << std::endl;
  }
  
  auto count = int{0};
  auto keep_going = true;
  while (keep_going) {
    for (auto i = size_t{0}; i < numbers.size(); ++i) {
      auto& n = numbers[i];
       
      if (n > B) {
        keep_going = false; 
        break;
      }
      
      if (n >= A) {
        ++count;
      }
      
      if (i == 0) {
        n = n * 10 + 1;
      } else {
        n = (i + 1) * numbers[0];
      }
    }
  }
   
  return count;
}

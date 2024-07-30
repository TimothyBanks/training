/*
A photography set consists of 
N
N cells in a row, numbered from 
1
1 to 
N
N in order, and can be represented by a string 
C
C of length 
N
N. Each cell 
i
i is one of the following types (indicated by 
C
i
C 
i
​
 , the 
i
ith character of 
C
C):
If 
C
i
C 
i
​
  = “P”, it is allowed to contain a photographer
If 
C
i
C 
i
​
  = “A”, it is allowed to contain an actor
If 
C
i
C 
i
​
  = “B”, it is allowed to contain a backdrop
If 
C
i
C 
i
​
  = “.”, it must be left empty
A photograph consists of a photographer, an actor, and a backdrop, such that each of them is placed in a valid cell, and such that the actor is between the photographer and the backdrop. Such a photograph is considered artistic if the distance between the photographer and the actor is between 
X
X and 
Y
Y cells (inclusive), and the distance between the actor and the backdrop is also between 
X
X and 
Y
Y cells (inclusive). The distance between cells 
i
i and 
j
j is 
∣
i
−
j
∣
∣i−j∣ (the absolute value of the difference between their indices).
Determine the number of different artistic photographs which could potentially be taken at the set. Two photographs are considered different if they involve a different photographer cell, actor cell, and/or backdrop cell.
Constraints
1
≤
N
≤
3
0
0
,
0
0
0
1≤N≤300,000
1
≤
X
≤
Y
≤
N
1≤X≤Y≤N
Sample test case #1
N = 5
C = APABA
X = 1
Y = 2
Expected Return Value = 1
Sample test case #2
N = 5
C = APABA
X = 2
Y = 3
Expected Return Value = 0
Sample test case #3
N = 8
C = .PBAAP.B
X = 1
Y = 3
Expected Return Value = 3
Sample Explanation
In the first case, the absolute distances between photographer/actor and actor/backdrop must be between 
1
1 and 
2
2. The only possible photograph that can be taken is with the 
3
3 middle cells, and it happens to be artistic.
In the second case, the only possible photograph is again taken with the 
3
3 middle cells. However, as the distance requirement is between 
2
2 and 
3
3, it is not possible to take an artistic photograph.
In the third case, there are 
4
4 possible photographs, illustrated as follows:
.P.A...B
.P..A..B
..BA.P..
..B.AP..
All are artistic except the first, where the actor and backdrop exceed the maximum distance of 
3
3.
*/

#include <bits/stdc++.h>
using namespace std;
// Write any include statements here

long long getArtisticPhotographCount(int N, string C, int X, int Y) {
    long long count = 0;

    // Sliding window from left to right to count photographers
    std::vector<long long> left_photographers(N, 0);
    std::vector<long long> left_backdrops(N, 0);
    long long photographers = 0;
    long long backdrops = 0;

    for (int i = 0; i < N; ++i) {
        if (i > 0) {
            left_photographers[i] = left_photographers[i - 1];
            left_backdrops[i] = left_backdrops[i - 1];
        }
        if (i >= X) {
            if (C[i - X] == 'P') {
                photographers++;
            } else if (C[i - X] == 'B') {
               backdrops++;
            }
        }
        if (i > Y) {
            if (C[i - Y - 1] == 'P') {
              photographers--;
            } else if(C[i - Y - 1] == 'B') {
              backdrops--;
            }
        }
        if (C[i] == 'A') {
            left_photographers[i] = photographers;
            left_backdrops[i] = backdrops;
        }
    }

    // Sliding window from right to left to count backdrops and count valid photographs
    std::vector<long long> right_backdrops(N, 0);
    std::vector<long long> right_photographers(N, 0);
    photographers = 0;
    backdrops = 0;

    for (int i = N - 1; i >= 0; --i) {
        if (i < N - 1) {
            right_backdrops[i] = right_backdrops[i + 1];
            right_photographers[i] = right_photographers[i + 1];
        }
        if (i + X < N) {
            if ( C[i + X] == 'B') {
              backdrops++;
            } else if (C[i + X] == 'P') {
              photographers++;
            }
        }
        if (i + Y + 1 < N) {
            if (C[i + Y + 1] == 'B') {
              backdrops--;
            } else if (C[i + Y + 1] == 'P') {
              photographers--;
            }
        }
        if (C[i] == 'A') {
            right_backdrops[i] = backdrops;
            right_photographers[i] = photographers;
        }
    }

    for (int i = 0; i < N; ++i) {
        if (C[i] == 'A') {
            count += left_photographers[i] * right_backdrops[i];
            count += right_photographers[i] * left_backdrops[i];
        }
    }

    return count; 
}

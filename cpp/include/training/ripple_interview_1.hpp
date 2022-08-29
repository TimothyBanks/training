#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

unsigned int trapped_water(const std::vector<unsigned int>& heights)
{
  if (heights.empty())
  {
    return 0;
  }

  unsigned int result = 0;
  for (size_t i = 1; i < heights.size() - 1; ++i)
  {
    unsigned int left_max = heights[i];
    for (size_t j = 0; j < i; ++j)
    {
      left_max = std::max(heights[j], left_max);
    }

    unsigned int right_max = heights[i];
    for (size_t j = i + 1; j < heights.size(); ++j)
    {
      right_max = std::max(heights[j], right_max);
    }

    result += (std::min(right_max, left_max) - heights[i]);    
  }
  return result;
}

// To execute C++, please define "int main()"
int main() {
  auto data = std::vector<unsigned int>{4, 2, 0, 3, 2, 5};
  // auto data = std::vector<unsigned int>{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
  std::cout << trapped_water(data) << std::endl;
  return 0;
}

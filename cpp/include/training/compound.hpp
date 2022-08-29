/*
We are working on a security system for a badged-access room in our company's building.
We want to find employees who badged into our secured room unusually often. We have an unordered list of names and entry times over a single day. Access times are given as numbers up to four digits in length using 24-hour time, such as "800" or "2250".
Write a function that finds anyone who badged into the room three or more times in a one-hour period. Your function should return each of the employees who fit that criteria, plus the times that they badged in during the one-hour period. If there are multiple one-hour periods where this was true for an employee, just return the earliest one for that employee.

badge_times = [
  ["Paul",     "1355"],
  ["Jennifer", "1910"],
  ["John",      "835"],
  ["John",      "830"],
  ["Paul",     "1315"],
  ["John",     "1615"],
  ["John",     "1640"],
  ["Paul",     "1405"],
  ["John",      "855"],
  ["John",      "930"],
  ["John",      "915"],
  ["John",      "730"],
  ["John",      "940"],
  ["Jennifer", "1335"],
  ["Jennifer",  "730"],
  ["John",     "1630"],
  ["Jennifer",    "5"]
]

Expected output (in any order)
  John:  830  835  855  915  930
  Paul: 1315 1355 1405

n: length of the badge records array
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

int main() {

  vector<pair<string, string>> badge_times = {
    {"Paul",     "1355"},
    {"Jennifer", "1910"},
    {"John",      "835"},
    {"John",      "830"},
    {"Paul",     "1315"},
    {"John",     "1615"},
    {"John",     "1640"},
    {"Paul",     "1405"},
    {"John",      "855"},
    {"John",      "930"},
    {"John",      "915"},
    {"John",      "730"},
    {"John",      "940"},
    {"Jennifer", "1335"},
    {"Jennifer",  "730"},
    {"John",     "1630"},
    {"Jennifer",    "5"}
  };

  auto user_times = std::unordered_map<std::string, std::vector<uint16_t>>{};
  for (const auto& badge_time : badge_times) {
    auto it = user_times.find(badge_time.first);
    if (it == std::end(user_times)) {
      user_times.emplace(badge_time.first, std::vector<uint16_t>{std::atoi(badge_time.second.c_str())});
    } else {
      it->second.emplace_back(std::atoi(badge_time.second.c_str()));
    }
  }

  auto normalize = [](uint16_t& time_string) {
    return std::pair(time_string / 100, time_string % 100);
  };

  auto print = [](auto& record, size_t begin, size_t end) {
    std::cout << record.first << ": ";
    for (size_t i = begin; i <= end; ++i) {
      std::cout << record.second[i] << " ";
    }
    std::cout << std::endl;
  };

  for (auto& user_time : user_times) {
    if (user_time.second.size() < 3) {
      continue;
    }
    std::sort(std::begin(user_time.second), std::end(user_time.second));

    for (size_t i = 0; i < user_time.second.size() - 2; ++i) {
      auto [hours_2, minutes_2] = normalize(user_time.second[i + 2]);
      auto [hours_1, minutes_1] = normalize(user_time.second[i]);

      if (hours_2 - hours_1 > 1) {
        // Move onto the next window
        continue;
      }
      
      if (hours_2 == hours_1 || minutes_2 <= minutes_1) {
        print(user_time, i, i + 2);
        break;
      }
    }
  }
  return 0;
}
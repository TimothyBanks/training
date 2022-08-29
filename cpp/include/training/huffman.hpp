#include <string>

namespace huffman {
bool huffman(const std::string& s) {
  if (s.empty()) {
    return false;
  }

  auto encoded = std::string{};
  auto previous = s[0];
  auto count = size_t{1};
  for (size_t i = 1; i < s.size(); ++i) {
    if (previous == s[i]) {
      ++count;
      continue;
    }
    encoded += previous + std::to_string(count);
    previous = s[i];
    count = 1;
  }
  encoded += previous + std::to_string(count);
  std::cout << encoded << " ";

  return encoded.size() < s.size();
}
namespace tests
{
void run_test()
{
  // “aaabbcc” -> “a3b2c2” : True 
  // “aaabcaa” -> “a3b1c1a2” : False

  auto test = [](const std::string& s) {
    std::cout << s << " " << std::to_string(huffman(s)) << std::endl;
  };

  test("aaabbcc");
  test("aaabcaa");
  test(std::string{});
  test("zbbmmmmmmlaa");
  test("aabbc");
  test("aabbcc");
  test("z");
}
}
}



struct Owner {

};

struct Pet {
  Owner owner;
};

struct Cat : public Pet {

};

struct Dog : public Pet {

};

struct Bird : public Pet {

};

struct Robin : public Bird {

};

struct Appointment {
  timestamp time;
  Pet* pet;
};


POST /base-url/<owner>/register-pet
{
  "type": "dog",
  "name": "",
}

POST /base-url/<pet>/update-appointment
{
  
}
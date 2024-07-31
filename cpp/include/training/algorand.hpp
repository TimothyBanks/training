#include <functional>
#include <iostream>
using namespace std;

#include <memory>

class list {
  struct node;
  using node_ptr = std::shared_ptr<node>;
  struct node {
    node(std::string _data_) : data{std::move(_data_)} {
      hash = std::hash<std::string>{}(data);
    }

    bool valid() {
      std::cout << "hash: " << hash
                << ", hash2: " << std::hash<std::string>{}(data) << std::endl;
      return std::hash<std::string>{}(data) == hash;
    }

    node_ptr next;
    size_t hash{0};
    std::string data;
  };

 public:
  void push(std::string data) {
    auto n = std::make_shared<node>(std::move(data));
    if (!m_head) {
      m_head = n;
      return;
    }
    n->next = m_head;
    m_head = n;
  }

  std::string pop() {
    if (!m_head) {
      return {};
    }
    auto data = m_head->data;
    m_head = m_head->next;
    return data;
  }

  bool empty() const { return !m_head; }

  void tamper(uint32_t d, std::string data) {
    auto i = uint32_t{0};
    auto current = m_head;
    while (current && i < d) {
      current = current->next;
      ++i;
    }
    if (!current) {
      return;
    }
    current->data = std::move(data);
  }

  bool valid() const {
    if (!m_head) {
      return true;
    }

    auto current = m_head;
    while (current) {
      if (!current->valid()) {
        return false;
      }
      current = current->next;
    }
    return true;
  }

 private:
  node_ptr m_head;
};

// To execute C++, please define "int main()"
int main() {
  auto l = list{};
  // true
  std::cout << "empty: " << std::to_string(l.empty()) << std::endl;

  l.push("7500");

  // false
  std::cout << "empty: " << std::to_string(l.empty()) << std::endl;

  // "7500"
  std::cout << "front: " << l.pop() << std::endl;
  std::cout << "empty: " << std::to_string(l.empty()) << std::endl;

  l.push("100");
  l.push("200");
  l.push("300");

  // true
  std::cout << "valid: " << std::to_string(l.valid()) << std::endl;

  l.tamper(1, "10000");

  // false
  std::cout << "valid: " << std::to_string(l.valid()) << std::endl;

  l.tamper(1, "200");
  std::cout << "valid: " << std::to_string(l.valid()) << std::endl;

  return 0;
}

// Your previous Plain Text content is preserved below:

// Implement a linked-list which has the following functions:

//   push(string x) -- Add string value x to front of list.
//   pop() string -- Removes and return string at front of list.
//   empty() bool -- Return whether the list is empty.
//   tamper(int d, string x) -- Set value at position d to x
//   valid() bool -- Verify that the list contains its original values.

// Do not use built in data structures.

// Example usage:

// l = new list()
// // true
// print(l.empty())

// l.push("7500")

// // false
// print(l.empty())

// // "7500"
// print(l.pop())

// l.push("100")
// l.push("200")
// l.push("300")

// // true
// print(l.valid())

// l.tamper(1, "10000")
// // false
// print(l.valid())

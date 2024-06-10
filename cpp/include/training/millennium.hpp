#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>

namespace millennium {

// // write a conversion function that translates string: "abc" -> "abc";
// "ab3[cd]ef" -> abcdcdcdef"; "a2[b3[c]]" -> "abcccbccc" std::string
// decode(const std::string_view& s)
// {
//     auto parts   = std::stack<std::string>{}; // stack to store the parts of
//     the string auto counts  = std::stack<uint8_t>{};    // stack to store the
//     number of times each count should be repeated parts.emplace(""); // start
//     with an empty string

//     // Handle edge case such as no opening bracket, no closing bracket,
//     mismatch brackets, etc.

//     for (auto i = size_t{0}; i < s.size(); ++i) {
//         if (std::isdigit(s[i])) {
//             // Assumming that count is always followed by a '['
//             auto current_count = (s[i] - '0');
//             while (std::isdigit(s[i + 1])) {
//                 current_count = current_count * 10 + (s[++i] - '0');
//             }
//             counts.emplace(current_count);
//         } else if (std::isalpha(s[i])) {
//             parts.top().push_back(s[i]);
//         } else if (s[i] == '[') {
//             parts.emplace(""); // start a new part
//         } else if (s[i] == ']') {
//             auto part = parts.top();
//             parts.pop();
//             auto count = counts.top();
//             counts.pop();
//             while (count--) {
//                 parts.top().append(part);
//             }
//         }
//     }

//     return parts.top();
// }

// // Implement a hash map using smart pointers
// template <typename K, typename V>
// struct hash_map
// {
//     using key_type = K;
//     using value_type = V;

// private:
//     struct node {
//         key_type key;
//         value_type value;
//         std::unique_ptr<node> next;
//     };

//     using node_type  = std::unique_ptr<node>;
//     using table_type = std::vector<node_type>;
//     table_type table;
//     size_t buckets = 10;
//     size_t size    = 0;

//     size_t hash(const key_type& key) const {
//         return std::hash<key_type>{}(key) % buckets;
//     }

//     void insert(std::unique_ptr<node> node) {
//         auto index = hash(node->key);
//         if (!table[index]) {
//             table[index] = std::move(node);
//         } else {
//             auto* current = table[index].get();
//             while (current->next) {
//                 current = current->next.get();
//             }
//             current->next = std::move(node);
//         }
//         ++size;
//     }

//     void resize(size_t new_buckets) {
//         // When the table is resized, we need to rehash all the elements.
//         auto temp_table = table_type{};

//         // Drain the current table
//         for (auto& bucket : table) {
//             auto current = std::move(bucket);
//             while (current) {
//                 temp_table.emplace_back(std::move(current));
//                 current = std::move(temp_table.back()->next);
//                 temp_table.back()->next = nullptr;
//             }
//         }
//         table.clear();
//         table.resize(new_buckets);

//         // Drain the temp_table back into the table
//         size    = 0;
//         buckets = new_buckets;
//         for (auto& bucket : temp_table) {
//             insert(std::move(bucket));
//         }
//     }

//     template <typename Functor>
//     void for_each(const node_type& node, Functor&& f) const {
//         if (!node) {
//             return;
//         }
//         auto* current  = node.get();
//         auto* previous =  decltype(current){nullptr};
//         while (current) {
//             if (f(current, previous)) {
//                 return;
//             }
//             previous = current;
//             current  = current->next.get();
//         }
//     }

// public:
//     hash_map(size_t buckets = 10) : buckets{buckets}, table(buckets) {}
//     hash_map(const hash_map&) = delete;
//     hash_map(hash_map&&) = default;

//     hash_map& operator=(const hash_map&) = delete;
//     hash_map& operator=(hash_map&&) = default;

//     void insert(key_type key, value_type value) {
//         // If we wanted a static table, then just remove this line
//         if (size >= buckets) {
//             resize(buckets * 2);
//         }
//         insert(std::make_unique<node>(node{key, value, nullptr}));
//     }

//     void remove(const key_type& key) {
//         auto index = hash(key);

//         if (!table[index]) {
//             return;
//         }

//         for_each(table[index], [&](auto* current, auto* previous) {
//             if (current->key == key) {
//                 if (previous) {
//                     previous->next = std::move(current->next);
//                 } else {
//                     table[index] = std::move(current->next);
//                 }
//                 --size;
//                 return true;
//             }
//             return false;
//         });
//     }

//     value_type find(const key_type& key) const {
//         auto index  = hash(key);
//         auto result = value_type{};

//         if (!table[index]) {
//             return result;
//         }

//         for_each(table[index], [&](auto* current, auto* previous) {
//             if (current->key == key) {
//                 result = current->value;
//                 return true;
//             }
//             return false;
//         });

//         return result;
//     }
// };

// write a conversion function that translates string: "abc" -> "abc";
// "ab3[cd]ef" -> abcdcdcdef"; "a2[b3[c]]" -> "abcccbccc"
std::string decode(const std::string_view &s) {
  auto parts = std::stack<std::string>{};
  auto counts = std::stack<int8_t>{};
  auto paren_count = uint8_t{0};

  parts.push("");
  for (auto i = size_t{0}; i < s.size(); ++i) {
    if (std::isdigit(s[i])) {
      auto current = s[i] - '0';
      while (std::isdigit(s[i + 1]) && i < s.size()) {
        current = (current * 10) + (s[++i] - '0');
      }
      counts.push(current);
    } else if (s[i] == '[') {
      parts.push("");
    } else if (s[i] == ']') {
      auto count = counts.top();
      counts.pop();
      auto part = parts.top();
      parts.pop();
      while (count-- > 0) {
        parts.top() += part;
      }
    } else {
      parts.top() += s[i];
    }
  }

  return parts.top();
}

// Implement a hash map using smart pointers
template <typename Key, typename Value> struct hash_map {
  using key_type = Key;
  using value_type = Value;

private:
  struct node {
    node(key_type k, value_type v) : k{std::move(k)}, v{std::move(v)} {}

    key_type k;
    value_type v;
    std::unique_ptr<node> next;
  };
  using node_ptr = std::unique_ptr<node>;

  size_t bucket_count{10};
  std::vector<node_ptr> buckets{bucket_count}; // This is dynamic in nature.
  size_t node_count{0};

  size_t hash(const key_type &k) {
    return std::hash<key_type>{}(k) % bucket_count;
  }

  template <typename Functor> void for_each(node_ptr &bucket, Functor f) {
    auto *current = bucket.get();
    node *previous = nullptr;
    while (current) {
      if (f(current, previous)) {
        return;
      }
      previous = current;
      current = current->next.get();
    }
  }

  bool insert_(node_ptr ptr) {
    auto index = hash(ptr->k);

    if (buckets[index] == nullptr) {
      buckets[index] = std::move(ptr);
      ++node_count;
      return true;
    }

    auto updated = false;
    node *parent = nullptr;
    for_each(buckets[index], [&](auto *current, auto *previous) {
      parent = current;
      if (current->k == ptr->k) {
        current->v = ptr->v;
        updated = true;
        return true;
      }
      return false;
    });
    if (!updated) {
      parent->next = std::move(ptr);
      ++node_count;
    }
    return !updated;
  }

  void resize(size_t new_bucket_count) {
    auto temp = std::vector<node_ptr>{};

    for (auto &bucket : buckets) {
      if (!bucket) {
        continue;
      }
      temp.push_back(std::move(bucket));
      while (temp.back()->next) {
        auto &previous = temp.back();
        auto current = std::move(previous->next);
        previous->next = nullptr;
        temp.push_back(std::move(current));
      }
    }

    // Clear the current table.
    buckets.clear();
    buckets.resize(new_bucket_count);
    bucket_count = new_bucket_count;
    node_count = 0;

    // Rehash.
    for (auto &ptr : temp) {
      insert_(std::move(ptr));
    }
  }

public:
  bool insert(key_type k, value_type v) {
    auto result = insert_(std::make_unique<node>(std::move(k), std::move(v)));
    if (node_count > bucket_count) {
      resize(bucket_count * 2);
    }
    return result;
  }

  void remove(const key_type &k) {
    auto index = hash(k);
    if (!buckets[index]) {
      return;
    }
    for_each(buckets[index], [&](auto *current, auto *previous) {
      if (current->k == k) {
        if (!previous) {
          auto next = std::move(current->next);
          buckets[index] = std::move(next);
        } else {
          previous->next = std::move(current->next);
        }
        --node_count;
        return true;
      }
      return false;
    });
  }

  value_type &find(const key_type &k) {
    auto index = hash(k);
    if (!buckets[index]) {
      throw "key not found";
    }
    node *n = nullptr;
    for_each(buckets[index], [&](auto *current, auto *previous) {
      if (current->k == k) {
        n = current;
        return true;
      }
      return false;
    });
    if (!n) {
      throw "key not found";
    }
    return n->v;
  }
};

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

enum class traversal_type {
  breadth,
  pre_order, // node, left, right
  in_order,  // left, node, right
  post_order // left, right, node
};

template <typename Value> struct tree {
  using value_type = Value;

private:
  struct node {
    node() = default;
    node(value_type v_) : v{std::move(v_)} {}

    value_type v;
    std::shared_ptr<node> left;
    std::shared_ptr<node> right;
  };
  using node_ptr = std::shared_ptr<node>;
  node_ptr root;

  template <typename Functor> void breadth_first(Functor f) {
    if (!root) {
      return;
    }

    auto q = std::queue<node_ptr>{};
    q.push(root);

    while (!q.empty()) {
      auto current = q.front();
      q.pop();

      if (f(current->v)) {
        return;
      }

      if (current->left) {
        q.push(current->left);
      }

      if (current->right) {
        q.push(current->right);
      }
    }
  }

  template <typename Functor> void pre_order(Functor f) {
    if (!root) {
      return;
    }

    auto s = std::stack<node_ptr>{};
    auto current = root;

    while (current || !s.empty()) {
      if (current) {
        if (f(current->v)) {
          return;
        }
        if (current->right) {
          s.push(current->right);
        }
        current = current->left;
      } else {
        current = s.top();
        s.pop();
      }
    }
  }

  template <typename Functor> void in_order(Functor f) {
    if (!root) {
      return;
    }

    auto s = std::stack<node_ptr>{};
    auto current = root;

    while (current || !s.empty()) {
      if (current) {
        s.push(current);
        current = current->left;
      } else {
        current = s.top();
        s.pop();
        if (f(current->v)) {
          return;
        }
        current = current->right;
      }
    }
  }

  template <typename Functor> void post_order(Functor f) {
    if (!root) {
      return;
    }

    auto s = std::stack<node_ptr>{};
    auto current = root;
    node_ptr last_visited;

    while (current || !s.empty()) {
      if (current) {
        s.push(current);
        current = current->left;
      } else {
        auto &peek = s.top();
        if (peek->right && peek->right != last_visited) {
          current = peek->right;
        } else {
          last_visited = peek;
          if (f(peek->v)) {
            return;
          }
          s.pop();
        }
      }
    }
  }

public:
  template <typename Functor> void visit(traversal_type traversal, Functor f) {
    switch (traversal) {
    case traversal_type::breadth:
      breadth_first(std::move(f));
      break;
    case traversal_type::pre_order:
      pre_order(std::move(f));
      break;
    case traversal_type::in_order:
      in_order(std::move(f));
      break;
    case traversal_type::post_order:
      post_order(std::move(f));
      break;
    };
  };

  static tree<Value> construct(
      const std::vector<std::variant<std::monostate, value_type>> &items) {
    if (items.empty()) {
      return {};
    }

    auto create = [](const auto &element) {
      return std::visit(
          overloaded{
              [](const std::monostate &) { return std::shared_ptr<node>{}; },
              [](const value_type &v) { return std::make_shared<node>(v); },
          },
          element);
    };

    auto result = tree<Value>{};
    result.root = create(items[0]);

    auto nodes = std::queue<node_ptr>{};
    nodes.push(result.root);

    for (auto i = size_t{1}; i < items.size(); i += 2) {
      auto parent = nodes.front();
      nodes.pop();

      parent->left = create(items[i]);
      parent->right = create(items[i + 1]);

      if (parent->left) {
        nodes.push(parent->left);
      }

      if (parent->right) {
        nodes.push(parent->right);
      }
    }

    return result;
  }
};

template <typename T> struct sort {
  static std::vector<T> bubble(const std::vector<T> &v) {
    auto result = v;

    if (v.empty()) {
      return v;
    }

    auto sorted = false;
    while (!sorted) {
      sorted = true;
      for (auto i = size_t{0}; i < result.size() - 1; ++i) {
        for (auto j = i + 1; j < result.size(); ++j) {
          if (result[i] > result[j]) {
            std::swap(result[i], result[j]);
            sorted = false;
          }
        }
      }
    }

    return result;
  }

  static std::vector<T> insertion(const std::vector<T> &v) {
    auto result = v;

    if (v.empty()) {
      return v;
    }

    for (size_t i = 1; i < result.size(); ++i) {
      for (int64_t j = i; j > 0; --j) {
        if (result[j] < result[j - 1]) {
          std::swap(result[j], result[j - 1]);
        }
      }
    }

    return result;
  }

  static std::vector<T> selection(const std::vector<T> &v) {
    auto result = v;

    if (v.empty()) {
      return v;
    }

    for (auto i = size_t{0}; i < result.size() - 1; ++i) {
      auto min = i;
      for (auto j = i + 1; j < result.size(); ++j) {
        if (result[j] < result[min]) {
          min = j;
        }
      }
      std::swap(result[i], result[min]);
    }

    return result;
  }

  static std::vector<T> merge(const std::vector<T> &v) {
    if (v.size() <= 1) {
      return v;
    }

    auto left =
        merge(std::vector<T>{std::begin(v), std::begin(v) + v.size() / 2});
    auto right =
        merge(std::vector<T>{std::begin(v) + v.size() / 2, std::end(v)});
    auto result = std::vector<T>{};

    auto li = size_t{0};
    auto ri = size_t{0};

    while (li < left.size() && ri < right.size()) {
      result.push_back(left[li] < right[ri] ? left[li++] : right[ri++]);
    }

    while (li < left.size()) {
      result.push_back(left[li++]);
    }

    while (ri < right.size()) {
      result.push_back(right[ri++]);
    }

    return result;
  }

  static void quick(std::vector<T> &v, size_t begin, size_t end) {
    if (begin >= end) {
      return;
    }

    auto partition = [](auto &v, auto start, auto end) {
      auto begin = start;
      for (auto i = start; i < end; ++i) {
        if (v[i] <= v[end]) {
          std::swap(v[begin++], v[i]);
        }
      }
      // move the pivot to the its final spot
      std::swap(v[begin], v[end]);
      return begin;
    };

    partition(v, 0, end);
    quick(v, 0, begin);
    quick(v, begin + 1, end);
  }

  static std::vector<T> quick(const std::vector<T> &v) {
    auto result = v;
    quick(result, 0, result.size() - 1);
    return result;
  }
};

namespace tests {
void run_tests() {
  auto d = decode("abc");
  std::cout << d << std::endl;
  d = decode("ab3[cd]ef");
  std::cout << d << std::endl;
  d = decode("a2[b3[c]]");
  std::cout << d << std::endl;

  // prove that char* is the key in std::unordered_map<char*, ...>
  auto m = std::unordered_map<const char *, int>{};
  auto key1 = std::string{"abc"};
  auto key2 = std::string{"abc"};
  m.emplace(key1.c_str(), 1);
  m.emplace(key2.c_str(), 2);
  for (const auto &[k, v] : m) {
    std::cout << k << " " << v << std::endl;
  }

  // test hash_map
  auto h = hash_map<std::string, int>{};
  for (auto i = size_t{0}; i < 20; ++i) {
    h.insert(std::to_string(i), i);
    std::cout << h.find(std::to_string(i)) << std::endl;
  }
  for (auto i = size_t{0}; i < 20; ++i) {
    std::cout << h.find(std::to_string(i)) << std::endl;
  }

  // test tree
  auto t = tree<std::string>::construct({{"F"},
                                         {"B"},
                                         {"G"},
                                         {"A"},
                                         {"D"},
                                         {std::monostate{}},
                                         {"I"},
                                         {std::monostate{}},
                                         {std::monostate{}},
                                         {"C"},
                                         {"E"},
                                         {"H"},
                                         {std::monostate{}}});
  auto print = [](auto &v) {
    std::cout << v << ",";
    return false;
  };
  std::cout << "beadth first: ";
  t.visit(traversal_type::breadth, print);
  std::cout << std::endl;
  std::cout << "   pre order: ";
  t.visit(traversal_type::pre_order, print);
  std::cout << std::endl;
  std::cout << "    in order: ";
  t.visit(traversal_type::in_order, print);
  std::cout << std::endl;
  std::cout << "  post order: ";
  t.visit(traversal_type::post_order, print);
  std::cout << std::endl;

  auto print_array = [](const auto &v) {
    for (const auto &i : v) {
      std::cout << i << ",";
    }
  };
  std::cout << "   bubble sort: ";
  print_array(sort<uint32_t>::bubble({9, 7, 5, 3, 1, 2, 4, 6, 8, 0}));
  std::cout << std::endl;
  std::cout << "insertion sort: ";
  print_array(sort<uint32_t>::insertion({9, 7, 5, 3, 1, 2, 4, 6, 8, 0}));
  std::cout << std::endl;
  std::cout << "selection sort: ";
  print_array(sort<uint32_t>::selection({9, 7, 5, 3, 1, 2, 4, 6, 8, 0}));
  std::cout << std::endl;
  std::cout << "    merge sort: ";
  print_array(sort<uint32_t>::merge({9, 7, 5, 3, 1, 2, 4, 6, 8, 0}));
  std::cout << std::endl;
  std::cout << "    quick sort: ";
  print_array(sort<uint32_t>::quick({9, 7, 5, 3, 1, 2, 4, 6, 8, 0}));
  std::cout << std::endl;
}
} // namespace tests

} // namespace millennium
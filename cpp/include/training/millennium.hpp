#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace millennium {

// write a conversion function that translates string: "abc" -> "abc";
// "ab3[cd]ef" -> abcdcdcdef"; "a2[b3[c]]" -> "abcccbccc"
// std::string decode(const std::string_view& s) {
//   auto parts = std::stack<std::string>{};
//   auto counts = std::stack<uint8_t>{};

//   parts.emplace();
//   for (auto i = size_t{0}; i < s.size(); ++i) {
//     if (std::isdigit(s[i])) {
//       auto count = s[i] - '0';
//       while (i + 1 < s.size() && std::isdigit(s[i + 1])) {
//         count = (count * 10) + (s[++i] - '0');
//       }
//       counts.push(count);
//     } else if (s[i] == '[') {
//       parts.emplace();
//     } else if (s[i] == ']') {
//       auto count = counts.top();
//       counts.pop();
//       auto part = parts.top();
//       parts.pop();
//       while (count-- > 0) {
//         parts.top() += part;
//       }
//     } else {
//       parts.top() += s[i];
//     }
//   }
//   return parts.top();
// }

// Implement a hash map using smart pointers
// template <typename Key, typename Value>
// struct hash_map
// {
//   using key_type   = Key;
//   using value_type = Value;

// private:
//   struct node {
//     node (key_type k_, value_type v_) : k{std::move(k_)}, v{std::move(v_)} {}

//     key_type k;
//     value_type v;
//     std::unique_ptr<node> next;
//   };
//   using node_ptr = std::unique_ptr<node>;

//   size_t node_count{0};
//   size_t bucket_count{10};
//   std::vector<node_ptr> buckets{bucket_count};

//   size_t hash(const key_type& k) {
//     return std::hash<key_type>{}(k) % bucket_count;
//   }

//   template <typename Functor>
//   void for_each(node* ptr, Functor f) {
//     auto current   = ptr;
//     node* previous = nullptr;

//     while (current) {
//       if (f(current, previous)) {
//         return;
//       }
//       previous = current;
//       current  = current->next.get();
//     }
//   }

//   bool insert(node_ptr ptr) {
//     if (!ptr) {
//       return false;
//     }

//     auto index = hash(ptr->k);

//     if (!buckets[index]) {
//       buckets[index] = std::move(ptr);
//       ++node_count;
//       return true;
//     }

//     node* n    = nullptr;
//     auto found = false;
//     for_each(buckets[index].get(), [&](auto current, auto previous){
//       n = current;
//       if (current->k != ptr->k) {
//         return false;
//       }
//       found = true;
//       return true;
//     });

//     if (found) {
//       n->v = std::move(ptr->v);
//     } else {
//       n->next = std::move(ptr);
//       ++node_count;
//     }

//     return !found;
//   }

//   void resize(size_t new_bucket_count) {
//     auto temp = std::vector<node_ptr>{};

//     for (auto& bucket : buckets) {
//       if (!bucket) {
//         continue;
//       }
//       temp.push_back(std::move(bucket));
//       while (temp.back()->next) {
//         auto current = std::move(temp.back()->next);
//         temp.back()->next = nullptr;
//         temp.push_back(std::move(current));
//       }
//     }

//     bucket_count = new_bucket_count;
//     node_count   = 0;
//     buckets.clear();
//     buckets.resize(bucket_count);

//     for (auto& node : temp) {
//       insert(std::move(node));
//     }
//   }

// public:
//   bool insert(key_type k, value_type v) {
//     auto result = insert(std::make_unique<node>(std::move(k), std::move(v)));
//     if (node_count == bucket_count) {
//       resize(bucket_count * 2);
//     }
//     return result;
//   }

//   void remove(const key_type& k) {
//     auto index = hash(k);

//     if (!buckets[index]) {
//       return;
//     }

//     if (buckets[index]->k == k) {
//       buckets[index] = std::move(buckets[index]->next);
//       return;
//     }

//     for_each(buckets[index].get(), [&](auto current, auto previous){
//       if (current->k != k) {
//         return false;
//       }
//       previous->next = std::move(current->next);
//       return true;
//     });
//   }

//   value_type& find(const key_type& k) {
//     auto index = hash(k);

//     node* n = nullptr;
//     for_each(buckets[index].get(), [&](auto current, auto previous){
//       if (current->k != k) {
//         return false;
//       }
//       n = current;
//       return true;
//     });

//     if (!n) {
//       throw "key not found";
//     }
//     return n->v;
//   }
// };

// write a conversion function that translates string: "abc" -> "abc";
// "ab3[cd]ef" -> abcdcdcdef"; "a2[b3[c]]" -> "abcccbccc"
std::string decode(const std::string_view& s) {
  auto chunks = std::stack<std::string>{};
  auto counts = std::stack<uint8_t>{};

  chunks.emplace();
  for (auto i = size_t{0}; i < s.size(); ++i) {
    if (std::isdigit(s[i])) {
      auto count = s[i] - '0';
      while (i + 1 < s.size() && std::isdigit(s[i + 1])) {
        count = (count * 10) + (s[++i] - '0');
      }
      counts.push(count);
    } else if(s[i] == '[') {
      chunks.emplace();
    } else if (s[i] == ']') {
      auto chunk = chunks.top();
      chunks.pop();
      auto count = counts.top();
      counts.pop();
      while (count-- > 0) {
        chunks.top() += chunk;
      }
    } else {
      chunks.top() += s[i];
    }
  }
  return chunks.top();
}

// Implement a hash map using smart pointers
template <typename Key, typename Value>
struct hash_map {
  using key_type   = Key;
  using value_type = Value;

protected:
  struct node {
    node(key_type k_, value_type v_) : k{std::move(k_)}, v{std::move(v_)} {}

    key_type k;
    value_type v;
    std::unique_ptr<node> next;
  };
  using node_ptr = std::unique_ptr<node>;

  size_t node_count   = 0;
  size_t bucket_count = 10;
  std::vector<node_ptr> buckets{bucket_count};

  size_t hash(const key_type& k) const {
    return std::hash<key_type>{}(k) % bucket_count;
  }

  template <typename Functor>
  void for_each(node* ptr, Functor f) const {
    if (!ptr) {
      return;
    }

    auto current   = ptr;
    node* previous = nullptr;
    while (current) {
      if (f(current, previous)) {
        return;
      }
      previous = current;
      current  = current->next.get();
    }
  }

  bool insert(node_ptr ptr) {
    if (!ptr) {
      return false;
    }

    auto index = hash(ptr->k);
    auto& root = buckets[index];
    if (!root) {
      root = std::move(ptr);
      ++node_count;
      return true;
    }

    node* insertion_point = nullptr;
    auto found = false;
    for_each(root.get(), [&](auto current, auto previous){
      insertion_point = current;
      if (current->k == ptr->k) {
        found = true;
      }
      return found;
    });

    if (found) {
      insertion_point->v = std::move(ptr->v);
    } else {
      insertion_point->next = std::move(ptr);
    }

    return !found;
  }

  void resize(size_t new_bucket_count) {
    if (new_bucket_count == bucket_count) {
      return;
    }

    auto temp = std::vector<node_ptr>{};

    for (auto& bucket : buckets) {
      if (!bucket) {
        continue;
      }
      temp.push_back(std::move(bucket));
      while (temp.back()->next) {
        temp.push_back(std::move(temp.back()->next));
      }
    }

    node_count = 0;
    bucket_count = new_bucket_count;
    buckets.clear();
    buckets.resize(bucket_count);

    for (auto& bucket : temp) {
      insert(std::move(bucket));
    }
  }

public:
  bool insert(key_type k, value_type v) {
    auto result = insert(std::make_unique<node>(std::move(k), std::move(v)));
    if (node_count == bucket_count) {
      resize(2 * bucket_count);
    }
    return result;
  }

  void erase(const key_type k) {
    auto index = hash(k);

    auto& root = buckets[index];
    if (!root) {
      return;
    }

    for_each(root.get(), [&](auto current, auto previous){
      if (current->k != k) {
        return false;
      }

      if (!previous) {
        root->next = std::move(current->next);
      } else {
        previous->next = std::move(current->next);
      }

      return true;
    });
  }

  const value_type& find(const key_type& k) const {
    auto index = hash(k);

    auto& root = buckets[index];
    node* ptr  = nullptr;
    for_each(root.get(), [&](auto current, auto previous){
      if (current->k != k) {
        return false;
      }
      ptr = current;
      return true;
    });

    if (!ptr) {
      throw "key not found";
    }
    return ptr->v;
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

  // TODO:  Look at how this is done using coroutines!!!!!!
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

    auto s       = std::stack<node_ptr>{};
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

    auto s       = std::stack<node_ptr>{};
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

    auto s                = std::stack<node_ptr>{};
    auto current          = root;
    node_ptr last_visited = nullptr;

    while (current || !s.empty()) {
      if (current) {
        s.push(current);
        current = current->left;
      } else {
        auto peek = s.top();
        if (peek->right && peek->right != last_visited) {
          current = peek->right;
        } else {
          if (f(peek->v)) {
            return;
          }
          s.pop();
          last_visited = peek;
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

      auto create = [](const auto& item) {
        return std::visit(overloaded{
          [](std::monostate) { return std::shared_ptr<node>{}; },
          [](value_type v) { return std::make_shared<node>(std::move(v)); }
        }, item);
      };

      auto nodes = std::queue<node_ptr>{};
      auto t     = tree<Value>{};
      t.root     = create(items[0]);
      nodes.push(t.root);

      for (auto i = size_t{1}; i < items.size(); i+=2) {
        auto parent = nodes.front();
        nodes.pop();

        parent->left  = create(items[i]);
        parent->right = create(items[i + 1]);

        if (parent->left) {
          nodes.push(parent->left);
        }
        if (parent->right) {
          nodes.push(parent->right);
        }
      }

      return t;
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

  static std::vector<T> selection(const std::vector<T> &v) {
    auto result = v;

    if (v.empty()) {
      return v;
    }

    for (auto i = size_t{1}; i < result.size(); ++i) {
      for (auto j = i; j > 0; --j) {
        if (result[j] < result[j - 1]) {
          std::swap(result[j], result[j - 1]);
        }
      }
    }

    return result;
  }

  static std::vector<T> insertion(const std::vector<T> &v) {
    auto result = v;

    for (auto i = size_t{1}; i < result.size(); ++i) {
      for (auto j = i; j > 0; --j) {
        if (result[j] < result[j - 1]) {
          std::swap(result[j], result[j - 1]);
        }
      }
    }

    return result;
  }

  static std::vector<T> merge(const std::vector<T>& v) {
    if (v.size() < 2) {
      return v;
    } 

    auto mid = v.size() / 2;
    auto left = merge({std::begin(v), std::begin(v) + mid});
    auto right = merge({std::begin(v) + mid, std::end(v)});

    auto li = size_t{0};
    auto ri = size_t{0};

    auto result = std::vector<T>{};
    while (li < left.size() && ri < right.size()) {
      if (left[li] < right[ri]) {
        result.push_back(left[li++]);
      } else {
        result.push_back(right[ri++]);
      }
    }    

    while (li < left.size() ) {
      result.push_back(left[li++]);
    }    

    while (ri < right.size() ) {
      result.push_back(right[ri++]);
    }

    return result;
  }

  static void quick(std::vector<T> &v, int64_t begin, int64_t end) {
    if (begin >= end) {
      return;
    }

    auto partition = [&]() {
      auto start = begin;
      for (auto i = begin; i < end; ++i) {
        if (v[i] < v[end]) {
          std::swap(v[i], v[start++]);
        }
      }
      // Move the element at end to its final position
      std::swap(v[start], v[end]);
      return start; // Return the pivot.
    };
    auto pivot = partition();
    quick(v, begin, pivot - 1);
    quick(v, pivot + 1, end);
  }

  static std::vector<T> quick(const std::vector<T> &v) {
    auto result = v;
    quick(result, 0, result.size() - 1);
    return result;
  }
};

class Base
{
    using print_callback = std::function<void(void)>;
    
public:
    Base() : print_{[this]() { 
        ++this->size_;
        std::cout << "Base" << std::endl; 
        std::cout << "size_: " << size_ << std::endl;
    }}
    {}

    void print() const 
    {
        print_();
    }
    
protected:
    print_callback print_;
    std::size_t size_{1};
};

class Derived : public Base
{
public:
    Derived() : Base() {
        auto base_version = print_;
        print_ = [this, base_version](){ 
            base_version(); 
            ++this->extraSize_;
            std::cout << "Derived" << std::endl; 
            std::cout << "extraSize_: " << extraSize_ << std::endl;
        };
    }
    
    std::size_t extraSize_{2};
};

class DerivedDerived : public Derived
{
public:
    DerivedDerived() : Derived() {
        print_ = [](){ std::cout << "DerivedDerived" << std::endl; };
    }
};

namespace tests {
void run_tests() {
  //   Base* b = new Base;
  //   const Base* b2 = b;
  //   b2->print();
    
  //   const Derived* de = new Derived;
  //   de->print();
    
  //   const DerivedDerived* dd = new DerivedDerived;
  //   dd->print();
    
  //   const Base* bd = new Derived;
  //   bd->print();
  // return;

  auto d = decode("abc");
  std::cout << d << std::endl;
  d = decode("ab3[cd]ef");
  std::cout << d << std::endl;
  d = decode("ab10[cd]ef");
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

// Actual first round interview question
/*
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

class Base
{
    using print_callback = std::function<void(void)>;
    
public:
    Base() : print_{[&]() { 
        ++size_;
        std::cout << "Base" << std::endl; 
        std::cout << "size_: " << size_ << std::endl;
    }}
    {}

    void print() const 
    {
        print_();
    }
    
protected:
    print_callback print_;
    std::size_t size_{1};
};

class Derived : public Base
{
public:
    Derived() : Base() {
        auto base_version = print_;
        print_ = [&](){ 
            base_version(); 
            ++extraSize_;
            std::cout << "Derived" << std::endl; 
            std::cout << "extraSize_: " << extraSize_ << std::endl;
        };
    }
    
    std::size_t extraSize_{2};
};

class DerivedDerived : public Derived
{
public:
    DerivedDerived() : Derived() {
        print_ = [](){ std::cout << "DerivedDerived" << std::endl; };
    }
};



int main() {

    //
    Base* b = new Base;
    const Base* b2 = b;
    b2->print();
    
    const Derived* d = new Derived;
    d->print();
    
    const DerivedDerived* dd = new DerivedDerived;
    dd->print();
    
    const Base* bd = new Derived;
    bd->print();
        

    return 0;
}

*/



/*
Blockchain consensus algorithms

PoW (Proof of Work)
1.  Nodes solve an computational problem based on a series of transactions 
they have choosen to be included within the block.  The first node that 
solves this problem (normally it is generating a block hash that is less than some number)
gets to proposed the block to be added to the network.
2.  The proposed block will be sent to the other nodes for verification.
This verification may include verifying the transactions in it are valid and signed, 
the previous block hash is correct, the proof of work is correct, the timestamp is 
greater than the previous block, verifies the merkle tree of the transactions within the
block to make sure transactions have not been tampered with, etc.
3.  Once the block has been verified, it will be added to the local state and propogated
to the rest of the network.
4.  There may temporarily be multiple forks on each node during the mining and verification chain.
The longest chain wins.

PoS
1.  Nodes stake some assets into the network and then they are randomly selected to produced blocks
based on the size of their stake.
2.  The selected node will gather the transactions they want to include within the block and broadcast
the block to the network.
3.  Other validator nodes will validate the new block and if a quorum of validators approve the block it 
is added to the network.
4.  These networks can also have local temporary forks and will use some sort of algorithm for determiningg
the correct fork.

DPoS
1.  Nodes stake some assets into the network and then vote for a small number of validators to produce
and validate blocks on behalf of the network.
2.  Each elected validator take a turn at producing blocks in a predefined order.  If a validator fails 
to produce a block, the next validator in order will take over.
DPoS has a higher efficiency (given the smaller number of validators) and faster block times than PoS.
*/
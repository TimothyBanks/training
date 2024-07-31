#pragma once

template <typename Key, typename Value>
struct hash_map {
  using key_type = Key;
  using value_type = Value;

 private:
  struct node {
    key_type k;
    value_type v;
    std::unique_ptr<node> n;

    node() = default;
    node(const node&) = default;
    node(node&&) = default;
    node(key_type k_, value_type v_) : k{std::move(k_)}, v{std::move(v_)} {}
  };
  using node_ptr = std::unique_ptr<node>;

  size_t node_count{0};
  size_t bucket_count{2};
  std::vector<node_ptr> buckets{bucket_count};

  size_t hash(const key_type& k) {
    return std::hash<key_type>{}(k) % bucket_count;
  }

  template <typename Functor>
  void for_each(node* root, Functor f) {
    auto current = root;
    node* previous = nullptr;

    while (current) {
      if (f(current, previous)) {
        break;
      }
      previous = current;
      current = current->n.get();
    }
  }

  bool insert(node_ptr ptr) {
    if (!ptr) {
      return false;
    }

    auto index = hash(ptr->k);
    auto& row = buckets[index];

    if (!row) {
      row = std::move(ptr);
      ++node_count;
      return true;
    }

    node* last = nullptr;
    bool found = false;
    for_each(row.get(), [&](auto current, auto previous) {
      last = current;
      if (current->k == ptr->k) {
        found = true;
        return true;
      }
      return false;
    });

    if (found) {
      last->v = std::move(ptr->v);
      return false;
    }

    last->n = std::move(ptr);
    ++node_count;
    return true;
  }

  void resize(size_t new_bucket_size) {
    auto temp = std::vector<node_ptr>{};

    for (auto& b : buckets) {
      if (!b) {
        continue;
      }
      temp.push_back(std::move(b));
      while (temp.back()->n) {
        temp.push_back(std::move(temp.back()->n));
      }
    }

    node_count = 0;
    bucket_count = new_bucket_size;
    buckets.clear();
    buckets.resize(bucket_count);

    for (auto& b : temp) {
      insert(std::move(b));
    }
  }

 public:
  void print() {
    for (auto& b : buckets) {
      for_each(b.get(), [](auto current, auto previous) {
        std::cout << current->k << ", " << current->v << std::endl;
        return false;
      });
    }
  }

  bool insert(key_type k, value_type v) {
    auto result = insert(std::make_unique<node>(std::move(k), std::move(v)));
    if (node_count == bucket_count) {
      resize(bucket_count * 2);
    }
    return result;
  }

  const value_type& find(const key_type& k) const {
    auto index = hash(k);
    auto& row = buckets[index];

    node* found = nullptr;
    for_each(row.get(), [&](auto current, auto previous) {
      if (current->k == k) {
        found = current;
        return true;
      }
      return false;
    });

    if (!found) {
      throw "not found";
    }

    return found->v;
  }

  void remove(const key_type& k) {
    auto index = hash(k);
    auto& row = buckets[index];

    node* prev = nullptr;
    auto found = false;
    for_each(row.get(), [&](auto current, auto previous) {
      if (current->k == k) {
        prev = previous;
        found = true;
        return true;
      }
      return false;
    });

    if (!found) {
      return;
    }

    if (prev) {
      row = std::move(row->n);
    } else {
      prev->n = std::move(prev->n->n);
    }
  }
};
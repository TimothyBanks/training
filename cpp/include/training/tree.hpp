#include <memory>
#include <queue>
#include <stack>

namespace training {

namespace details {
template <typename T> struct tree_node;

template <typename T> using tree_node_ptr = std::shared_ptr<tree_node<T>>;

template <typename T> struct tree_node {
  T data;
  tree_node_ptr<T> left;
  tree_node_ptr<T> right;
};
} // namespace details

template <typename T> class tree {
public:
  enum class traversal {
    breadth_first,
    depth_first_pre_order, // Parent -> Left -> Right
    depth_first_in_order,  // Left -> Parent -> Right
    depth_first_post_order // Left -> Right -> Parent
  };

public:
  template <typename Container> explicit tree(const Container &c);

  template <typename Functor> void traverse(traversal t, const Functor &f);

private:
  template <typename Functor> void traverse_breadth_first_(const Functor &f);
  template <typename Functor> void traverse_pre_order_(const Functor &f);
  template <typename Functor> void traverse_in_order_(const Functor &f);
  template <typename Functor> void traverse_post_order_(const Functor &f);

private:
  using node_type = details::tree_node_ptr<T>;
  node_type m_root;
};

template <typename T>
template <typename Container>
tree<T>::tree(const Container &c) {
  if (c.empty()) {
    return;
  }

  m_root = std::make_shared<details::tree_node<T>>();
  m_root->data = c[0];

  auto q = std::queue<node_type>{};
  q.push(m_root);

  auto get_child = [&](auto &node, const auto &value) {
    if (value != '\0') {
      node = std::make_shared<details::tree_node<T>>();
      node->data = value;
      q.push(node);
    }
  };

  for (size_t i = 1; i < c.size();) {
    auto next = q.front();
    q.pop();
    get_child(next->left, c[i++]);
    get_child(next->right, c[i++]);
  }
}

template <typename T>
template <typename Functor>
void tree<T>::traverse(tree<T>::traversal t, const Functor &f) {
  switch (t) {
  case traversal::breadth_first:
    traverse_breadth_first_(f);
    break;
  case traversal::depth_first_in_order:
    traverse_in_order_(f);
    break;
  case traversal::depth_first_post_order:
    traverse_post_order_(f);
    break;
  case traversal::depth_first_pre_order:
    traverse_pre_order_(f);
    break;
  }
}

template <typename T>
template <typename Functor>
void tree<T>::traverse_breadth_first_(const Functor &f) {
  if (!m_root) {
    return;
  }

  auto q = std::queue<node_type>{};
  q.push(m_root);

  while (!q.empty()) {
    auto front = q.front();
    q.pop();
    if (f(front->data)) {
      return;
    }
    if (front->left) {
      q.push(front->left);
    }
    if (front->right) {
      q.push(front->right);
    }
  }
}

template <typename T>
template <typename Functor>
void tree<T>::traverse_pre_order_(const Functor &f) {
  if (!m_root) {
    return;
  }

  auto s = std::stack<node_type>{};
  auto current = m_root;
  while (current || !s.empty()) {
    if (current) {
      if (f(current->data)) {
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

template <typename T>
template <typename Functor>
void tree<T>::traverse_in_order_(const Functor &f) {
  if (!m_root) {
    return;
  }

  auto s = std::stack<node_type>{};
  auto current = m_root;
  while (current || !s.empty()) {
    if (current) {
      s.push(current);
      current = current->left;
    } else {
      current = s.top();
      s.pop();
      if (f(current->data)) {
        return;
      }
      current = current->right;
    }
  }
}

template <typename T>
template <typename Functor>
void tree<T>::traverse_post_order_(const Functor &f) {
  if (!m_root) {
    return;
  }

  auto s = std::stack<node_type>{};
  auto current = m_root;
  auto last = node_type{};

  while (current || !s.empty()) {
    if (current) {
      s.push(current);
      current = current->left;
    } else {
      auto peek = s.top();
      if (peek->right && peek->right != last) {
        current = peek->right;
      } else {
        if (f(peek->data)) {
          return;
        }
        last = peek;
        current = nullptr;
        s.pop();
      }
    }
  }
}

} // namespace training
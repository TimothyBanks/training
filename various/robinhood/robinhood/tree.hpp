//
//  tree.hpp
//  robinhood
//
//  Created by Timothy Banks on 4/27/21.
//

#ifndef tree_h
#define tree_h

#include <memory>
#include <queue>
#include <stack>
#include <limits>
#include <vector>

namespace tree {

template <typename T>
struct tree_node;

template <typename T>
using tree_node_ptr = std::shared_ptr<tree_node<T>>;

template <typename T>
struct tree_node {
    T data;
    tree_node_ptr<T> left;
    tree_node_ptr<T> right;
};

enum class traversal {
    breadth,
    pre_order_depth_first, // Parent, Left, Right
    in_order_depth_first,  // Left, Parent, Rigt
    post_order_depth_first // Left, Right, Parent
};

namespace details {

template <typename T, typename Functor>
void traverse_breadth_first(tree_node_ptr<T>& root, Functor f);

template <typename T, typename Functor>
void traverse_pre_order_depth_first(tree_node_ptr<T>& root, Functor f);

template <typename T, typename Functor>
void traverse_in_order_depth_first(tree_node_ptr<T>& root, Functor f);

template <typename T, typename Functor>
void traverse_post_order_depth_first(tree_node_ptr<T>& root, Functor f);

}

template <typename T>
tree_node_ptr<T> from_array(const std::vector<T>& values);

template <>
tree_node_ptr<char> from_array(const std::vector<char>& values) {
    if (values.empty()) {
        return nullptr;
    }
    
    auto root = std::make_shared<tree_node<char>>();
    root->data = values[0];
    
    auto q = std::queue<tree_node_ptr<char>>{};
    q.push(root);
    
    auto get_child = [&](auto& node, const auto& value) {
        if (value != '\0') {
            node = std::make_shared<tree_node<char>>();
            node->data = value;
            q.push(node);
        }
    };
    
    for (size_t i = 1; i < values.size();) {
        auto next = q.front();
        q.pop();
        get_child(next->left, values[i++]);
        get_child(next->right, values[i++]);
    }
    
    return root;
}

template <typename T, typename Functor>
void traverse(traversal t, tree_node_ptr<T>& root, Functor f) {
    switch (t) {
        case traversal::breadth:
            details::traverse_breadth_first(root, std::move(f));
            break;
        case traversal::pre_order_depth_first:
            details::traverse_pre_order_depth_first(root, std::move(f));
            break;
        case traversal::in_order_depth_first:
            details::traverse_in_order_depth_first(root, std::move(f));
            break;
        case traversal::post_order_depth_first:
            details::traverse_post_order_depth_first(root, std::move(f));
            break;
    }
}

template <typename T>
void print(traversal t, tree_node_ptr<T>& root) {
    auto print_lambda = [](auto& node) {
        std::cout << node.data << " ";
    };
    traverse(t, root, print_lambda);
    std::cout << std::endl;
}

namespace details {

template <typename T, typename Functor>
void traverse_breadth_first(tree_node_ptr<T>& root, Functor f) {
    if (!root) {
        return;
    }
    
    auto q = std::queue<tree_node_ptr<T>>{};
    q.push(root);
    
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        
        f(*current);
        
        if (current->left) {
            q.push(current->left);
        }
        
        if (current->right) {
            q.push(current->right);
        }
    }
}

template <typename T, typename Functor>
void traverse_pre_order_depth_first(tree_node_ptr<T>& root, Functor f) {
    if (!root) {
        return;
    }
    
    /*
     if (!root) return
     visit(root)
     move(root->left)
     move(root->right);
    */
    
    auto s = std::stack<tree_node_ptr<T>>{};
    auto current = root;
    
    while (current || !s.empty()) {
        if (current) {
            f(*current);
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

template <typename T, typename Functor>
void traverse_in_order_depth_first(tree_node_ptr<T>& root, Functor f) {
    if (!root) {
        return;
    }
    
    /*
     if (!root) return
     move(root->left)
     visit(root)
     move(root->right);
    */
    
    auto s = std::stack<tree_node_ptr<T>>{};
    auto current = root;
    
    while (current || !s.empty()) {
        if (current) {
            s.push(current);
            current = current->left;
        } else {
            current = s.top();
            s.pop();
            f(*current);
            current = current->right;
        }
    }
}

template <typename T, typename Functor>
void traverse_post_order_depth_first(tree_node_ptr<T>& root, Functor f) {
    if (!root) {
        return;
    }
    
    /*
     if (!root) return
     move(root->left)
     move(root->right);
     visit(root)
    */
    
    auto s = std::stack<tree_node_ptr<T>>{};
    auto current = root;
    auto last_visited = tree_node_ptr<T>{};
    
    while (current || !s.empty()) {
        if (current) {
            s.push(current);
            current = current->left;
        } else {
            auto peek = s.top();
            if (peek->right && peek->right != last_visited) {
                current = peek->right;
            } else {
                f(*peek);
                last_visited = peek;
                current = nullptr;
                s.pop();
            }
        }
    }
}

}

class Solution {
public:
    int coinChange(std::vector<int>& coins, int amount) {
        if (amount <= 0) {
            return 0;
        }
        std::sort(std::begin(coins), std::end(coins), std::greater<int>());
        return coin_change(coins, 0, amount);
    }
    
private:
    int coin_change(const std::vector<int>& coins, size_t index, int amount) {
        auto min = std::numeric_limits<int>::max();
        for (auto i = index; i < coins.size(); ++i) {
            auto amount_ = amount - coins[i];
            if (amount_ == 0) {
                min = std::min(min, 1);
            } else if (amount_ > 0) {
                auto result = coin_change(coins, i, amount_);
                if (result > 0) {
                    min = std::min(min, result + 1);
                }
            }
        }
        return min == std::numeric_limits<int>::max() ? -1 : min;
    }
};

long getWays_brute_force(int n, const std::vector<long>& c, size_t index) {
    if (n == 0) {
        return 1;
    }
    
    auto count = long{0};
    
    for (size_t i = index; i < c.size(); ++i) {
        auto& x = c[i];
        if (x > n) {
            continue;
        }
        count += getWays_brute_force(n - x, c, i);
    }
    
    return count;
}

long getWays_brute_force(int n, const std::vector<long>& c) {
    return getWays_brute_force(n, c, 0);
}

long getWays_top_down(int n, const std::vector<long>& c, size_t index, std::vector<long>& lookup) {
    // Top Down is also known as memoization
    if (n == 0) {
        lookup[n] = 1;
        return lookup[n];
    }
    auto count = long{0};
    for (size_t i = index; i < c.size(); ++i) {
        auto& x = c[i];
        if (x > n) {
            continue;
        }
        auto n_ = n - x;
        if (lookup[n_] == 0) {
            lookup[n_] = getWays_top_down(n_, c, i, lookup);
        }
        count += lookup[n_];
    }
    lookup[n] = count;
    return lookup[n];
}

long getWays_top_down(int n, const std::vector<long>& c) {
    // Top Down is also known as memoization
    auto lookup = std::vector<long>(n + 1, 0);
    return getWays_top_down(n, c, 0, lookup);
}

long getWays_bottom_up(int n, const std::vector<long>& c) {
    if (n == 0) {
        return 0;
    }
    auto ways = std::vector<long>(n + 1, 0);
    ways[0] = 1;
    for (const auto& x : c) {
        for (auto i = x; i <= n; ++i) {
            ways[i] += ways[i - x];
        }
    }
    return ways[n];
}

void run_test() {
    auto coins = std::vector<long>{1, 2, 3};
    auto ways = getWays_bottom_up(6, coins);
    ways = getWays_brute_force(6, coins);
    ways = getWays_top_down(6, coins);
    
//    auto s = Solution{};
//    auto result = s.coinChange(coins, 0);
    
    
    auto values = std::vector<char>{'F', 'B', 'G', 'A', 'D', '\0', 'I', '\0', '\0', 'C', 'E', 'H', '\0'};
    auto root = from_array(values);
    
    print(traversal::breadth, root);
    print(traversal::pre_order_depth_first, root);
    print(traversal::in_order_depth_first, root);
    print(traversal::post_order_depth_first, root);
}

}

#endif /* tree_h */

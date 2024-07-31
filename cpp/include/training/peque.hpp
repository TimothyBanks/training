#pragma once

template <typename T, typename Compare>
struct peque {  // priority_queue => peque?
  using type = T;
  using compare = Compare;

 private:
  std::vector<T> container;

  void heapify_down(size_t index) {
    auto target_index = index;
    auto left = 2 * index + 1;
    auto right = left + 1;

    auto adjust = [&](auto index) {
      if (index < container.size() &&
          compare{}(container[index], container[target_index])) {
        target_index = index;
      }
    };

    adjust(left);
    adjust(right);

    if (index != target_index) {
      std::swap(container[index], container[target_index]);
      heapify_down(target_index);
    }
    // else the item at index is correctly ordered.
  }

  void heapify_up(size_t index) {
    while (index > 0) {
      auto parent = (index - 1) / 2;
      if (compare{}(container[index], container[parent])) {
        std::swap(container[index], container[parent]);
        index = parent;
      } else {
        return;
      }
    }
  }

 public:
  T& front() {
    if (container.empty()) {
      throw "empty peque";
    }
    return container.front();
  }

  const T& front() const {
    if (container.empty()) {
      throw "empty peque";
    }
    return container.front();
  }

  bool empty() const { return container.empty(); }

  void pop() {
    if (container.empty()) {
      return;
    }

    if (container.size() == 1) {
      container.pop_back();
      return;
    }

    std::swap(container[0], container[container.size() - 1]);
    container.pop_back();

    heapify_down(0);
  }

  void push(T value) {
    container.push_back(std::move(value));
    heapify_up(container.size() - 1);
  }
};

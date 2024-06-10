namespace training::sort {

// Container concept
// Container c requires size() and indexer.

struct bubble {
  template <typename Container> static void sort(Container &c);
};

template <typename Container> void bubble::sort(Container &c) {
  if (c.size() <= 1) {
    return;
  }

  auto continue_ = true;
  while (continue_) {
    continue_ = false;
    for (size_t i = 0; i < c.size() - 1; ++i) {
      if (c[i] > c[i + 1]) {
        continue_ = true;
        std::swap(c[i], c[i + 1]);
      }
    }
  }
}

struct selection {
  template <typename Container> static void sort(Container &c);
};

template <typename Container> void selection::sort(Container &c) {
  if (c.size() <= 1) {
    return;
  }

  for (size_t i = 0; i < c.size() - 1; ++i) {
    auto min = i;
    for (size_t j = i + 1; j < c.size(); ++j) {
      min = c[j] < c[min] ? j : min;
    }
    std::swap(c[min], c[i]);
  }
}

struct insertion {
  template <typename Container> static void sort(Container &c);
};

template <typename Container> void insertion::sort(Container &c) {
  if (c.size() <= 1) {
    return;
  }

  for (size_t i = 1; i < c.size(); ++i) {
    for (int64_t j = i; j > 0; --j) {
      if (c[j] < c[j - 1]) {
        std::swap(c[j], c[j - 1]);
      }
    }
  }
}

namespace details {
template <typename Container>
size_t partition(Container &c, int64_t start, int64_t end) {
  // quicksort uses a pivot value.  Based on that value
  // items within the container will be arranged in such a way
  // that items less than will be moved to the left of the container
  // and items greater than will be moved to the right of the container.
  // This pivot value can be anything but choosing the correct pivot is
  // instrumental in achieving the optimal running time of this algorithm.
  auto begin = start;
  for (size_t i = start; i < end; ++i) {
    if (c[i] <= c[end]) {
      std::swap(c[i], c[begin++]);
    }
  }
  std::swap(c[begin], c[end]);
  return begin;
}

template <typename Container, typename Partition_functor>
void quick_sort(Container &c, int64_t start, int64_t end,
                const Partition_functor &p) {
  if (start >= end) {
    return;
  }
  auto pivot = p(c, start, end);
  // At this point, the pivot value is in its final sorted position at index
  // 'begin'
  quick_sort(c, start, pivot - 1, p);
  quick_sort(c, pivot + 1, end, p);
}
} // namespace details

struct quick {
  template <typename Container> static void sort(Container &c);

  template <typename Container, typename Partition_functor>
  static void sort(Container &c, const Partition_functor &p);
};

template <typename Container> void quick::sort(Container &c) {
  return sort(c, [](auto &con, auto start, auto end) {
    return details::partition(con, start, end);
  });
}

template <typename Container, typename Partition_functor>
void quick::sort(Container &c, const Partition_functor &p) {
  details::quick_sort(c, 0, c.size() - 1, p);
}

struct merge {
  template <typename Container> static void sort(Container &c);
};

template <typename Container> void merge::sort(Container &c) {
  if (c.size() <= 1) {
    return;
  }

  auto mid = c.size() / 2;
  auto left = Container{std::begin(c), std::begin(c) + mid};
  auto right = Container{std::begin(c) + mid, std::end(c)};
  sort(left);
  sort(right);
  auto left_index = size_t{0};
  auto right_index = size_t{0};
  auto index = size_t{0};
  while (left_index < left.size() && right_index < right.size()) {
    if (left[left_index] < right[right_index]) {
      c[index++] = left[left_index++];
    } else {
      c[index++] = right[right_index++];
    }
  }
  auto exhaust = [&](auto &partition, auto partition_index) {
    while (partition_index < partition.size()) {
      c[index++] = partition[partition_index++];
    }
  };
  exhaust(left, left_index);
  exhaust(right, right_index);
}

} // namespace training::sort
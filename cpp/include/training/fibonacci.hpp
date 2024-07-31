#include <unordered_map>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"

namespace training {

namespace details {
template <uint64_t N>
struct fibonacci {
  enum : uint64_t { value = fibonacci<N - 1>::value + fibonacci<N - 2>::value };
};

template <>
struct fibonacci<0> {
  enum : uint64_t { value = 0 };
};

template <>
struct fibonacci<1> {
  enum : uint64_t { value = 1 };
};
}  // namespace details

// static recursion
template <uint64_t N>
constexpr uint64_t fibonacci() {
  return details::fibonacci<N>::value;
}

// dynamic recursion
uint64_t fibonacci2(uint64_t n) {
  if (n == 0) {
    return 0;
  }

  if (n == 1) {
    return 1;
  }

  return fibonacci2(n - 1) + fibonacci2(n - 2);
}

namespace details {
uint64_t fibonacci3(uint64_t n, std::unordered_map<uint64_t, uint64_t>& cache) {
  if (n <= 1) {
    return n;
  }

  auto it = cache.find(n - 1);
  if (it == std::end(cache)) {
    cache[n - 1] = fibonacci3(n - 1, cache);
  }
  it = cache.find(n - 2);
  if (it == std::end(cache)) {
    cache[n - 2] = fibonacci3(n - 2, cache);
  }
  return cache[n - 1] + cache[n - 2];
}
}  // namespace details

// dynamic programming version 1
uint64_t fibonacci3(uint64_t n) {
  auto cache = std::unordered_map<uint64_t, uint64_t>{};
  return details::fibonacci3(n, cache);
}

// dynamic programming version 2
uint64_t fibonacci(uint64_t n) {
  if (n <= 1) {
    return n;
  }

  auto n_0 = uint64_t{0};
  auto n_1 = uint64_t{1};

  for (uint64_t i = 2; i <= n; ++i) {
    auto temp = n_1 + n_0;
    ;
    n_0 = n_1;
    n_1 = temp;
  }

  return n_1;
}

}  // namespace training

#pragma GCC diagnostic pop
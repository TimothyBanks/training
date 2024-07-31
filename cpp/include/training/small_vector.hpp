#pragma once

template <typename T, size_t Small_size_threshold>
class small_vector {
  using type = T;
  static constexpr auto threshold = Small_size_threshold;

 private:
  size_t length{0};
  size_t reserved{threshold};
  T small_buffer[threshold];
  std::unique_ptr<int8_t[]> large_buffer;
  T* ptr{small_buffer};

 public:
  constexpr small_vector() = default;

  template <typename... Args>
  T& emplace_back(Args&&... args) {
    if (length + 1 >= reserved) {
      auto new_buffer =
          std::unique_ptr<int8_t[]>(new int8_t[2 * reserved * sizeof(T)]);
      for (auto i = size_t{0}; i < length; ++i) {
        // new_buffer memory was not initialized yet need to construct in place
        // here too.
        new (&reinterpret_cast<T*>(new_buffer.get())[i]) T{std::move(ptr[i])};
      }
      large_buffer.swap(new_buffer);
      ptr = reinterpret_cast<T*>(large_buffer.get());
      reserved *= 2;
    }

    ++length;
    new (&ptr[length - 1]) T{std::forward<Args>(args)...};
    return ptr[length - 1];
  }

  void pop_back() {
    if (length == 0) {
      return;
    }
    ptr[length - 1].~T();
    --length;
  }

  const T& operator[](size_t idx) const { return ptr[idx]; }

  T& operator[](size_t idx) { return ptr[idx]; }

  size_t size() const { return length; }

  bool empty() const { return length == 0; }
};
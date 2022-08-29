#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

template <typename T>
class threadsafe_vector
{
public:
  threadsafe_vector(std::size_t size)
    : m_vector(size)
  {
  }

  T read(size_t index) const {
    auto [bi, ni] = block_for_index(index);
    auto& l = m_vector[bi];
    auto lock = std::shared_lock<std::shared_mutex>{l.mutex};
    return l.storage[ni];
  }

  void write(std::size_t index, T&& data) {
    auto [bi, ni] = block_for_index(index);
    auto& l = m_vector[bi];
    auto lock = std::unique_lock<std::shared_mutex>{l.mutex};
    l.storage[ni] = std::move(data);
  }

protected:
  std::pair<size_t, size_t> block_for_index(size_t index) const {
    auto block_index = index / block_size;
    auto new_index = index % block_size;
    return {block_index, new_index};
  }

private:
  constexpr static auto block_size = size_t{1};

  struct lockable
  {
    lockable(std::size_t size = 1)
    {
      storage.resize(size);
    }

    lockable(const lockable& l)
    : storage(l.storage)
    {
    }

    std::vector<T> storage;
    mutable std::shared_mutex mutex;
  };
  
  std::vector<lockable> m_vector;
};

// To execute C++, please define "int main()"
int main() {
  auto v = threadsafe_vector<std::string>{3};

  auto m = std::mutex{};
  auto producer_thread_1 = std::thread{[&](){
    v.write(0, "hello");
  }};
  auto producer_thread_2 = std::thread{[&](){
    v.write(1, "world");
  }};
  auto producer_thread_3 = std::thread{[&](){
    v.write(2, "foobar");
  }};
  auto consumer_thread_1 = std::thread{[&](){
    auto t = v.read(0);
    auto lock = std::lock_guard<std::mutex>{m};
    std::cout << t << std::endl;
  }};
  auto consumer_thread_2 = std::thread{[&](){
    auto t = v.read(1);
    auto lock = std::lock_guard<std::mutex>{m};
    std::cout << t << std::endl;
  }};

  producer_thread_1.join();
  producer_thread_2.join();
  producer_thread_3.join();
  consumer_thread_1.join();
  consumer_thread_2.join();

  return 0;
}

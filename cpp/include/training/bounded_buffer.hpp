// AKA Producer Consumer Problem
#pragma once

#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

template <typename T, size_t N = 255>
class bounded_buffer {
  using type = T;

 public:
  size_t size() const { return N; }

  void push(T t) {
    auto lock = std::unique_lock<std::mutex>{m_mutex};
    m_not_full.wait(lock, [&]() { return m_buffer.size() < N; });
    m_buffer.push_back(std::move(t));
    m_not_empty.notify_one();
  }

  T pop() {
    auto result = T{};
    {
      auto lock = std::unique_lock<std::mutex>{m_mutex};
      m_not_empty.wait(lock, [&]() { return !m_buffer.empty(); });
      result = m_buffer.front();
      m_buffer.pop_front();
      m_not_full.notify_one();
    }
    return result;
  }

 private:
  std::deque<T> m_buffer;
  std::mutex m_mutex;
  std::condition_variable m_not_full;
  std::condition_variable m_not_empty;
};

void run_bounded_buffer_test() {
  auto buffer = bounded_buffer<std::string>{};

  auto consumers = std::vector<std::thread>{};
  auto producers = std::vector<std::thread>{};
  auto write_mutex = std::mutex{};

  auto how_many_consumers = size_t{2};
  for (size_t i = 0; i < how_many_consumers; ++i) {
    consumers.emplace_back(
        [&](auto index) {
          while (true) {
            auto data = buffer.pop();
            {
              auto lock = std::lock_guard<std::mutex>{write_mutex};
              std::cout << "from consumer " << index << " read " << data
                        << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
          }
        },
        i);
  }

  auto how_many_producers = size_t{5};
  for (size_t i = 0; i < how_many_producers; ++i) {
    producers.emplace_back(
        [&](auto index) {
          auto message =
              std::string{"hello from producer "} + std::to_string(index);
          while (true) {
            buffer.push(message);
            std::this_thread::sleep_for(std::chrono::seconds(1));
          }
        },
        i);
  }

  for (auto& consumer : consumers) {
    consumer.join();
  }

  for (auto& producer : producers) {
    producer.join();
  }
}
// Write a struct that implements a thread-safe, FIFO, bounded int buffer.
// The struct should have two methods, push and pop. If you call push and there
// is no space to add a new int into the buffer, the call should block until a
// space is freed up to add the int. Similarly, if pop is called and there is
// nothing to remove from the buffer, the call should block until there is an
// element to remove.

// struct IntBuffer
// public:
// 	IntBuffer(const int max_size);
// 	void push(const int item);
// 	int pop();
// };

#include <condition_variable>
#include <deque>
#include <mutex>

template <typename T, size_t Max_size> class buffer {
public:
  void push(T item) {
    auto lock = std::unique_lock<std::mutex>{m_mutex};
    m_has_space.wait_for(lock, []() { return m_buffer.size() < Max_size; });
    m_buffer.push_back(std::move(item));
    m_not_empty.notify_one();
  }

  T pop() {
    auto result = T{};
    {
      auto lock = std::unique_lock<std::mutex>{m_mutex};
      m_not_empty.wait_for(lock, []() { return !m_buffer.empty(); }) result =
          m_buffer.front();
      m_buffer.pop_front();
      m_has_space.notify_one();
    }
    return result;
  }

private:
  std::deque<T> m_buffer;
  std::mutex m_mutex;
  std::condition_variable m_has_space;
  std::condition_variable m_not_empty;
};
#include <list>
#include <deque>

namespace training
{
class moving_average {
public:
  moving_average(size_t window)
  : m_window_size{window}
  {
  }

  void push(size_t n) {
    if (m_buffer.size() == m_window_size) {
      m_sum -= m_buffer.front();
      m_buffer.pop_front();
    } 
    m_buffer.push_back(n);
    m_sum += n;
  }

  moving_average& operator()(size_t n) {
    push(n);
    return *this;
  }

  double average() const {
    return static_cast<double>(m_sum) / m_buffer.size();
  }

private:
  std::deque<size_t> m_buffer;
  size_t m_window_size{0};
  size_t m_sum{0};
};
}
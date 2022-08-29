#pragma once
#include <functional>
#include <limits>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <string>

namespace dapperlabs {

using block           = std::string;
using process_functor = std::function<void(uint64_t, const std::vector<block>&)>;

/// A simple min/max range type.
template <typename T>
struct range {
   T min{ std::numeric_limits<T>::min() };
   T max{ std::numeric_limits<T>::max() };
};

/// Mutex that doesn't do anything.
/// \remarks Implements the basiclockable, lockable and sharedlockable concepts.  This mutex is used with the dispatch queue policy.
struct noop_mutex {
   auto lock() -> void {}
   auto unlock() -> void {}
   auto try_lock() -> bool { return true; }
   auto lock_shared() -> void {}
   auto unlock_shared() -> void {}
};

/// Lock guard that doesn't lock anything.
/// \remarks This lock guard is used with the dispatch queue policy.
struct noop_lock_guard {};

/// Policy class that implements a dispatch queue for processing the blocks.
/// \remarks This policy class exposes variable types and generic functionality that is used by the range_response_processor.
/// \remarks A dispatch queue results in a solution that can have less contention on the locks since there is only two places
///          where a lock needs to occur, one when pushing onto the queue and one when popping off the queue.  It can greatly
///          simplify the complexity of the synchronization by reducing the number of synchronization points.  It also insulates
///          the algorithm of the range processor from all the external threads and the algorithm can achieve it's maximum runtime
///          without the need for worrying about the overhead of acquiring locks.
struct dispatch_queue_policy {
   /// One of the main differences with the dispatch queue is that we don't require an atomic as the value type.
   using block_confirmations_t = std::unordered_map<block, uint64_t>;
   using confirmed_mutex_t     = noop_mutex;
   using active_mutex_t        = std::shared_mutex;

   /// Returns a new confirmation instance. In this case, just return an integer.
   static auto new_confirmation(uint64_t count) -> uint64_t { return count; }

   /// Increments the confirmation passed in.
   static auto increment_confirmation(uint64_t& confirmation) -> uint64_t {
      ++confirmation;
      return confirmation;
   }

   /// Returns a noop lock guard.
   /// \remarks In the case of a dispatch queue, the range processor algorithm doesn't need to lock.
   static auto get_shared_lock(active_mutex_t& mutex) {
     return noop_lock_guard{};
   }

   /// Initializes the policy.
   /// \remarks The dispatch queue policy starts a thread for processing the data off the queue.
   auto init(process_functor functor) -> void {
      auto thread_functor = [process_fn = std::move(functor), this]() {
         while (m_running.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(0));

            auto data = std::pair<uint64_t, std::vector<block>>{};
            {
               auto lock = std::lock_guard<std::mutex>{ m_mutex };
               if (m_process_queue.empty()) {
                  continue;
               }
               data = m_process_queue.front();
               m_process_queue.pop();
            }
            process_fn(data.first, data.second);
         }
      };
      auto dispatch_thread = std::thread{ thread_functor };
      dispatch_thread.detach();
   }

   /// Policy implementation of processing a range.
   /// \remarks The dispatch queue policy just pushes data onto a queue that the dispatch thread will process.
   auto process_range(uint64_t start_height, std::vector<block> blocks) -> void {
      auto lock = std::lock_guard<std::mutex>{ m_mutex };
      m_process_queue.emplace(std::pair(start_height, std::move(blocks)));
   }

 private:
   std::mutex                                          m_mutex;
   std::atomic<bool>                                   m_running{ true };
   std::queue<std::pair<uint64_t, std::vector<block>>> m_process_queue;
};

/// Policy class that uses finer grained mutexes for synchronization.
/// \remarks This policy class exposes variable types and generic functionality that is used by the range_response_processor.
/// \remarks This policy uses well placed mutexes within the range processing algorithm to achieve synhronization.
struct mutex_policy {
   /// Main difference of the mutex policy is that the confirmation count is sittig behind an atomic.
   /// This allows for updating the confirmation count while sitting behind a shared (reader) lock.
   using block_confirmations_t = std::unordered_map<block, std::shared_ptr<std::atomic<uint64_t>>>;
   using confirmed_mutex_t     = std::shared_mutex;
   using active_mutex_t        = std::shared_mutex;

   /// Returns a new confirmation instance. In this case, a shared poinrt to an atomic.
   static auto new_confirmation(uint64_t count) -> std::shared_ptr<std::atomic<uint64_t>> {
      return std::make_shared<std::atomic<uint64_t>>(count);
   }

   /// Increments the confirmation passed in.
   static auto increment_confirmation(const std::shared_ptr<std::atomic<uint64_t>>& confirmation) -> uint64_t {
      return confirmation->fetch_add(1) + 1;
   }

   /// Returns a shared locked for use by the range processor algorithm.
   static auto get_shared_lock(active_mutex_t& mutex) {
     return std::shared_lock{mutex};
   }

   /// Initializes the policy.
   /// \remarks In the mutex policy, we just save the functor to invoke directly by the process_range method.
   auto init(process_functor&& functor) -> void { m_process_functor = functor; }

   /// Policy implementation of processing a range.
   /// \remarks The mutex policy will invoke the process_functor directly.
   auto process_range(uint64_t start_height, const std::vector<block>& blocks) -> void {
      m_process_functor(start_height, blocks);
   }

 private:
   process_functor m_process_functor;
};

} // namespace dapperlabs
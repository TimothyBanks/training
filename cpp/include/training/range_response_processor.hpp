#pragma once

#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace dapperlabs {

using block = std::string;

template <typename T>
struct range {
  T min{std::numeric_limits<T>::min()};
  T max{std::numeric_limits<T>::max()};
};

class range_response_processor {
  using confirmed_block_functor = std::function<void(uint64_t, const block&)>;
  using block_confirmations =
      std::unordered_map<block, std::shared_ptr<std::atomic<uint64_t>>>;

 public:
  range_response_processor(uint64_t confirmation_threshold,
                           confirmed_block_functor&& confirmation_functor)
      : m_confirmation_threshold{confirmation_threshold},
        m_confirmed_block_functor{
            std::forward<confirmed_block_functor>(confirmation_functor)} {}

  auto process_range(uint64_t start_height, const std::vector<block>& blocks)
      -> void {
    auto is_confirmed = [&](auto block_height) {
      auto lock = std::shared_lock{m_confirmed_mutex};
      return m_confirmed_blocks.find(block_height) !=
             std::end(m_confirmed_blocks);
    };

    for (auto block_height = start_height;
         block_height < start_height + blocks.size(); ++block_height) {
      auto& current_block = blocks[block_height - start_height];

      if (is_confirmed(block_height)) {
        // block at this block_height has already been confirmed.
        continue;
      }

      auto found = false;
      auto confirmation_count = uint64_t{0};

      {
        // This is a happy path in that the block height has already been
        // encountered and the block has already been encountered at this block
        // height.  This should be the majority of the runtime, so we can do all
        // this behind a reader lock.
        auto lock = std::shared_lock{m_active_mutex};
        if (auto active_it = m_active_blocks.find(block_height);
            active_it != std::end(m_active_blocks)) {
          if (auto block_it = active_it->second.find(current_block);
              block_it != std::end(active_it->second)) {
            // Since confirmation count is atomic, we can do all of this behind
            // the shared locks.
            found = true;
            confirmation_count = block_it->second->fetch_add(1) + 1;
          }
        }

        if (found && confirmation_count < m_confirmation_threshold) {
          // Done processing this block height
          continue;
        }
      }

      // We are mutating the data structures, we need a write lock to do that.
      auto lock = std::unique_lock{m_active_mutex};

      if (is_confirmed(block_height)) {
        // block may have actually been confirmed by the time we get here.
        // We do this again because another thread may have confirmed the block
        // at this block height before we get here.
        continue;
      }

      // There is this edge case where the confirmation might be something like
      // 0 or 1.  That case should be taken care of if we allow the insertion to
      // happen first.
      auto active_it = m_active_blocks.find(block_height);
      if (active_it == std::end(m_active_blocks)) {
        // We need to insert a new active block height
        m_active_blocks.emplace(
            block_height,
            block_confirmations{
                {current_block, std::make_shared<std::atomic<uint64_t>>(1)}});
        confirmation_count = 1;
      } else if (active_it->second.find(current_block) ==
                 std::end(active_it->second)) {
        // We need to insert a new active block at a block height
        active_it->second.emplace(current_block,
                                  std::make_shared<std::atomic<uint64_t>>(1));
        confirmation_count = 1;
      }

      if (confirmation_count < m_confirmation_threshold) {
        // Still haven't reached the confirmation threshold.
        continue;
      }

      // No iterators beyond this point. They will be invalidated by the
      // mutations to the data structures.
      auto confirmed_lock = std::unique_lock{m_confirmed_mutex};

      // block is no longer active.
      m_active_blocks.erase(block_height);

      // The first one will always win here.  So if we encounter another
      // confirmed block, it will be ignored.
      if (m_confirmed_blocks.emplace(block_height, current_block).second) {
        confirmed_lock.unlock();
        lock.unlock();
        m_confirmed_block_functor(block_height, current_block);
      }
    }
  }

  auto get_active_range() -> range<uint64_t> {
    auto min = std::numeric_limits<uint64_t>::max();
    auto max = std::numeric_limits<uint64_t>::min();
    {
      auto lock = std::shared_lock{m_active_mutex};
      for (auto record : m_active_blocks) {
        min = std::min(record.first, min);
        max = std::max(record.first, max);
      }
    }
    return {.min = min, .max = max};
  }

 private:
  uint64_t m_confirmation_threshold{0};
  confirmed_block_functor m_confirmed_block_functor;
  std::shared_mutex m_confirmed_mutex;
  std::unordered_map<uint64_t, block> m_confirmed_blocks;
  std::shared_mutex m_active_mutex;
  std::unordered_map<uint64_t, block_confirmations> m_active_blocks;
};

}  // namespace dapperlabs
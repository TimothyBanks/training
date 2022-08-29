#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <dapperlabs/policies.hpp>

namespace dapperlabs {

/// Type to track the range of block heights that the synchronization engine should actively be requesting.
/// \tparam Policy A policy class that controls various parts of the range processor algorithm.
/// \remarks Refer to dispatch_queue_policy and mutex_policy for examples of Policy types.
template <typename Policy>
class range_response_processor {
   using confirmed_block_functor = std::function<void(uint64_t, const block&)>;
   using block_confirmations     = typename Policy::block_confirmations_t;

 public:
   /// Constructor
   /// \param confirmation_threshold The number of nodes that must confirm that a block is at a certain block height.
   /// \param confirmation_functor The callback invoked when a blocked becomes confirmed.
   range_response_processor(uint64_t confirmation_threshold, confirmed_block_functor&& confirmation_functor)
       : m_confirmation_threshold{ confirmation_threshold }, m_confirmed_block_functor{
            std::forward<confirmed_block_functor>(confirmation_functor)
         } {
      m_policy.init([&](auto start_height, const auto& blocks) {
         auto is_confirmed = [&](auto block_height) {
            // Depending on the policy, this may be a noop lock
            auto lock = std::shared_lock{ m_confirmed_mutex };
            return m_confirmed_blocks.find(block_height) != std::end(m_confirmed_blocks);
         };

         for (auto block_height = start_height; block_height < start_height + blocks.size(); ++block_height) {
            auto& current_block = blocks[block_height - start_height];

            if (is_confirmed(block_height)) {
               // block at this block_height has already been confirmed.
               continue;
            }

            auto found              = false;
            auto confirmation_count = uint64_t{ 0 };

            {
               // This is a happy path in that the block height has already been
               // encountered and the block has already been encountered at this block
               // height.  This should be the majority of the runtime, so we can do all
               // this behind a reader lock.
               // Depending on the policy, the following line may be a noop lock
               auto lock = Policy::get_shared_lock( m_active_mutex );
               if (auto active_it = m_active_blocks.find(block_height); active_it != std::end(m_active_blocks)) {
                  if (auto block_it = active_it->second.find(current_block); block_it != std::end(active_it->second)) {
                     // Since confirmation count is atomic, we can do all of this behind
                     // the shared locks.
                     found              = true;
                     confirmation_count = Policy::increment_confirmation(block_it->second);
                  }
               }

               if (found && confirmation_count < m_confirmation_threshold) {
                  // Done processing this block height
                  continue;
               }
            }

            // We are mutating the data structures, we need a write lock to do that.
            auto lock = std::unique_lock{ m_active_mutex };

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
               confirmation_count = 1;
               m_active_blocks.emplace(block_height,
                                       block_confirmations{ { current_block, Policy::new_confirmation(confirmation_count) } });
            } else if (active_it->second.find(current_block) == std::end(active_it->second)) {
               // We need to insert a new active block at a block height
               confirmation_count = 1;
               active_it->second.emplace(current_block, Policy::new_confirmation(confirmation_count));
            }

            if (confirmation_count < m_confirmation_threshold) {
               // Still haven't reached the confirmation threshold.
               continue;
            }

            // Reached the confirmation threshold.

            // No iterators beyond this point. They will be invalidated by the
            // mutations to the data structures.
            // Depending on the policy, this may be a noop lock.
            auto confirmed_lock = std::unique_lock{ m_confirmed_mutex };

            // block is no longer active.
            m_active_blocks.erase(block_height);

            // The first one will always win here.  So if we encounter another
            // confirmed block, it will be ignored. Given the nature of concurrency, this 
            // doesn't result in a deterministic result.
            if (m_confirmed_blocks.emplace(block_height, current_block).second) {
               confirmed_lock.unlock();
               lock.unlock();
               m_confirmed_block_functor(block_height, current_block);
            }
         }
      });
   }

   /// Processes a range of blocks.
   auto process_range(uint64_t start_height, const std::vector<block>& blocks) -> void {
      m_policy.process_range(start_height, blocks);
   }

   /// Returns the current active range of the range processor.
   auto get_active_range() -> range<uint64_t> {
      auto min = std::numeric_limits<uint64_t>::max();
      auto max = std::numeric_limits<uint64_t>::min();
      {
         auto lock = std::shared_lock{ m_active_mutex };
         std::cout << "active block height count " << m_active_blocks.size() << std::endl;
         for (auto record : m_active_blocks) {
            min = std::min(record.first, min);
            max = std::max(record.first, max);
         }
      }
      return { .min = min == std::numeric_limits<uint64_t>::max() ? 0 : min, .max = max };
   }

 private:
   uint64_t                                          m_confirmation_threshold{ 0 };
   confirmed_block_functor                           m_confirmed_block_functor;
   typename Policy::confirmed_mutex_t                m_confirmed_mutex;
   std::unordered_map<uint64_t, block>               m_confirmed_blocks;
   typename Policy::active_mutex_t                   m_active_mutex;
   std::unordered_map<uint64_t, block_confirmations> m_active_blocks;
   Policy                                            m_policy;
};

} // namespace dapperlabs
#include "cache.hpp"
#include <mutex>
#include <utility>

namespace automata::game_of_life {

namespace executor {

SetCells& Cache::GetNextCell() {
  std::lock_guard guard{mutex_};

  if (valid_.size() > 1) {
    deleter_.GoDelete(std::move(valid_.front()));
    valid_.pop_front();

    // Wake Producer
    if (cache_line_workload_ < MidCacheLineWorkload) {
      is_big_cache_.notify_one();
    }

    cache_line_workload_ -= valid_.front().size();
  }

  return valid_.front();
}

void Cache::PushNewCell(SetCells&& set) {
  std::unique_lock lock{mutex_};

  is_big_cache_.wait(lock, [this]() {
    return cache_line_workload_ < MaxCacheLineWorkload;
  });

  if (is_invalidation_process) {
    // todo
    deleter_.GoDelete(std::move(set));
    is_invalidation_process = false;
  }

  cache_line_workload_ += set.size();

  valid_.push_back(std::move(set));
}

const SetCells& Cache::GetLastCell() {
  std::lock_guard guard{mutex_};

  return valid_.back();
}

void Cache::Invalidate() {
  std::lock_guard guard{mutex_};
  is_invalidation_process = true;

  auto tmp = std::move(valid_.front());
  deleter_.GoDelete(std::move(valid_));

  valid_.push_back(std::move(tmp));
}

}  // namespace executor

}  // namespace automata::game_of_life

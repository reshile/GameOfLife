#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>

#include "../data_presentation/set_cells.hpp"
#include "deleter.hpp"

namespace automata::game_of_life {

namespace executor {

// TODO instead, we need to do: template <typename T>
class Cache {
 public:
  /////////////////////////////////////////////////////

  using CacheLine = std::list<SetCells>;

  /////////////////////////////////////////////////////

  //! [Consumer]

  // To get the next cell on the line cache
  SetCells& GetNextCell();

  // todo Invalidates the current cache line and returns the last valid state
  void Invalidate();

  /////////////////////////////////////////////////////

  //! [Producer]

  // Adds a new line cache cell
  void PushNewCell(SetCells&& set);

  const SetCells& GetLastCell();

  /////////////////////////////////////////////////////

 private:
  static const std::size_t MidCacheLineWorkload{200'000};
  static const std::size_t MaxCacheLineWorkload{1'000'000};

  // Synchronization
  std::mutex mutex_;
  std::condition_variable is_big_cache_;

  // The workload of the cache line
  std::size_t cache_line_workload_{0};

  bool is_invalidation_process{false};

  ParallelDeleter deleter_;

  CacheLine valid_;
};

}  // namespace executor

}  // namespace automata::game_of_life

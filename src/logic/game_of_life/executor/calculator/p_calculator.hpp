#pragma once

#include <oneapi/tbb/cache_aligned_allocator.h>
#include <oneapi/tbb/null_mutex.h>
#include <oneapi/tbb/queuing_mutex.h>
#include <oneapi/tbb/spin_mutex.h>
#include <tbb/tbb.h>
#include <cstddef>
#include <deque>
#include <vector>

#include "../../data_presentation/game_config.hpp"
#include "icalculator.hpp"

namespace automata::game_of_life {

namespace executor {

// Set => Calculate => Take the result
// All calculations take place in one thread
class ParallelCalculator : public ICalculator {
 public:
  /////////////////////////////////////////////////////

  // One-shot [or if the game configuration has changed]
  void Set(GameConfig* config) override;

  // Clear result
  void Clear() override;

  // Calc result
  void Calc(SetCells* status_quo) override;

  // Set result into [where]
  void Update(SetCells* where) override;

  /////////////////////////////////////////////////////

 private:
  void CellProcessing(std::size_t thread_id, Cell cell, CellState is_alive);

  /////////////////////////////////////////////////////

 private:
  // Mutex:
  tbb::spin_mutex mutex1_;
  tbb::spin_mutex mutex2_;

  SetCells* set_{nullptr};
  std::size_t thread_id_{0};

  // todo: std::vector<std::vector<Cell>> => std::vector<std::list<Cell>> +
  // todo: + stack allocator
  std::vector<std::deque<Cell>> split_response_;

  // Calculate result
  SetCells result;

  // Game config
  GameConfig* config_{nullptr};
};

}  // namespace executor

}  // namespace automata::game_of_life

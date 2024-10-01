#pragma once

#include <oneapi/tbb/parallel_for_each.h>
#include "../../data_presentation/cell.hpp"
#include "../../data_presentation/game_config.hpp"
#include "icalculator.hpp"

namespace automata::game_of_life {

namespace executor {

// Set => Calculate => Take the result
// All calculations take place in one thread
class NoParallelCalculator : public ICalculator {
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
  void CellProcessing(SetCells* set, Cell cell, CellState is_alive);

  /////////////////////////////////////////////////////

 private:
  // Calculate result
  SetCells result;

  // Game config
  GameConfig* config_{nullptr};
};

}  // namespace executor

}  // namespace automata::game_of_life

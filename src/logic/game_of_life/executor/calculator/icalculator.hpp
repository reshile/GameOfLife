#pragma once

#include "../../data_presentation/game_config.hpp"
#include "../../data_presentation/set_cells.hpp"

namespace automata::game_of_life {

namespace executor {

// Set => Calculate => Take the result
class ICalculator {
 public:
  /////////////////////////////////////////////////////

  ICalculator() = default;

  // Non-copyable
  ICalculator(const ICalculator&) = delete;
  ICalculator& operator=(const ICalculator&) = delete;

  // Non-movable
  ICalculator(ICalculator&&) = delete;
  ICalculator& operator=(ICalculator&&) = delete;

  virtual ~ICalculator() = default;

  /////////////////////////////////////////////////////

  // One-shot [or if the game configuration has changed]
  virtual void Set(GameConfig* config) = 0;

  // Clear result
  virtual void Clear() = 0;

  // Calc result
  virtual void Calc(SetCells* status_quo) = 0;

  // Set result into [where]
  virtual void Update(SetCells* where) = 0;

  /////////////////////////////////////////////////////
};

}  // namespace executor

}  // namespace automata::game_of_life

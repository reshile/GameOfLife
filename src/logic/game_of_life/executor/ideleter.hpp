#pragma once

#include <list>

#include "../data_presentation/set_cells.hpp"

namespace automata::game_of_life {

namespace executor {

class IDeleter {
 public:
  ~IDeleter() = default;

  /////////////////////////////////////////////////////

  using CacheLine = std::list<SetCells>;

  /////////////////////////////////////////////////////

  // Start Deleter
  virtual void Start() = 0;

  // Submit a task for deletion
  virtual void GoDelete(SetCells&& node) = 0;

  // Submit tasks for deletions
  virtual void GoDelete(CacheLine&& nodes) = 0;

  // Stop Deleter
  virtual void Stop() = 0;

  /////////////////////////////////////////////////////
};

}  // namespace executor

}  // namespace automata::game_of_life

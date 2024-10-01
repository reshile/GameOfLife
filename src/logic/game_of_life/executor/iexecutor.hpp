#pragma once

#include <memory>

#include "calculator/icalculator.hpp"

namespace automata::game_of_life {

namespace executor {

class IExexutor {
 public:
  /////////////////////////////////////////////////////

  IExexutor() = default;

  // Non-copyable
  IExexutor(const IExexutor&) = delete;
  IExexutor& operator=(const IExexutor&) = delete;

  // Non-movable
  IExexutor(IExexutor&&) = delete;
  IExexutor& operator=(IExexutor&&) = delete;

  virtual ~IExexutor() = default;

  /////////////////////////////////////////////////////

  // Initial
  virtual void SetConfig(GameConfig* config) = 0;
  virtual void SetCalculator(std::unique_ptr<ICalculator>&& calculator) = 0;

  /////////////////////////////////////////////////////

  // One-shot
  virtual void Start() = 0;
  virtual void Stop() = 0;

  // The game will paused and resumed
  virtual void Pause() = 0;
  virtual void ReStart() = 0;

  // Kill all the cells
  virtual void Clear() = 0;

  // Get the next iteration in the game and show
  virtual void Step() = 0;
  virtual const SetCells& Show() = 0;

  //
  virtual void SetNewCells(SetCells&& new_cells) = 0;
  virtual void DeleteCells(const SetCells& cells) = 0;

  /////////////////////////////////////////////////////
};

}  // namespace executor

}  // namespace automata::game_of_life

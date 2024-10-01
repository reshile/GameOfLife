#pragma once

#include <cstddef>
#include <memory>
#include <thread>

#include "../data_presentation/game_config.hpp"
#include "cache.hpp"
#include "calculator/icalculator.hpp"
#include "iexecutor.hpp"
#include "state_machine.hpp"

namespace automata::game_of_life {

namespace executor {

// The class for calculating the entire logic of the game, and it does it on
// different threads
class MultiThreadedExecutor : public IExexutor {
 public:
  /////////////////////////////////////////////////////

  ~MultiThreadedExecutor();

  /////////////////////////////////////////////////////

  void SetConfig(GameConfig* config) override;
  void SetCalculator(std::unique_ptr<ICalculator>&& calculator) override;

  /////////////////////////////////////////////////////

  // One-shot
  void Start() override;
  void Stop() override;

  // The game will paused and resumed
  void Pause() override;
  void ReStart() override;

  // Kill all the cells
  void Clear() override;

  // Get the next iteration in the game and show
  void Step() override;
  const SetCells& Show() override;

  //
  void SetNewCells(SetCells&& new_cells) override;

  /////////////////////////////////////////////////////

 private:
  void Work();

  /////////////////////////////////////////////////////

 private:
  enum ExecutorState {
    run,
    pause,
    stop,
  };

  // Runtime
  ExecutorState executor_state_{stop};
  StateMachine machine_;

  // Game setting
  std::size_t iter_count_{1};
  GameConfig* config_;

  // Calculation
  std::thread worker_;
  SetCells* for_render_{nullptr};
  Cache cache_;
  std::unique_ptr<ICalculator> calculator_;
};

}  // namespace executor

}  // namespace automata::game_of_life

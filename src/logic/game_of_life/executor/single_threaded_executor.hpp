#pragma once

#include "../data_presentation/game_config.hpp"
#include "calculator/icalculator.hpp"
#include "iexecutor.hpp"

namespace automata::game_of_life {

namespace executor {

class SingleThreadedExecutor : public IExexutor {
 public:
  /////////////////////////////////////////////////////

  ~SingleThreadedExecutor();

  /////////////////////////////////////////////////////

  // Initial
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
  void DeleteCells(const SetCells& cells) override;

  /////////////////////////////////////////////////////

 private:
  enum State {
    run,
    pause,
    stop,
  };

  // Runtime
  State executor_state_{State::stop};

  // Game
  GameConfig* config_;
  SetCells living_cells_;

  // Calculator
  std::unique_ptr<ICalculator> calculator_;

  /////////////////////////////////////////////////////
};

}  // namespace executor

}  // namespace automata::game_of_life

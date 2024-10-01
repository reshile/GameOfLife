#pragma once

#include <SFML/Config.hpp>
#include <SFML/System/Clock.hpp>

#include <cstddef>
#include <memory>
#include <string>

#include "data_presentation/cell.hpp"
#include "data_presentation/game_config.hpp"
#include "data_presentation/game_info.hpp"
#include "data_presentation/game_template.hpp"
#include "data_presentation/set_cells.hpp"
#include "executor/iexecutor.hpp"

namespace automata {

enum class Calculators {
  ParallelCalculator,
  NoParallelCalculator,
};

enum class Executors {
  SingleThreadedExecutor,
  MultiThreadedExecutor,
};

namespace game_of_life {

// todo
// Game = IExecutor + ICalculator ( +  IGraphic)
// [ game_loop_1 ] [ game_loop_2 ] [ game_loop_3 ] ... [ game_loop_n ]
class Game {
 public:
  /////////////////////////////////////////////////////

  Game() = default;
  ~Game() = default;

  // Non-copyable
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  // Non-movable
  Game(Game&&) = delete;
  Game& operator=(Game&&) = delete;

  /////////////////////////////////////////////////////

  //! One-shot functionality in the game loop:

  // Logic initial
  void SetExecutor(Executors);
  void SetCalculator(Calculators);

  // Game config initial
  void SetBoard(size_t row_count, size_t columns_count, bool is_endless_board);
  void SetRule(rule::GameTemplate);

  // One-shot
  void OneLoopStart();
  void OneLoopStop();

  /////////////////////////////////////////////////////

  //! Online functionality in the game loop:

  // Info
  const GameInfo& Info();

  // Speed
  void IncreaseGameSpeed();
  void DecreaseGameSpeed();

  // The game will paused and resumed
  void Pause();
  void ReStart();

  // Kill all the cells
  void Clear();

  // Get the next iteration in the game
  const SetCells& OneIter();

  // Template
  void SetTemplate(std::string template_name, Cell where);
  void SetNewCell(Cell cell);
  void DeleteCell(Cell cell);

  // todo : wtf
  template <class Container>
  void PutCells(Container&& cells);

  /////////////////////////////////////////////////////

 private:
  // Game speed
  std::size_t iter_count_;
  sf::Int32 update_frequency_{40};
  sf::Clock timer_;

  // Game
  GameConfig config_;

  // For more information about the game
  GameInfo info_;

  // Executor
  std::unique_ptr<executor::IExexutor> executor_;
};

}  // namespace game_of_life

}  // namespace automata

#include <cstddef>
#include <type_traits>

#include "game_construct.hpp"
#include "data_presentation/rule.hpp"
#include "executor/iexecutor.hpp"
// #include "executor/multi_threaded_executor.hpp"
#include "executor/calculator/np_calculator.hpp"
#include "executor/calculator/p_calculator.hpp"
#include "executor/single_threaded_executor.hpp"
#include "templates/parser.hpp"

namespace automata::game_of_life {

///////////////////////////////////////////////////////////////////////////////

void Game::SetExecutor(Executors executor_type) {
  if (executor_type == Executors::SingleThreadedExecutor) {
    executor_ = std::make_unique<executor::SingleThreadedExecutor>();

  } else if (executor_type == Executors::MultiThreadedExecutor) {
    // executor_ = std::make_unique<executor::MultiThreadedExecutor>();
  }
}
void Game::SetCalculator(Calculators calculator_type) {
  if (calculator_type == Calculators::NoParallelCalculator) {
    executor_->SetCalculator(
        std::make_unique<executor::NoParallelCalculator>());

  } else if (calculator_type == Calculators::ParallelCalculator) {
    executor_->SetCalculator(std::make_unique<executor::ParallelCalculator>());
  }
}

void Game::SetRule(rule::GameTemplate template_name) {
  config_.rule = rule::GameRules(template_name);
}
void Game::SetBoard(size_t row_count, size_t columns_count,
                    bool is_endless_board) {
  config_.is_endless_board = is_endless_board;
  config_.rows_count = row_count;
  config_.columns_count = columns_count;
}

void Game::OneLoopStart() {
  info_.iteration = 0;
  info_.number_of_living = 0;

  timer_.restart();
  iter_count_ = 1;

  executor_->SetConfig(&config_);
  executor_->Start();
}
void Game::OneLoopStop() {
  executor_->Stop();
}

///////////////////////////////////////////////////////////////////////////////

const GameInfo& Game::Info() {
  return info_;
}

void Game::IncreaseGameSpeed() {
  ++iter_count_;
}
void Game::DecreaseGameSpeed() {
  if (iter_count_ > 0) {
    --iter_count_;
  }
}

void Game::Pause() {
  executor_->Pause();
}
void Game::ReStart() {
  timer_.restart();
  executor_->ReStart();
}

void Game::Clear() {
  executor_->Clear();
}

const SetCells& Game::OneIter() {
  if (timer_.getElapsedTime().asMilliseconds() >= update_frequency_) {
    for (std::size_t iter = 0; iter < iter_count_; ++iter) {
      executor_->Step();

      ++info_.iteration;
      info_.number_of_living = executor_->Show().size();
    }

    timer_.restart();
  }

  return executor_->Show();
}

void Game::SetTemplate(std::string template_name, Cell where) {
  // Set template
  templates::Parser parse(&config_);
  parse.Parse(std::move(template_name), where);

  // For more information about the game
  info_.number_of_living += parse.living_cells_.size();

  executor_->SetNewCells(std::move(parse.living_cells_));
}

void Game::SetNewCell(Cell cell) {
  executor_->SetNewCells({cell});
}
void Game::DeleteCell(Cell cell) {
  executor_->DeleteCells({cell});
}

template <class Container>
void Game::PutCells(Container&& cells) {
  if constexpr (std::is_same_v<Container, SetCells>) {
    executor_->SetNewCells(std::move(cells));

  } else {
    executor_->SetNewCells(SetCells(cells.begin(), cells.end()));
  }
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace automata::game_of_life

#include <cassert>

#include "single_threaded_executor.hpp"
#include "calculator/icalculator.hpp"

namespace automata::game_of_life {

namespace executor {

///////////////////////////////////////////////////////////////////////////////

SingleThreadedExecutor::~SingleThreadedExecutor() {
  assert(executor_state_ == stop &&
         "you must call STOP before completing the logic of the game");
}

///////////////////////////////////////////////////////////////////////////////

void SingleThreadedExecutor::SetConfig(GameConfig* config) {
  config_ = config;
  if (calculator_ != nullptr) {
    calculator_->Set(config_);
  }
}

void SingleThreadedExecutor::SetCalculator(
    std::unique_ptr<ICalculator>&& calculator) {
  calculator_ = std::move(calculator);
  calculator_->Set(config_);
}

///////////////////////////////////////////////////////////////////////////////

void SingleThreadedExecutor::Start() {
  executor_state_ = run;
}
void SingleThreadedExecutor::Stop() {
  executor_state_ = stop;
}

void SingleThreadedExecutor::Pause() {
  executor_state_ = pause;
}
void SingleThreadedExecutor::ReStart() {
  executor_state_ = run;
}

void SingleThreadedExecutor::Clear() {
  living_cells_.clear();
}

void SingleThreadedExecutor::Step() {
  if (executor_state_ == run) {
    // Updating board data
    calculator_->Clear();
    calculator_->Calc(&living_cells_);
    calculator_->Update(&living_cells_);
  }
}
const SetCells& SingleThreadedExecutor::Show() {
  return living_cells_;
}

void SingleThreadedExecutor::SetNewCells(SetCells&& new_cells) {
  // Updating board data
  living_cells_.merge(std::move(new_cells));
}
void SingleThreadedExecutor::DeleteCells(const SetCells& cells) {
  // Updating board data
  for (const auto& cell : cells) {
    living_cells_.erase(cell);
  }
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace executor

}  // namespace automata::game_of_life

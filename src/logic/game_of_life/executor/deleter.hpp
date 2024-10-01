#pragma once

#include <atomic>
#include <cstddef>
#include <list>
#include <thread>

#include "../data_presentation/set_cells.hpp"
#include "ideleter.hpp"
#include "state_machine.hpp"

namespace automata::game_of_life {

namespace executor {

class ParallelDeleter : public IDeleter {
 public:
  /////////////////////////////////////////////////////

  // Start Deleter
  void Start() override;

  // Submit a task for deletion
  void GoDelete(SetCells&& node) override;

  // Submit tasks for deletions
  void GoDelete(CacheLine&& nodes) override;

  // Stop Deleter
  void Stop() override;

  /////////////////////////////////////////////////////

 private:
  void Worker();

  /////////////////////////////////////////////////////

 private:
  std::thread thread_;
  StateMachine machine_;

  std::atomic<std::size_t> major_target_sz_;
  std::atomic<std::size_t> minor_targets_sz_;

  CacheLine major_target_;
  std::list<CacheLine> minor_targets_;
};

}  // namespace executor

}  // namespace automata::game_of_life

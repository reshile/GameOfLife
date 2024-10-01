#pragma once

#include "deleter.hpp"
#include "state_machine.hpp"

namespace automata::game_of_life {

namespace executor {

///////////////////////////////////////////////////////////////////////////////

// Start Deleter
void ParallelDeleter::Start() {
  machine_.Start();

  thread_ = std::thread([this]() {
    Worker();
  });
}

// Submit a task for deletion
void ParallelDeleter::GoDelete(SetCells&& node) {
  major_target_.push_back(std::move(node));
  major_target_sz_.fetch_add(1);

  machine_.Wake();
}

// Submit tasks for deletions
void ParallelDeleter::GoDelete(CacheLine&& nodes) {
  std::size_t size = nodes.size();
  minor_targets_.push_back(std::move(nodes));
  minor_targets_sz_.fetch_add(size);

  machine_.Wake();
}

// Stop Deleter
void ParallelDeleter::Stop() {
  machine_.Stop();
}

///////////////////////////////////////////////////////////////////////////////

void ParallelDeleter::Worker() {
  while (machine_.IsWork()) {
    std::size_t zero = 0;

    if (major_target_sz_.compare_exchange_strong(zero, 0) &&
        minor_targets_sz_.compare_exchange_strong(zero, 0)) {
      machine_.Sleep();
    } else {
      zero = 0;
    }

    if (!major_target_sz_.compare_exchange_strong(zero, 0)) {
      major_target_.pop_front();
      major_target_sz_.fetch_sub(1);
    } else {
      zero = 0;
    }

    if (!minor_targets_sz_.compare_exchange_strong(zero, 0)) {
      minor_targets_.front().pop_front();
      minor_targets_sz_.fetch_sub(1);
    }

    machine_.Wait();
  }
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace executor

}  // namespace automata::game_of_life

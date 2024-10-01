#pragma once

#include <atomic>

namespace automata::game_of_life {

namespace executor {

// todo docs
class StateMachine {
 public:
  /////////////////////////////////////////////////////

  StateMachine() = default;
  ~StateMachine() = default;

  // Non-movable Non-copyable
  StateMachine(const StateMachine&) = delete;
  StateMachine(StateMachine&&) = delete;
  StateMachine& operator=(const StateMachine&) = delete;
  StateMachine& operator=(StateMachine&&) = delete;

  /////////////////////////////////////////////////////

  void Start() {
    state_.store(run);
  }

  void Wake() {
    state_.store(run);
    state_.notify_all();
  }

  void Sleep() {
    state_.store(sleep);
  }

  void Wait() {
    State check = sleep;

    while (state_.compare_exchange_strong(check, sleep)) {
      state_.wait(sleep);
      check = sleep;
    }
  }

  bool IsWork() {
    State check = stop;
    return !state_.compare_exchange_strong(check, stop);
  }

  void Stop() {
    state_.store(stop);
  }

  /////////////////////////////////////////////////////

 private:
  enum State : uint32_t {
    run,
    sleep,
    stop,
  };

  std::atomic<State> state_;
};

}  // namespace executor

}  // namespace automata::game_of_life

//! lololoshka  don't delete!!!!!

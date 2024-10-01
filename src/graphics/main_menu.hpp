#pragma once
#include <coroutine>
#include "grid.hpp"
#include "user_interface.hpp"

#include <sfeMovie/Movie.hpp>

struct Awaitable;

struct Menu  // NOLINT
{
  struct promise_type;
  using Handle = std::coroutine_handle<promise_type>;

  struct promise_type  // NOLINT
  {
    Menu get_return_object() {
      return Menu(Handle::from_promise(*this));
    }  // NOLINT
    std::suspend_never initial_suspend() {
      return {};
    }  // NOLINT
    std::suspend_never final_suspend() noexcept {
      return {};
    }  // NOLINT
    void return_void() {
    }  // NOLINT
    void unhandled_exception() {
    }  // NOLINT
  };

  explicit Menu(Handle handle)
      : handle_(handle) {
  }
  ~Menu() {
    // handle_.destroy();
  }

  void Resume() {
    if (!handle_.done()) {
      handle_.resume();
    }
  }

  Handle handle_;
};

Menu MainMenu(Grid&, UserInterface&);
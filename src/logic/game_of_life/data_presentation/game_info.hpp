#pragma once

#include <cstddef>

namespace automata::game_of_life {

struct GameInfo {
  std::size_t number_of_living{0};
  std::size_t iteration{0};
};

}  // namespace automata::game_of_life

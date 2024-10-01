#pragma once

#include <cstdint>

#include "rule.hpp"

namespace automata::game_of_life {

struct GameConfig {
  bool is_endless_board;
  std::int64_t rows_count;
  std::int64_t columns_count;
  rule::GameRules rule;
};

}  // namespace automata::game_of_life

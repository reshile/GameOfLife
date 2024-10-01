#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include <unordered_set>

#include "cell_state.hpp"
#include "game_template.hpp"

namespace automata::game_of_life {

namespace rule {

struct Offset {
  std::int8_t x_coordinate;
  std::int8_t y_coordinate;
};

struct GameRules {
  //////////////////////////////////////////////////////////////////

  GameRules();
  ~GameRules() = default;

  // Copyable and Movable
  GameRules(const GameRules&) = default;
  GameRules& operator=(const GameRules&) = default;
  GameRules(GameRules&&) = default;
  GameRules& operator=(GameRules&&) = default;

  //////////////////////////////////////////////////////////////////

  GameRules(std::vector<Offset> neighbors,
            std::unordered_set<std::size_t> cell_survival_conditions,
            std::unordered_set<std::size_t> cell_birth_conditions);

  // Game patterns
  GameRules(GameTemplate template_name);

  CellState WillCellSurvive(CellState status_quo,
                            std::size_t living_neighbors_count);

  //////////////////////////////////////////////////////////////////

  std::vector<Offset> neighbors;
  std::unordered_set<std::size_t> cell_survival_conditions;
  std::unordered_set<std::size_t> cell_birth_conditions;
};

}  // namespace rule

}  // namespace automata::game_of_life

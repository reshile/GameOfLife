#include "rule.hpp"
#include "cell_state.hpp"
#include "game_template.hpp"

namespace automata::game_of_life {

namespace rule {

////////////////////////////////////////////////////////////////////////////////////

// classic ways to determine the neighborhood
std::vector<Offset> neighborhood_of_moore = {
    {/*x_coordinate*/ -1, /*y_coordinate*/ -1},
    {/*x_coordinate*/ 0, /*y_coordinate*/ -1},
    {/*x_coordinate*/ 1, /*y_coordinate*/ -1},
    {/*x_coordinate*/ 1, /*y_coordinate*/ 0},
    {/*x_coordinate*/ 1, /*y_coordinate*/ 1},
    {/*x_coordinate*/ 0, /*y_coordinate*/ 1},
    {/*x_coordinate*/ -1, /*y_coordinate*/ 1},
    {/*x_coordinate*/ -1, /*y_coordinate*/ 0},
};
std::vector<Offset> neighborhood_of_von_neumann = {
    {/*x_coordinate*/ 0, /*y_coordinate*/ -1},
    {/*x_coordinate*/ 1, /*y_coordinate*/ 0},
    {/*x_coordinate*/ 0, /*y_coordinate*/ 1},
    {/*x_coordinate*/ -1, /*y_coordinate*/ 0},
};

// rule: B2/C23
// Classic Game of live
std::unordered_set<std::size_t> cell_survival_conditions_classic = {2, 3};
std::unordered_set<std::size_t> cell_birth_conditions_classic = {3};

////////////////////////////////////////////////////////////////////////////////////

CellState GameRules::WillCellSurvive(CellState status_quo,
                                     std::size_t living_neighbors_count) {
  if (status_quo == CellState::alive) {
    return static_cast<CellState>(
        cell_survival_conditions.contains(living_neighbors_count));
  }

  // case: status_quo == CellState::dead
  return static_cast<CellState>(
      cell_birth_conditions.contains(living_neighbors_count));
}

GameRules::GameRules()
    : neighbors(neighborhood_of_moore),
      cell_survival_conditions(cell_survival_conditions_classic),
      cell_birth_conditions(cell_birth_conditions_classic) {
}

GameRules::GameRules(std::vector<Offset> neighbor,
                     std::unordered_set<std::size_t> cell_survival_cond,
                     std::unordered_set<std::size_t> cell_birth_cond)
    : neighbors(std::move(neighbor)),
      cell_survival_conditions(std::move(cell_survival_cond)),
      cell_birth_conditions(std::move(cell_birth_cond)) {
}

////////////////////////////////////////////////////////////////////////////////////

// non classic ways to determine the neighborhood
std::vector<automata::game_of_life::rule::Offset> octagon_neighbors = {
    {/*x_coordinate*/ 2, /*y_coordinate*/ 1},
    {/*x_coordinate*/ 1, /*y_coordinate*/ 2},
    {/*x_coordinate*/ -2, /*y_coordinate*/ 1},
    {/*x_coordinate*/ -1, /*y_coordinate*/ 2},
    {/*x_coordinate*/ 2, /*y_coordinate*/ -1},
    {/*x_coordinate*/ 1, /*y_coordinate*/ -2},
    {/*x_coordinate*/ -2, /*y_coordinate*/ -1},
    {/*x_coordinate*/ -1, /*y_coordinate*/ -2},
};
std::vector<automata::game_of_life::rule::Offset> hexagon_neighbors = {
    {/*x_coordinate*/ 0, /*y_coordinate*/ -1},
    {/*x_coordinate*/ 1, /*y_coordinate*/ 0},
    {/*x_coordinate*/ 0, /*y_coordinate*/ 1},
    {/*x_coordinate*/ -1, /*y_coordinate*/ 0},
    {/*x_coordinate*/ 1, /*y_coordinate*/ -1},
    {/*x_coordinate*/ -1, /*y_coordinate*/ 1},
};

////////////////////////////////////////////////////////////////////////////////////

GameRules::GameRules(GameTemplate template_name) {
  if (template_name == GameTemplate::Default) {
    neighbors = neighborhood_of_moore;
    cell_birth_conditions = cell_birth_conditions_classic;
    cell_survival_conditions = cell_survival_conditions_classic;

  } else if (template_name == GameTemplate::SquareWaves) {
    neighbors = neighborhood_of_von_neumann;
    cell_birth_conditions = {1, 2};
    cell_survival_conditions = {1};

  } else if (template_name == GameTemplate::Snowflake) {
    neighbors = hexagon_neighbors;
    cell_birth_conditions = {1, 3, 5, 7};
    cell_survival_conditions = {0, 2, 4, 6, 8};

  } else if (template_name == GameTemplate::BlinkingDiamonds) {
    neighbors = hexagon_neighbors;
    cell_birth_conditions = {1, 2};
    cell_survival_conditions = {1};

  } else if (template_name == GameTemplate::Lol) {
    neighbors = octagon_neighbors;
    cell_survival_conditions = {1};
    cell_birth_conditions = {0, 1, 4, 6, 8};

  } else if (template_name == GameTemplate::Umbrellas) {
    neighbors = octagon_neighbors;
    cell_birth_conditions = {1};
    cell_survival_conditions = {};

  } else if (template_name == GameTemplate::FlashingSquares) {
    neighbors = neighborhood_of_moore;
    cell_birth_conditions = {1};
    cell_survival_conditions = {};
  }
}

////////////////////////////////////////////////////////////////////////////////////

}  // namespace rule

}  // namespace automata::game_of_life

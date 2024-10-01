#include <cstdint>
#include <functional>

#include "cell.hpp"

namespace automata::game_of_life {

bool operator==(const Cell& cell1, const Cell& cell2) {
  return (cell1.x_coordinate == cell2.x_coordinate) &&
         (cell1.y_coordinate == cell2.y_coordinate);
}

std::int64_t HashCell::operator()(const Cell& cell) const {
  return std::hash<std::int64_t>{}(cell.x_coordinate) ^
         std::hash<std::int64_t>{}(cell.y_coordinate);
}

}  // namespace automata::game_of_life

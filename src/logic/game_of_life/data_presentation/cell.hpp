#pragma once

#include <algorithm>
#include <cstdint>

namespace automata::game_of_life {

struct Cell {
  Cell() = default;
  ~Cell() = default;

  /////////////////////////////////////////////////////

  // Copyable
  Cell(const Cell&) = default;
  Cell& operator=(const Cell&) = default;

  /////////////////////////////////////////////////////

  // Movable
  Cell(Cell&& other)
      : x_coordinate(other.x_coordinate),
        y_coordinate(other.y_coordinate) {
    other.y_coordinate = 0;
    other.x_coordinate = 0;
  }
  Cell& operator=(Cell&& other) {
    Cell tmp = std::move(other);
    std::swap(tmp.x_coordinate, x_coordinate);
    std::swap(tmp.y_coordinate, y_coordinate);

    return *this;
  }

  /////////////////////////////////////////////////////

  Cell(std::int64_t x_coord, std::int64_t y_coord)
      : x_coordinate(x_coord),
        y_coordinate(y_coord) {
  }

  /////////////////////////////////////////////////////

  std::int64_t x_coordinate{0};
  std::int64_t y_coordinate{0};
};

bool operator==(const Cell& cell1, const Cell& cell2);

struct HashCell {
  std::int64_t operator()(const Cell& cell) const;
};

}  // namespace automata::game_of_life

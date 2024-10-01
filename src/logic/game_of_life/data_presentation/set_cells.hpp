#pragma once

#include <unordered_set>

#include "cell.hpp"

#include <unordered_set>

namespace automata::game_of_life {

// TODO use custom allocator
using SetCells = std::unordered_set<Cell, HashCell, std::equal_to<Cell>>;

}  // namespace automata::game_of_life

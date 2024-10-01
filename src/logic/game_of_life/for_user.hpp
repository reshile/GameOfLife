#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/task_group.h>
#include "data_presentation/cell_state.hpp"
#include "data_presentation/game_template.hpp"
#include "game_construct.hpp"

namespace automata {

// alive or dead
using automata::game_of_life::CellState;

//  internal representation of a board with live cells
using automata::game_of_life::SetCells;

// Use: GameRule{automata::GameTemplate::Dafault]
// GameTemplate implicit conversion GameRule
using automata::game_of_life::rule::GameTemplate;

// todo
using automata::game_of_life::Game;

}  // namespace automata

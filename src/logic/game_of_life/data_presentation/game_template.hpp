#pragma once

#include <cstddef>
namespace automata::game_of_life {

namespace rule {

// Use: GameRule{automata::GameTemplate::Dafault}
// GameTemplate implicit conversion GameRule
enum GameTemplate : size_t {
  Default = 0,  // game of life
  Umbrellas = 1,
  BlinkingDiamonds = 2,
  Snowflake = 3,
  SquareWaves = 4,
  FlashingSquares = 5,
  Lol = 6,
};

}  // namespace rule

}  // namespace automata::game_of_life
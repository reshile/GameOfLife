#pragma once

#include <cstddef>
#include <string>

#include "../data_presentation/cell.hpp"
#include "../data_presentation/set_cells.hpp"
#include "../data_presentation/game_config.hpp"

namespace automata::game_of_life {

namespace templates {

// Recognizes templates that are in project/resources/template_data_storage/ and
// provides a user-friendly interface for using this data further
class Parser {
 public:
  //////////////////////////////////////////////////////////////////

  Parser() = delete;
  ~Parser() = default;

  // Non-copyable and Non-movable
  Parser(Parser&&) = delete;
  Parser& operator=(Parser&&) = delete;
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;

  //////////////////////////////////////////////////////////////////

  explicit Parser(GameConfig* config);
  void Parse(std::string, Cell);

  //////////////////////////////////////////////////////////////////

 private:
  void ParseRle(std::string, Cell);
  void ParseTxt(std::string, Cell);

  GameConfig* config_;

  //////////////////////////////////////////////////////////////////

 public:
  std::size_t rows_count{0};
  std::size_t columns_count{0};
  SetCells living_cells_;

  //////////////////////////////////////////////////////////////////
};

}  // namespace templates

}  // namespace automata::game_of_life

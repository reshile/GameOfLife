#include <cstddef>
#include <iostream>

#include "np_calculator.hpp"

namespace automata::game_of_life {

namespace executor {

////////////////////////////////////////////////////////////////////////////////////

void NoParallelCalculator::Set(GameConfig* config) {
  config_ = config;
}

void NoParallelCalculator::CellProcessing(SetCells* set, Cell cell,
                                          CellState is_alive) {
  std::size_t living_count = 0;

  for (const auto& [x_offset, y_offset] : config_->rule.neighbors) {
    Cell neighbor;

    if (config_->is_endless_board) {
      neighbor.x_coordinate = cell.x_coordinate + x_offset;
      neighbor.y_coordinate = cell.y_coordinate + y_offset;

    } else {
      neighbor.x_coordinate =
          (cell.x_coordinate + x_offset + config_->rows_count) %
          config_->rows_count;
      neighbor.y_coordinate =
          (cell.y_coordinate + y_offset + config_->columns_count) %
          config_->columns_count;
    }

    if (set->contains(neighbor)) {
      ++living_count;
    }
  }

  if (config_->rule.WillCellSurvive(is_alive, living_count) ==
      CellState::alive) {
    result.insert(cell);
  }
}

void NoParallelCalculator::Clear() {
  result.clear();
}

void NoParallelCalculator::Calc(SetCells* status_quo) {
  // todo : for optimize
  result.reserve(status_quo->size());

  for (const auto& cell : *status_quo) {
    CellProcessing(status_quo, cell, /*is_alive=*/CellState::alive);

    for (const auto& [x_offset, y_offset] : config_->rule.neighbors) {
      if (config_->is_endless_board) {
        CellProcessing(
            /*set=*/status_quo,
            /*cell=*/
            {cell.x_coordinate + x_offset, cell.y_coordinate + y_offset},
            /*is_alive=*/CellState::dead);
      } else {
        CellProcessing(
            /*set=*/status_quo,
            /*cell=*/
            {(cell.x_coordinate + x_offset + config_->rows_count) %
                 config_->rows_count,
             (cell.y_coordinate + y_offset + config_->columns_count) %
                 config_->columns_count},
            /*is_alive=*/CellState::dead);
      }
    }
  }
}

void NoParallelCalculator::Update(SetCells* where) {
  *where = std::move(result);
}

////////////////////////////////////////////////////////////////////////////////////

}  // namespace executor

}  // namespace automata::game_of_life

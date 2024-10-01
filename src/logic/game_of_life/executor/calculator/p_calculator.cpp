#include <atomic>
#include <cstddef>
#include <mutex>
#include <vector>

#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/queuing_mutex.h>
#include <oneapi/tbb/task_arena.h>
#include <tbb/tbb.h>

#include "p_calculator.hpp"

namespace automata::game_of_life {

namespace executor {

////////////////////////////////////////////////////////////////////////////////////

void ParallelCalculator::Set(GameConfig* config) {
  config_ = config;
}

void ParallelCalculator::CellProcessing(std::size_t thread_id, Cell cell,
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

    if (set_->contains(neighbor)) {
      ++living_count;
    }
  }

  if (config_->rule.WillCellSurvive(is_alive, living_count) ==
      CellState::alive) {
    std::lock_guard guard{mutex2_};
    split_response_[thread_id].push_back(cell);
  }
}

void ParallelCalculator::Clear() {
  thread_id_ = 0;
  split_response_.clear();
  result.clear();
}

void ParallelCalculator::Calc(SetCells* status_quo) {
  set_ = status_quo;

  //! Copy O(n)
  std::vector shared_data(set_->begin(), set_->end());

  tbb::parallel_for(
      tbb::blocked_range<std::size_t>(0, shared_data.size()),
      [&shared_data, this](const tbb::blocked_range<std::size_t>& range) {
        std::size_t thread_id;

        {
          std::lock_guard guard{mutex1_};
          thread_id = thread_id_++;
          split_response_.push_back({});
        }

        for (std::size_t iter = range.begin(); iter < range.end(); ++iter) {
          auto cell = shared_data[iter];

          CellProcessing(thread_id, cell, /*is_alive=*/CellState::alive);

          for (const auto& [x_offset, y_offset] : config_->rule.neighbors) {
            if (config_->is_endless_board) {
              CellProcessing(
                  /*thread_id=*/thread_id,
                  /*cell=*/
                  {cell.x_coordinate + x_offset, cell.y_coordinate + y_offset},
                  /*is_alive=*/CellState::dead);
            } else {
              CellProcessing(
                  /*thread_id=*/thread_id,
                  /*cell=*/
                  {(cell.x_coordinate + x_offset + config_->rows_count) %
                       config_->rows_count,
                   (cell.y_coordinate + y_offset + config_->columns_count) %
                       config_->columns_count},
                  /*is_alive=*/CellState::dead);
            }
          }
        }
      });

  //! O(n)
  for (const auto& one_thread_result : split_response_) {
    for (const auto& cell : one_thread_result) {
      result.insert(cell);
    }
  }
}

void ParallelCalculator::Update(SetCells* where) {
  *where = std::move(result);
}

////////////////////////////////////////////////////////////////////////////////////

}  // namespace executor

}  // namespace automata::game_of_life

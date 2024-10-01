// #include <cassert>
// #include <cstddef>
// #include <thread>

// #include "multi_threaded_executor.hpp"
// #include "../templates/parser.hpp"

// namespace automata::game_of_life {

// namespace executor {

// ////////////////////////////////////////////////////////////////////////////////////////////

// MultiThreadedExecutor::~MultiThreadedExecutor() {
//   assert(executor_state_ == ExecutorState::stop &&
//          "you must call STOP before completing the logic of the game");
// }

// ////////////////////////////////////////////////////////////////////////////////////////////

// void MultiThreadedExecutor::SetlRules(rule::GameRules&& rule,
//                                       std::size_t row_count,
//                                       std::size_t columns_count) {
//   // For work
//   config_.rule = std::move(rule);
//   config_.rows_count = row_count;
//   config_.columns_count = columns_count;

//   // For more information about the game
//   info.number_of_living = 0;
//   info.iteration = 0;
// }

// void MultiThreadedExecutor::SetUpdateFrequency(sf::Int32 uf,
//                                                std::size_t speed = 1) {
//   update_frequency_ = uf;
//   timer_.restart();
//   iter_count_ = speed;
// }

// void MultiThreadedExecutor::SetTemplate(std::string template_name, Cell
// where) {
//   // Set template
//   templates::Parser parse(&config_);
//   parse.Parse(std::move(template_name), where);

//   //! Updating board data
//   for_render_->merge(std::move(parse.living_cells_));
//   cache_.Invalidate();
// }

// ////////////////////////////////////////////////////////////////////////////////////////////

// void MultiThreadedExecutor::Start() {
//   timer_.restart();

//   calculator_.Set(&config_);

//   executor_state_ = run;
//   machine_.Start();

//   worker_ = std::thread([this]() {
//     Work();
//   });
// }

// void MultiThreadedExecutor::Stop() {
//   executor_state_ = stop;  // end executing
//   machine_.Stop();         // stop working
//   worker_.join();          // stop thread
// }

// void MultiThreadedExecutor::Pause() {
//   executor_state_ = pause;
// }

// void MultiThreadedExecutor::ReStart() {
//   executor_state_ = run;
// }

// void MultiThreadedExecutor::Clear() {
//   for_render_->clear();
//   cache_.Invalidate();
// }

// // const SetCells& MultiThreadedExecutor::Step() {
// // }

// // SetCells& MultiThreadedExecutor::TakeCells() {
// // TODO
// // return cache_.GetCache();
// // }

// ////////////////////////////////////////////////////////////////////////////////////////////

// void MultiThreadedExecutor::Work() {
//   while (machine_.IsWork()) {
//     std::size_t zero = 0;

//     machine_.Wait();
//   }
// }

// ////////////////////////////////////////////////////////////////////////////////////////////

// }  // namespace executor

// }  // namespace automata::game_of_life

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>

#include "../logic/game_of_life/for_user.hpp"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "grid_shell.hpp"
#include "user_interface.hpp"

#include "user_interface_shell.hpp"

using automata::CellState;
using automata::Game;

struct VisibleArea {
  int from_x = 0;
  int to_x = 0;
  int from_y = 0;
  int to_y = 0;
};

struct Grid {
  size_t map_size_; // deprecate in newer version
  size_t max_live_cells_ = 1000000;
  constexpr static float kGridSizeF = 40.f;
  constexpr static size_t kGridSize = 40;
  constexpr static unsigned kGridSizeU = static_cast<unsigned>(kGridSizeF);
  static size_t kRenderWidthLeft;    // NOLINT
  static size_t kRenderWidthLRight;  // NOLINT
  static size_t kRenderHeightUp;     // NOLINT
  static size_t kRenderHeightDown;   // NOLINT
  VisibleArea area;

  static float zoom;
  sf::View view_;
  sf::RenderWindow window_;
  std::vector<std::vector<sf::RectangleShape>> tile_map_;
  std::vector<sf::Vertex> map_;
  std::vector<sf::RectangleShape> left_border_;
  std::vector<sf::RectangleShape> right_border_;
  std::vector<sf::RectangleShape> top_border_;
  std::vector<sf::RectangleShape> bottom_border_;

  static sf::VideoMode desktop_;  // NOLINT

  sf::RectangleShape selector_;

  static sf::Vector2i mouse_pos_screen_;  // NOLINT
  static sf::Vector2i mouse_pos_window_;  // NOLINT
  static sf::Vector2f mouse_pos_view_;    // NOLINT
  static sf::Vector2u mouse_pos_grid_;    // NOLINT

  // Game controls

  size_t update_rate = 50;
  float view_speed_ = 40.f;
  static float view_coeff_;  // NOLINT
  float dt_ = 1.f;

  bool borders_on_ = true;
  bool paused_ = false;

  // !!! For debug now  !!! //
  static sf::Font font_;  // NOLINT
  sf::Font ff;
  sf::Text text_;

  Grid() = default;

  explicit Grid(size_t);

  // Non-copyable
  Grid(const Grid&) = delete;
  Grid& operator=(const Grid&) = delete;

  // All below functions decomposed here (!!! ViewSetup is in Grid)
  void SetUp();

  // Create window
  void SetUpWindow();

  // Initialize play grid of map_size * map_size
  void InitializeGrid();

  // Set inital postion of views
  void SetUpView();

  void UpdateDebug();

  // Setup selector for mouse
  void SetUpSelector();
  void SetUpDebug();

  // Render current visible area
  void RenderGrid(Game&);

  sf::View& GetView();

  sf::RenderWindow& GetWindow();

  static float GridSizeF();

  static float GridSizeU();

  void UpdateInput();

  void UpdateSelector();

  void CalculateMousePosition();

  sf::RectangleShape& GetSelector();

  // For Grid
  void ZoomIn();
  void ZoomOut();

  // For GameControl
  void Pause(Game&);
  void Restart(Game&);
  bool Paused();

  // Finish later
  void Update();
  void Render(Game&);

  void UpdateUserClick(Game&, UserInterface&);

  // Maybe better for game control block
  void SpeedUp(Game&);
  void SlowDown(Game&);
};
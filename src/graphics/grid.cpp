#include "grid.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "user_interface.hpp"
#include <cstddef>
#include <iostream>
#include <sstream>

sf::VideoMode Grid::desktop_ = sf::VideoMode::getDesktopMode();

size_t Grid::kRenderWidthLeft = 0;
size_t Grid::kRenderWidthLRight = 0;
size_t Grid::kRenderHeightUp = 0;
size_t Grid::kRenderHeightDown = 0;
float Grid::view_coeff_ = 0.25f;

float Grid::zoom = 1.f;

sf::Font Grid::font_ = sf::Font();

sf::Vector2i Grid::mouse_pos_screen_ = sf::Vector2i();
sf::Vector2i Grid::mouse_pos_window_ = sf::Vector2i();
sf::Vector2f Grid::mouse_pos_view_ = sf::Vector2f();
sf::Vector2u Grid::mouse_pos_grid_ = sf::Vector2u();

Grid::Grid(size_t map_size)
    : map_size_(map_size) {
}

void Grid::SetUp() {
  SetUpWindow();
  SetUpView();
  InitializeGrid();
  SetUpSelector();
  SetUpDebug();  // REMOVE LATER
}

void Grid::SetUpWindow() {
  window_.create(sf::VideoMode::getDesktopMode(), "test run",
                 sf::Style::Default);
  window_.setTitle("Game of Life");
  window_.setFramerateLimit(60);
}

void Grid::InitializeGrid() {
  map_.resize(6 * max_live_cells_);
  
  tile_map_.resize(map_size_, std::vector<sf::RectangleShape>(
                                  map_size_, sf::RectangleShape()));

  top_border_.resize(map_size_ + 1);
  for (size_t x = 0; x < top_border_.size(); ++x) {
    top_border_[x].setSize(sf::Vector2f(kGridSizeF, kGridSizeF));
    top_border_[x].setFillColor(sf::Color::White);
    top_border_[x].setPosition(x * kGridSizeF, 0);
  }

  bottom_border_.resize(map_size_ + 1);
  for (size_t x = 0; x < bottom_border_.size(); ++x) {
    bottom_border_[x].setSize(sf::Vector2f(kGridSizeF, kGridSizeF));
    bottom_border_[x].setFillColor(sf::Color::White);
    bottom_border_[x].setPosition(x * kGridSizeF,
                                  (bottom_border_.size() - 1) * kGridSizeF);
  }

  left_border_.resize(map_size_ + 1);
  for (size_t y = 0; y < left_border_.size(); ++y) {
    left_border_[y].setSize(sf::Vector2f(kGridSizeF, kGridSizeF));
    left_border_[y].setFillColor(sf::Color::White);
    left_border_[y].setPosition(0, y * kGridSizeF);
  }

  right_border_.resize(map_size_ + 1);
  for (size_t y = 0; y < right_border_.size(); ++y) {
    right_border_[y].setSize(sf::Vector2f(kGridSizeF, kGridSizeF));
    right_border_[y].setFillColor(sf::Color::White);
    right_border_[y].setPosition(map_size_ * kGridSizeF, y * kGridSizeF);
  }

  for (size_t x = 0; x < map_size_; ++x) {
    for (size_t y = 0; y < map_size_; ++y) {
      tile_map_[x][y].setSize(sf::Vector2f(kGridSizeF, kGridSizeF));
      tile_map_[x][y].setFillColor(sf::Color::Black);
      // tile_map_[x][y].setOutlineThickness(-1.f);
      // tile_map_[x][y].setOutlineColor(sf::Color::Black);
      tile_map_[x][y].setPosition((x + 1) * kGridSizeF, (y + 1) * kGridSizeF);
    }
  }
}

void Grid::SetUpView() {
  float center_x = ((1 + view_coeff_) * desktop_.width) / 2;

  kRenderWidthLeft =
      (size_t((1 - view_coeff_) * desktop_.width / kGridSizeF) / 2);
  kRenderWidthLRight = kRenderWidthLeft;
  kRenderHeightUp = (size_t(desktop_.height / kGridSizeF) / 2);
  kRenderHeightDown = kRenderHeightUp;

  view_.setCenter(center_x, desktop_.height / 2.f);
  view_.setSize(desktop_.width * (1 - view_coeff_), desktop_.height);
  view_.setViewport(sf::FloatRect(
      view_coeff_, 0.f, (1 - view_coeff_),
      1.f));  // Set the viewport to scale the view down to half the window size
  view_.move(center_x - 150, desktop_.height + 300);

  ZoomOut();
  ZoomOut();
}

// *** Indicates the grid where mouse is poiningf ***
void Grid::SetUpSelector() {
  selector_.setSize(sf::Vector2f(Grid::GridSizeF(), Grid::GridSizeF()));
  selector_.setFillColor(sf::Color::Transparent);
  selector_.setOutlineThickness(1.f);
  selector_.setOutlineColor(sf::Color::Green);
}

void Grid::RenderGrid(Game& game) {
  auto& cells = game.OneIter();

  area.from_x = (view_.getCenter().x / kGridSizeF) - kRenderWidthLeft * zoom;
  area.to_x = (view_.getCenter().x / kGridSizeF) + kRenderWidthLRight * zoom;
  area.from_y = (view_.getCenter().y / kGridSizeF) - kRenderHeightUp * zoom;
  area.to_y = (view_.getCenter().y / kGridSizeF) + kRenderHeightDown * zoom;

  if (area.from_x < 0) {
    area.from_x = 0;
  } else if (area.from_x >= map_size_) {
    area.from_x = map_size_ - 1;
  }

  if (area.to_x < 0) {
    area.to_x = 0;
  } else if (area.to_x >= map_size_) {
    area.to_x = map_size_ - 1;
  }

  if (area.from_y < 0) {
    area.from_y = 0;
  } else if (area.from_y >= map_size_) {
    area.from_y = map_size_ - 1;
  }

  if (area.to_y < 0) {
    area.to_y = 0;
  } else if (area.to_y >= map_size_) {
    area.to_y = map_size_ - 1;
  }

  if (borders_on_) {
    for (size_t x = 0; x < top_border_.size(); ++x) {
      window_.draw(top_border_[x]);
      window_.draw(bottom_border_[x]);
    }

    for (size_t y = 0; y < left_border_.size(); ++y) {
      window_.draw(left_border_[y]);
      window_.draw(right_border_[y]);
    }
  }

  int64_t xcord;
  int64_t ycord;
  size_t cnt = 0;
  for (const auto& cell : cells) {
    // tile_map_[cell.x_coordinate][cell.y_coordinate].setFillColor(
        // sf::Color::White);
    sf::Vertex* triangles = &map_[cnt * 6];
    int64_t xcord = cell.x_coordinate;
    int64_t ycord = cell.y_coordinate;

    triangles[0].position = sf::Vector2f(xcord * kGridSize, ycord * kGridSize);
    triangles[1].position = sf::Vector2f((xcord + 1) * kGridSize, ycord * kGridSize);
    triangles[2].position = sf::Vector2f(xcord * kGridSize, (ycord + 1) * kGridSize);
    triangles[3].position = sf::Vector2f(xcord * kGridSize, (ycord + 1) * kGridSize);
    triangles[4].position = sf::Vector2f((xcord + 1) * kGridSize, ycord * kGridSize);
    triangles[5].position = sf::Vector2f((xcord + 1) * kGridSize, (ycord + 1) * kGridSize);

    triangles[0].color = sf::Color::White;
    triangles[1].color = sf::Color::White;
    triangles[2].color = sf::Color::White;
    triangles[3].color = sf::Color::White;
    triangles[4].color = sf::Color::White;
    triangles[5].color = sf::Color::White;

    ++cnt;

    // window_.draw(tile_map_[cell.x_coordinate][cell.y_coordinate]);
  }

  window_.draw(&map_[0], cnt * 6, sf::Triangles);
}

float Grid::GridSizeF() {
  return kGridSizeF;
}

float Grid::GridSizeU() {
  return kGridSizeU;
}

// *** 2D camera movements. Limited movements behind the borders allowed ***
void Grid::UpdateInput() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {  // LEFT
    if ((float)(view_.getCenter().x / kGridSizeF) - area.from_x >=
        -int(map_size_ + 1) / 2.f) {
      view_.move(-view_speed_ * dt_, 0.f);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {  // RIGHT
    if (area.to_x - (float)(view_.getCenter().x / kGridSizeF) >=
        -int(map_size_ + 1) / 2.f) {
      view_.move(view_speed_ * dt_, 0.f);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {  // UP
    if ((float)(view_.getCenter().y / kGridSizeF) - area.from_y >=
        -int(map_size_ + 1) / 2.f) {
      view_.move(0.f, -view_speed_ * dt_);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {  // DOWN
    if (area.to_y - (float)(view_.getCenter().y / kGridSizeF) >=
        -int(map_size_ + 1) / 2.f) {
      view_.move(0.f, view_speed_ * dt_);
    }
  }
}

// *** Custom drawings allowed only when game paused. Also, custom erasurers
// can be made if no template captured. ***
void Grid::UpdateUserClick(Game& rt, UserInterface& ui) {
  if (Paused() && !ui.MouseInPanel(
                      sf::Vector2f(mouse_pos_screen_.x, mouse_pos_screen_.y))) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      if (mouse_pos_grid_.x > area.from_x && mouse_pos_grid_.x <= area.to_x &&
          mouse_pos_grid_.y > area.from_y && mouse_pos_grid_.y <= area.to_y) {
        rt.SetNewCell({mouse_pos_grid_.x - 1, mouse_pos_grid_.y - 1});
        tile_map_[mouse_pos_grid_.x][mouse_pos_grid_.y].setFillColor(
            sf::Color::White);
      }
    } else if (!ui.TemplateCaptured() &&
               sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
      if (mouse_pos_grid_.x > area.from_x && mouse_pos_grid_.x <= area.to_x &&
          mouse_pos_grid_.y > area.from_y && mouse_pos_grid_.y <= area.to_y) {
        rt.DeleteCell({mouse_pos_grid_.x - 1, mouse_pos_grid_.y - 1});
        tile_map_[mouse_pos_grid_.x][mouse_pos_grid_.y].setFillColor(
            sf::Color::Black);
      }
    }
  }
}

void Grid::UpdateSelector() {
  selector_.setPosition(mouse_pos_grid_.x * Grid::GridSizeF(),
                        mouse_pos_grid_.y * Grid::GridSizeF());
}

void Grid::CalculateMousePosition() {
  mouse_pos_screen_ = sf::Mouse::getPosition();
  mouse_pos_window_ = sf::Mouse::getPosition(window_);
  window_.setView(view_);
  mouse_pos_view_ = window_.mapPixelToCoords(mouse_pos_window_);
  if (mouse_pos_view_.x >= 0.f) {
    mouse_pos_grid_.x = mouse_pos_view_.x / kGridSizeU;
  }
  if (mouse_pos_view_.y >= 0.f) {
    mouse_pos_grid_.y = mouse_pos_view_.y / kGridSizeU;
  }
  window_.setView(window_.getDefaultView());
}

void Grid::SetUpDebug() {
  // ff.loadFromFile(
  //     "/home/alik/Desktop/4_semestr/tp_proj/nurik-alik-tp/"
  //     "NotoSerif-Italic[wght].ttf");
  // text_.setFont(ff);
  // text_.setCharacterSize(24);
  // text_.setFillColor(sf::Color::White);
}

void Grid::UpdateDebug() {
  std::stringstream ss;
  ss << "Screen: " << mouse_pos_screen_.x << " " << mouse_pos_screen_.y << '\n'
     << "Window: " << mouse_pos_window_.x << " " << mouse_pos_window_.y << '\n'
     << "View: " << mouse_pos_view_.x << " " << mouse_pos_view_.y << '\n'
     << "Xlimits: " << area.from_x << " " << area.to_x << '\n'
     << "Ylimits: " << area.from_y << " " << area.to_y << '\n'
     << "Grid: " << mouse_pos_grid_.x << " " << mouse_pos_grid_.y << '\n'
     << "Center: " << (unsigned)(view_.getCenter().x / kGridSizeU) << " "
     << (unsigned)(view_.getCenter().y / kGridSizeU) << '\n'
     << "kWidLeft: " << kRenderWidthLeft << '\n'
     << "kWidRight: " << kRenderWidthLRight << '\n'
     << "kHeiUp: " << kRenderHeightUp << '\n'
     << "kHeiDown " << kRenderHeightDown << '\n'
     << "Zoom: " << zoom << '\n';
  ss << "State: Paused" << '\n';

  text_.setString(ss.str());
}

sf::RectangleShape& Grid::GetSelector() {
  return selector_;
}

void Grid::ZoomIn() {
  if (zoom >= 0.5) {
    zoom *= 0.5;
    view_.zoom(0.5f);
  }
}

void Grid::ZoomOut() {
  if (zoom <= 8) {
    zoom *= 2;
    view_.zoom(2.f);
  }
}

void Grid::Pause(Game& Game) {
  paused_ = true;
  Game.Pause();
}

void Grid::Restart(Game& Game) {
  paused_ = false;
  Game.ReStart();
}

bool Grid::Paused() {
  return paused_;
}

sf::RenderWindow& Grid::GetWindow() {
  return window_;
}

sf::View& Grid::GetView() {
  return view_;
}

void Grid::Update() {
  CalculateMousePosition();
  UpdateSelector();

  // Poll Event

  //

  UpdateInput();

  window_.clear();
  window_.setView(view_);
  window_.draw(selector_);
}

void Grid::Render(Game& rt) {
  window_.clear();
  window_.setView(view_);

  RenderGrid(rt);
}

void Grid::SpeedUp(Game& rt) {
  rt.IncreaseGameSpeed();
}

void Grid::SlowDown(Game& rt) {
  rt.DecreaseGameSpeed();
}

#include "event_handler.hpp"

sf::Event EventHandler::event = sf::Event();
sf::Clock EventHandler::timer = sf::Clock();

void EventHandler::Handle(Grid& grid, UserInterface& ui, Game& rt, Menu& menu) {
  if (timer.getElapsedTime().asMilliseconds() >= 50) {
    while (grid.GetWindow().pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          CloseWindow(grid, ui, rt);
          break;
        case sf::Event::KeyPressed:
          KeyPressed(grid, ui, rt, menu);
          break;
        case sf::Event::MouseWheelScrolled:
          MouseWheelScrolleed(grid, ui, rt);
          break;
        case sf::Event::MouseButtonPressed:
          MouseButtonPressed(grid, ui, rt);
          break;
        case sf::Event::MouseButtonReleased:
          MouseButtonReleased(grid, ui, rt);
          break;
        default:
          break;
      }
    }
    timer.restart();
  }
}

void EventHandler::CloseWindow(Grid& grid, UserInterface& ui, Game& rt) {
  grid.GetWindow().close();
  rt.OneLoopStop();
}

void EventHandler::MouseWheelScrolleed(Grid& grid, UserInterface& ui,
                                       Game& rt) {
  if (ui.MouseInPanel(
          sf::Vector2f(grid.mouse_pos_screen_.x, grid.mouse_pos_screen_.y))) {
    if (event.mouseWheelScroll.delta < 0) {
      ui.scroll_area_move += 10.f;
      ui.scroll_area_.move(0.f, 10.f);
    } else if (event.mouseWheelScroll.delta > 0) {
      ui.scroll_area_move -= 10.f;
      ui.scroll_area_.move(0.f, -10.f);
    }
  }
}

void EventHandler::MouseButtonPressed(Grid& grid, UserInterface& ui, Game& rt) {
  if (ui.left_ui_.getGlobalBounds().contains(
          sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
    ui.MouseClicked(grid, rt);
    return;
  }

  if (ui.TemplateCaptured() && event.mouseButton.button == sf::Mouse::Right) {
    rt.SetTemplate(ui.GetTemplate(),
                   {Grid::mouse_pos_grid_.x, Grid::mouse_pos_grid_.y});
  }
}

void EventHandler::MouseButtonReleased(Grid& grid, UserInterface& ui,
                                       Game& rt) {
  ui.MouseReleased();
}

void EventHandler::KeyPressed(Grid& grid, UserInterface& ui, Game& rt,
                              Menu& menu) {
  if (event.key.scancode == sf::Keyboard::Scan::Num1) {
    grid.ZoomOut();
  } else if (event.key.scancode == sf::Keyboard::Scan::Num2) {
    grid.ZoomIn();
  } else if (event.key.scancode == sf::Keyboard::Scan::Backspace) {
    ui.ReleaseTemplate();
  } else if (event.key.scancode == sf::Keyboard::Scan::LShift) {
    rt.Clear();
  } else if (event.key.scancode == sf::Keyboard::Scan::Space) {
    if (grid.Paused()) {
      ui.HandleResume();
      grid.Restart(rt);
    } else {
      ui.HandlePause();
      grid.Pause(rt);
    }
  } else if (event.key.scancode == sf::Keyboard::Scan::Up) {
    grid.SpeedUp(rt);
  } else if (event.key.scancode == sf::Keyboard::Scan::Down) {
    grid.SlowDown(rt);
  } else if (event.key.scancode == sf::Keyboard::Scan::Escape) {
    grid.Pause(rt);
    menu.Resume();
  }
}
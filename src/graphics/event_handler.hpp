#pragma once

#include "SFML/System/Clock.hpp"
#include "grid.hpp"
#include "user_interface.hpp"
#include "../logic/game_of_life/for_user.hpp"
#include "main_menu.hpp"

struct EventHandler {
  static sf::Event event;
  static sf::Clock timer;

  static void Handle(Grid&, UserInterface&, Game&, Menu&);

  // Delegate obligations
  static void CloseWindow(Grid&, UserInterface&, Game&);
  static void MouseWheelScrolleed(Grid&, UserInterface&, Game&);
  static void MouseButtonPressed(Grid&, UserInterface&, Game&);
  static void MouseButtonReleased(Grid&, UserInterface&, Game&);
  static void KeyPressed(Grid&, UserInterface&, Game&, Menu&);
};
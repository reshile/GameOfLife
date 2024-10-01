#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../logic/game_of_life/for_user.hpp"
#include "buttons.hpp"
#include "grid_shell.hpp"

enum MenuState : uint32_t {
  Main = 0,
  Templates = 1,
  Rules = 2,
  Modes = 3,
};

struct UserInterface {
  sf::View& view_;
  sf::View scroll_area_;
  sf::RenderWindow& window_;

  sf::RectangleShape left_ui_;
  Buttons buttons_;
  Buttons menu_;
  Buttons rules_;
  Buttons modes_;

  sf::VideoMode desktop_ = sf::VideoMode::getDesktopMode();

  ////////////////////////////////////////////////////

  uint32_t menu_state = MenuState::Main;
  size_t game_state = 1; // Default

  sf::Font font;
  sf::Text game_paused_;
  sf::Text template_;
  sf::Text living_cells_;
  std::string pause_msg = "Game paused";
  std::string templ_msg = "Template: ";
  std::string living_msg = "Cells alive: ";

  std::string template_name;
  bool template_captured = false;

  ////////////////////////////////////////////////////

  float scroll_area_move = 0;

  ////////////////////////////////////////////////////

  sf::Text menu_text;

  ////////////////////////////////////////////////////

  UserInterface() = delete;

  UserInterface(sf::View&, sf::RenderWindow&);

  // Components below combined
  void SetUp();

  void SetUpScrollArea();

  void SetUpLeftPanel();

  void SetUpButtons();

  void SetUpText();

  void Draw();

  void HandlePause();
  void HandleResume();

  void SetTemplate(std::string);
  void ReleaseTemplate();
  bool TemplateCaptured();
  std::string GetTemplate();

  void MouseClicked(Grid&, automata::Game&);
  void HandleMenu(std::string);

  void MouseReleased();

  void RenderUI();

  sf::View& GetScrollArea();

  bool MouseInPanel(const sf::Vector2f&);

  static float GetPanelWidth();

  void SetUpMenu();
  void SetUpRules();
  void SetUpModes();

  void ResetScrollArea();

  void UpdateLivingCells(automata::Game&);
};
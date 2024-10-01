#include "user_interface.hpp"
#include <SFML/System/Vector2.hpp>
#include <sstream>
#include <string>
#include "SFML/Graphics/View.hpp"
#include "grid.hpp"
#include "main_menu.hpp"
// #include "SFML/Window/Mouse.hpp"

UserInterface::UserInterface(sf::View& view, sf::RenderWindow& window)
    : view_(view),
      window_(window) {
}

void UserInterface::SetUpScrollArea() {
  scroll_area_.setCenter((Grid::view_coeff_ * desktop_.width) / 2,
                         desktop_.height / 2.f);
  scroll_area_.setSize(Grid::view_coeff_ * desktop_.width, desktop_.height);
  scroll_area_.setViewport(sf::FloatRect(0.f, 0.f, Grid::view_coeff_, 1.f));
}

void UserInterface::SetUpLeftPanel() {
  left_ui_.setPosition(0, 0);
  left_ui_.setSize(
      sf::Vector2f(Grid::view_coeff_ * desktop_.width, desktop_.height * 3));
  left_ui_.setFillColor(sf::Color::Transparent);
}

void UserInterface::SetUpText() {
  font.loadFromFile("../resources/PressStart2P-Regular.ttf");
  game_paused_.setCharacterSize(24);
  game_paused_.setFillColor(sf::Color::Green);
  game_paused_.setFont(font);

  template_.setCharacterSize(24);
  template_.setFillColor(sf::Color::Green);
  template_.setFont(font);

  living_cells_.setCharacterSize(24);
  living_cells_.setFillColor(sf::Color::Green);
  living_cells_.setFont(font);
  living_cells_.setPosition(scroll_area_.getSize().x, 0);

  game_paused_.setString("Game paused");
  game_paused_.setPosition(
      desktop_.width - game_paused_.getGlobalBounds().width - 10.f, 0);
  game_paused_.setString("");

  menu_text.setFont(font);
  menu_text.setCharacterSize(64);
  menu_text.setFillColor(sf::Color::White);
  menu_text.setString("Game Of Life");
  menu_text.setPosition(
      (desktop_.width - menu_text.getGlobalBounds().width) / 2,
      (desktop_.height - menu_text.getGlobalBounds().height) / 6);
}

void UserInterface::SetUp() {
  SetUpLeftPanel();
  SetUpScrollArea();
  SetUpButtons();
  SetUpText();
  SetUpMenu();
  SetUpRules();
  SetUpModes();
}

void UserInterface::Draw() {
  // Before UI view set

  ////////////////////////

  window_.draw(game_paused_);
  window_.draw(template_);
  window_.draw(living_cells_);

  ////////////////////////

  // UI view set
  window_.setView(scroll_area_);
  window_.draw(left_ui_);
  switch (menu_state) {
    case Main:
      menu_.DrawButtons(window_);
      break;
    case Templates:
      buttons_.DrawButtons(window_);
      break;
    case Rules:
      rules_.DrawButtons(window_);
      break;
    case Modes:
      modes_.DrawButtons(window_);
      break;
    default:
      break;
  }
}

void UserInterface::SetUpButtons() {
  buttons_.SetButton("<--");
  buttons_.SetButton("Glider");
  buttons_.SetButton("Pulsar");
  buttons_.SetButton("Pulsars");
  buttons_.SetButton("Quasar");
  buttons_.SetButton("Quasar2");
  buttons_.SetButton("Quasar3");
  buttons_.SetButton("Snark");
  buttons_.SetButton("Scholar");
  buttons_.SetButton("Magic Snark");
  buttons_.SetButton("Sir Robin");
  buttons_.SetButton("Spider");
  buttons_.SetButton("Backrake 2");
  buttons_.SetButton("Pufferfish rake");
  buttons_.SetButton("Blinker puffer 2");
  buttons_.SetButton("Slow puffer 1");
  buttons_.SetButton("Hivenudger 2");
  buttons_.SetButton("Piorbital3");
  buttons_.SetButton("Frothing puffer");
  buttons_.SetButton("Puffer blinker");
  buttons_.SetButton("Gunstar 2");
  buttons_.SetButton("Glider shuttle");
  buttons_.SetButton("Nurik");
}

void UserInterface::HandleMenu(std::string str) {
  if (str == "Templates") {
    menu_state = Templates;
    Grid::view_coeff_ = 0.25f;
  } else if (str == "Rules") {
    menu_state = Rules;
    Grid::view_coeff_ = 0.25f;
  } else if (str == "Modes") {
    menu_state = Modes;
    Grid::view_coeff_ = 0.25f; 
  }
  ResetScrollArea();
}

void UserInterface::MouseClicked(Grid& grid, Game& rt) {
  std::string name{};
  window_.setView(scroll_area_);
  auto mouse_pos_view = window_.mapPixelToCoords(Grid::mouse_pos_window_);
  switch (menu_state) {
    case Main:
      for (auto& button : menu_.buttons) {
        if (button.Clicked(mouse_pos_view)) {
          name = button.ClickHandle();
          HandleMenu(std::move(name));
          break;
        }
      }
      break;
    case Templates:
      for (auto& button : buttons_.buttons) {
        if (button.Clicked(mouse_pos_view)) {
          name = button.ClickHandle();
          if (name == "<--") {
            menu_state = Main;
            ResetScrollArea();
            // Grid::view_coeff_ = 0.20f;
            // SetUpLeftPanel();
            // SetUpScrollArea();
            break;
          }
          SetTemplate(std::move(name));
          break;
        }
      }
      break;
    case Rules:
      for (auto& button : rules_.buttons) {
        if (button.Clicked(mouse_pos_view)) {
          size_t id = button.GetId();
          if (id == 0) {
            menu_state = Main;
            ResetScrollArea();
            // Grid::view_coeff_ = 0.20f;
            // SetUpLeftPanel();
            // SetUpScrollArea();
            break;
          }
          if (game_state != id) {
            game_state = id;
            rt.OneLoopStop();
            rt.SetRule(automata::game_of_life::rule::GameTemplate(id - 1));
            rt.OneLoopStart();
            grid.Pause(rt);
            HandlePause();
          }
          break;
        }

      }
      break;
    case Modes:
      for (auto& button : modes_.buttons) {
        if (button.Clicked(mouse_pos_view)) {
          name = button.ClickHandle();
          if (name == "<--") {
            menu_state = Main;
            ResetScrollArea();
            break;
          } else if (name == "Borders") {
            if (grid.borders_on_) {
              grid.borders_on_ = false;
            } else {
            grid.borders_on_ = true;
            }
          }
        }
      }
    default:
      break;
  }
  window_.setView(window_.getDefaultView());
}

void UserInterface::MouseReleased() {
  switch (menu_state) {
    case Main:
      for (auto& button : menu_.buttons) {
        button.ReleaseHandle();
      }
      break;
    case Templates:
      for (auto& button : buttons_.buttons) {
        button.ReleaseHandle();
      }
      break;
    case Rules:
      for (auto& button : rules_.buttons) {
        button.ReleaseHandle();
      }
      break;
    default:
      break;
  }
}

// For later
void UserInterface::RenderUI() {
  window_.setView(window_.getDefaultView());
  window_.setView(scroll_area_);
  Draw();
  window_.display();
}

sf::View& UserInterface::GetScrollArea() {
  return scroll_area_;
}

bool UserInterface::MouseInPanel(const sf::Vector2f& mouse_pos) {
  return left_ui_.getGlobalBounds().contains(mouse_pos);
}

void UserInterface::HandlePause() {
  game_paused_.setString("Game paused");
  template_.setPosition(desktop_.width - template_.getGlobalBounds().width - 10,
                        game_paused_.getGlobalBounds().height + 5);
}

void UserInterface::HandleResume() {
  game_paused_.setString("");
  template_.setPosition(desktop_.width - template_.getGlobalBounds().width - 10,
                        game_paused_.getGlobalBounds().height + 5);
}

void UserInterface::SetTemplate(std::string str) {
  std::stringstream ss;
  template_name = std::move(str);
  ss << templ_msg << template_name;
  template_.setString(ss.str());
  template_.setPosition(desktop_.width - template_.getGlobalBounds().width - 10,
                        game_paused_.getGlobalBounds().height + 5);
  template_captured = true;
}

void UserInterface::ReleaseTemplate() {
  template_.setString("");
  template_captured = false;
  template_name = "";
}

bool UserInterface::TemplateCaptured() {
  return template_captured;
}

std::string UserInterface::GetTemplate() {
  return template_name;
}

float UserInterface::GetPanelWidth() {
  return Grid::view_coeff_ * Grid::desktop_.width;
}

void UserInterface::ResetScrollArea() {
  scroll_area_.move(0.f, -scroll_area_move);
  scroll_area_move = 0;
}

void UserInterface::SetUpMenu() {
  menu_.button_height = menu_.button_width;
  menu_.margin = -25.f;  // Update later
  menu_.SetButton("Rules");
  menu_.SetButton("Templates");
  menu_.SetButton("Modes");
}

void UserInterface::SetUpRules() {
  rules_.SetButton("<--");
  rules_.SetButton("Default");
  rules_.SetButton("Umbrellas");
  rules_.SetButton("BlinkingDiamonds");
  rules_.SetButton("Snowflake");
  rules_.SetButton("SquareWaves");
  rules_.SetButton("FlashingSquares");
  rules_.SetButton("Lol");
}

void UserInterface::SetUpModes() {
  modes_.SetButton("<--");
  modes_.SetButton("Borders");
}

void UserInterface::UpdateLivingCells(automata::Game& rt) {
  auto info = rt.Info();
  living_cells_.setString(living_msg + std::to_string(info.number_of_living));
}
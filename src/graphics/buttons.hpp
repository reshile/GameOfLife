#pragma once

#include <SFML/Graphics.hpp>
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include <vector>
#include <string>

struct Button {
  sf::RectangleShape shape;
  sf::Text text;
  size_t id;

  bool Clicked(const sf::Vector2f&);

  std::string ClickHandle();

  void ReleaseHandle();

  size_t GetId();
};

struct Buttons {

  Buttons() {
    SetUp();
  }

  std::vector<Button> buttons;
  float button_width;
  float button_margin_coeff;
  float button_height;
  float margin_top;
  float margin_left;
  float outline_thickness;
  size_t last_button_id;
  float margin;

  float text_margin_left;
  float text_margin_top;

  sf::Color font_color;
  size_t char_size;
  sf::Font font;

  void SetUp();

  void SetButton(std::string);

  void DrawButtons(sf::RenderWindow&);

};
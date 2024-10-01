#include "buttons.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Window.hpp"

#include "grid.hpp"
#include "user_interface.hpp"

void Buttons::SetUp() {
  font.loadFromFile("../resources/PressStart2P-Regular.ttf");
  button_margin_coeff = 0.1f;
  margin = 0.f;
  button_width = UserInterface::GetPanelWidth() * (1 - 2 * button_margin_coeff);
  button_height = 80.f;
  margin_top = 10.f;
  margin_left = 60.f;
  outline_thickness = 1.f;
  last_button_id = 0;
  char_size = 24;
  font_color = sf::Color::White;

  text_margin_left = 0.f;
  text_margin_top = 10.f;
}


void Buttons::SetButton(std::string text) {
  Button button;
  button.shape.setSize(sf::Vector2f(button_width, button_height));
  button.shape.setPosition(margin_left, last_button_id * button_height +
                                  (last_button_id + 1) * margin_top + margin);
  button.shape.setFillColor(sf::Color::Transparent);
  // button.shape.setOutlineThickness(outline_thickness); // For debug tests
  // button.shape.setOutlineColor(sf::Color::Magenta);  // NURIK DON'T TOUCH!!!
 

  button.text.setCharacterSize(char_size);
  button.text.setFillColor(font_color);
  button.text.setFont(font);

  button.text.setString(text);
  button.id = last_button_id;

  auto sizes = button.text.getGlobalBounds();
  auto center_x = (button_width - sizes.width) / 2;
  auto center_y = (button_height - sizes.height) / 2;
  
  button.text.setPosition(margin_left + text_margin_left + center_x, 
                          margin + last_button_id * button_height +
                        ((last_button_id + 1) * margin_top) + center_y);

  ++last_button_id;
  buttons.push_back(button);
}

void Buttons::DrawButtons(sf::RenderWindow& window) {
  for (size_t i = 0; i < last_button_id; ++i) {
    window.draw(buttons[i].shape);
    window.draw(buttons[i].text);
  }
}

bool Button::Clicked(const sf::Vector2f& mouse_pos) {
    return shape.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y);
}

std::string Button::ClickHandle() {
  text.setFillColor(sf::Color(209, 202, 202));
  return text.getString();
}

size_t Button::GetId() {
  text.setFillColor(sf::Color(209, 202, 202));
  return id;
}

void Button::ReleaseHandle() {
  if (text.getFillColor() != sf::Color::White) {
    text.setFillColor(sf::Color::White);
  }
}
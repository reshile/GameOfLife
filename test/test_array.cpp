#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/WindowStyle.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "test", sf::Style::Default);
  window.setTitle("TEST");
  window.setFramerateLimit(60);

  std::vector<sf::Vertex> vertices;
  vertices.resize(6 * 1000 * 1000);

  size_t len = 40;

  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < 10; ++j) {
      sf::Vertex* triangles = &vertices[(i + j * 10) * 6];

      triangles[0].position = sf::Vector2f(i * len, j * len);
      triangles[1].position = sf::Vector2f((i + 1) * len, j * len);
      triangles[2].position = sf::Vector2f(i * len, (j + 1) * len);
      triangles[3].position = sf::Vector2f(i * len, (j + 1) * len);
      triangles[4].position = sf::Vector2f((i + 1) * len, j * len);
      triangles[5].position = sf::Vector2f((i + 1) * len, (j + 1) * len);

      triangles[0].color = sf::Color::White;
      triangles[1].color = sf::Color::White;
      triangles[2].color = sf::Color::White;
      triangles[3].color = sf::Color::White;
      triangles[4].color = sf::Color::White;
      triangles[5].color = sf::Color::White;
    }
  }



  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(&vertices[0], vertices.size(), sf::Triangles);
    window.display();

  }
  
}
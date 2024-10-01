#include "main_menu.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <coroutine>
#include <sfeMovie/Movie.hpp>

struct Awaitable {
  bool await_ready() {
    return false;
  }  // NOLINT
  void await_suspend(std::coroutine_handle<>) {
  }  // NOLINT
  void await_resume() {
  }  // NOLINT
};

Menu MainMenu(Grid& grid, UserInterface& ui) {
  sfe::Movie movie;
  movie.openFromFile("../resources/GamePreviewDemo.mp4");

  movie.fit(0, 0, Grid::desktop_.width / 2.f, Grid::desktop_.height / 2.f);
  movie.setPosition(500, 400);
  movie.pause();

  sf::Clock timer;

  movie.play();
  while (grid.GetWindow().isOpen()) {
    sf::Event event;

    while (grid.GetWindow().pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        grid.GetWindow().close();
        co_return;
      } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.scancode == sf::Keyboard::Scan::Enter) {
          movie.pause();
          co_await Awaitable{};
          grid.GetWindow().clear();
          movie.play();
        }
      }
    }

    if (timer.getElapsedTime().asSeconds() >=
        int(movie.getDuration().asSeconds())) {
      movie.stop();
      movie.play();
      timer.restart();
    }

    movie.update();

    grid.GetWindow().clear(sf::Color::Black);
    grid.GetWindow().draw(movie);
    grid.GetWindow().draw(ui.menu_text);
    grid.GetWindow().display();
  }
}

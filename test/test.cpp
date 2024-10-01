#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowHandle.hpp>

#include <cstdlib>
// #include "src/logic/game_of_life/for_user.hpp"

// Параметры игры
constexpr int CELL_SIZE = 10;  // Размер клетки
constexpr int ZOOM_FACTOR =
    2;  // Коэффициент масштабирования для приближения и отдаления

void renderGame(sf::RenderWindow& window, const automata::SetCells&
liveCells,
                int offsetX, int offsetY, float zoom) {
  // window.clear(sf::Color::Black);  // Очистка экрана

  // Создание буфера вершин
  sf::VertexArray vertices(sf::Quads);

  // Заполнение буфера вершин данными о живых клетках
  for (const auto& cell : liveCells) {
    int x = cell.x_coordinate * CELL_SIZE * zoom + offsetX;
    int y = cell.y_coordinate * CELL_SIZE * zoom + offsetY;

    vertices.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::White));
    vertices.append(
        sf::Vertex(sf::Vector2f(x + CELL_SIZE * zoom, y), sf::Color::White));
    vertices.append(
        sf::Vertex(sf::Vector2f(x + CELL_SIZE * zoom, y + CELL_SIZE * zoom),
                   sf::Color::White));
    vertices.append(
        sf::Vertex(sf::Vector2f(x, y + CELL_SIZE * zoom), sf::Color::White));
  }

  // Отрисовка буфера вершин
  window.draw(vertices);
}

int main() {
  automata::Game game;

  game.SetExecutor(automata::Executors::SingleThreadedExecutor);
  game.SetCalculator(automata::Calculators::ParallelCalculator);

  game.SetBoard(0, 0, true);
  game.SetRule(automata::GameTemplate::Snowflake);

  // game.SetTemplate("Cordership rake",
  //  {0, 0});  // Устанавливаем начальные координаты шаблона

  game.OneLoopStart();

  game.SetTemplate("Point",
                   {0, 0});  // Устанавливаем начальные координаты шаблона

  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game of Life");

  // Начальное смещение и масштаб
  sf::Font font;
  font.loadFromFile("../resources/PressStart2P-Regular.ttf");
  int offsetX = 0;
  int offsetY = 0;
  float zoom = 1.0f;

  bool is_pause = false;
  std::size_t iter = 0;

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        } else if (event.type == sf::Event::KeyPressed) {
          // Обработка нажатий клавиш для смещения и масштабирования
          if (event.key.code == sf::Keyboard::Left ||
              event.key.code == sf::Keyboard::A) {
            offsetX += CELL_SIZE * zoom * 10;
          } else if (event.key.code == sf::Keyboard::Right ||
                     event.key.code == sf::Keyboard::D) {
            offsetX -= CELL_SIZE * zoom * 10;
          } else if (event.key.code == sf::Keyboard::Up ||
                     event.key.code == sf::Keyboard::W) {
            offsetY += CELL_SIZE * zoom * 10;
          } else if (event.key.code == sf::Keyboard::Down ||
                     event.key.code == sf::Keyboard::S) {
            offsetY -= CELL_SIZE * zoom * 10;
          } else if (event.key.code == sf::Keyboard::Add) {
            zoom *= ZOOM_FACTOR;
          } else if (event.key.code == sf::Keyboard::Subtract) {
            zoom /= ZOOM_FACTOR;
          } else if (event.key.code == sf::Keyboard::Space) {
            if (is_pause) {
              game.ReStart();

            } else {
              game.Pause();
            }

            is_pause ^= true;
          }
        }
      }

    // Получаем координаты живых клеток на следующей итерации
    auto liveCells = game.OneIter();

    if (liveCells.size() >= 100'000) {
      break;
    }

    renderGame(window, liveCells, offsetX, offsetY,
               zoom);  // Рендерим текущее состояние игры с учетом смещения

    sf::Text text;
    text.setFont(font);
    text.setString("Living cells: " + std::to_string(liveCells.size()));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Cyan);
    text.setPosition(10, 10);
    window.draw(text);

    window.display();
  }

  game.OneLoopStop();

  return 0;
}

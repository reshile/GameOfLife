#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "src/logic/game_of_life/for_user.hpp"

#include "src/graphics/grid.hpp"
#include "src/graphics/user_interface.hpp"
#include "src/graphics/main_menu.hpp"
#include "src/graphics/event_handler.hpp"

int main() {
  automata::Game rt;
  std::size_t size_field = 500;

  rt.SetExecutor(automata::Executors::SingleThreadedExecutor);
  rt.SetCalculator(automata::Calculators::ParallelCalculator);

  rt.SetBoard(size_field, size_field, false);
  rt.SetRule(automata::GameTemplate::Default);

  Grid grid(500);
  grid.SetUp();

  UserInterface ui(grid.GetView(), grid.GetWindow());
  ui.SetUp();
  rt.OneLoopStart();

  // Main menu
  auto menu = MainMenu(grid, ui);

  while (grid.GetWindow().isOpen()) {
    grid.CalculateMousePosition();

    EventHandler::Handle(grid, ui, rt, menu);

    // React to user input
    grid.UpdateInput();
    grid.UpdateUserClick(rt, ui);

    grid.GetWindow().clear();
    grid.GetWindow().setView(grid.GetView());

    grid.RenderGrid(rt);

    // draw here user interface
    grid.GetWindow().setView(grid.GetWindow().getDefaultView());
    ui.UpdateLivingCells(rt);
    ui.Draw();

    // Done drawing
    grid.GetWindow().display();
  }

  rt.OneLoopStop();

  // rt.SetRule(automata::GameTemplate::BlinkingDiamonds);

  // rt.OneLoopStart(); // hehembra
}

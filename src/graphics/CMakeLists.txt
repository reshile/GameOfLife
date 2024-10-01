cmake_minimum_required(VERSION 3.20)


add_library(graphicslib grid.cpp user_interface.cpp buttons.cpp main_menu.cpp event_handler.cpp)
target_link_libraries(graphicslib PUBLIC sfeMovie)

target_link_libraries(graphicslib PUBLIC sfml-graphics sfml-window sfml-system)

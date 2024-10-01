cmake_minimum_required(VERSION 3.2)

add_subdirectory(game_of_life/data_presentation)
add_subdirectory(game_of_life/templates)
add_subdirectory(game_of_life/executor)

add_library(game game_of_life/game_construct.cpp)

target_include_directories(game INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(game PUBLIC datalib
                              PUBLIC templib
                              PUBLIC gamelib)

target_link_libraries(game PUBLIC sfml-graphics sfml-window sfml-system)

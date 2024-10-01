cmake_minimum_required(VERSION 3.20)

project(testing LANGUAGES C CXX)

# SET(CMAKE_CXX_FLAGS "-std=c++20 -O3")
# SET(CMAKE_CXX_FLAGS "-fsanitize=address")
# SET(CMAKE_CXX_FLAGS "rpath-link ")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6.1 COMPONENTS system window graphics audio REQUIRED)

add_executable(run test_array.cpp)

include_directories(run PUBLIC lib/sfml/SFML-2.6.1/include
                        PUBLIC lib/sfml/SFML-2.6.1/lib)

target_link_libraries(run PUBLIC sfml-graphics sfml-window sfml-system sfml-audio)


# Install sfeMovies packages
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/sfeMovie/lib/ DESTINATION lib/sfeMovieLib/)
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/sfeMovie/include/ DESTINATION lib/sfeMovieInclude/) 



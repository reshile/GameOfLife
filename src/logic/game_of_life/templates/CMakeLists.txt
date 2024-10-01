cmake_minimum_required(VERSION 3.2)

add_library(templib parser.cpp)

target_link_libraries(templib PUBLIC datalib)

target_include_directories(templib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.20)

add_library(datalib rule.cpp cell.cpp)
# include_directories(TBB_DIR)

# link_directories(${CMAKE_SOURCE_DIR}/lib/oneapi-tbb-2021.11.0/include)

# find_package(TBB 2021.11.0 COMPONENTS REQUIRED)
# target_link_libraries(datalib INTERFACE ${TBB_LIBRARIES})

target_include_directories(datalib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

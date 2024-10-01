add_library(gamelib calculator/np_calculator.cpp calculator/p_calculator.cpp single_threaded_executor.cpp cache.cpp)

# Equivalent to command-line option of `-DCMAKE_PREFIX_PATH=...`
list(APPEND CMAKE_MODULE_PATH "deps/tbb/cmake/")

find_package(TBB REQUIRED)

target_include_directories(gamelib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(gamelib PUBLIC datalib
                              PUBLIC templib)

target_link_libraries(gamelib PUBLIC TBB::tbb)


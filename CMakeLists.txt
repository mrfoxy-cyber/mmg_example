cmake_minimum_required(VERSION 3.16)
project(mmg_graph_example LANGUAGES CXX)

add_library(mmg_graph src/graph.cpp)
target_include_directories(mmg_graph PUBLIC include)
target_compile_features(mmg_graph PUBLIC cxx_std_20)

add_executable(mmg_graph_example src/main.cpp)
target_link_libraries(mmg_graph_example PRIVATE mmg_graph)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT mmg_graph_example)

enable_testing()
add_executable(mmg_graph_tests tests/graph_tests.cpp)
target_link_libraries(mmg_graph_tests PRIVATE mmg_graph)
add_test(NAME graph_tests COMMAND mmg_graph_tests)

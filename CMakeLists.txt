cmake_minimum_required(VERSION 3.16)

project(mmg_graph_example LANGUAGES CXX)

add_library(mmg_graph
    src/graph.cpp
)

target_include_directories(mmg_graph
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(mmg_graph PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(mmg_graph PRIVATE /W4 /permissive-)
else()
    target_compile_options(mmg_graph PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(mmg_graph_example
    src/main.cpp
)

target_link_libraries(mmg_graph_example PRIVATE mmg_graph)

include(CTest)

if(BUILD_TESTING)
    add_executable(mmg_graph_tests
        tests/graph_tests.cpp
    )

    target_link_libraries(mmg_graph_tests PRIVATE mmg_graph)

    if(MSVC)
        target_compile_options(mmg_graph_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(mmg_graph_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME mmg_graph_tests COMMAND mmg_graph_tests)
endif()

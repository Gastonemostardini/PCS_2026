cmake_minimum_required(VERSION 3.20)
project(esercitazione5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

enable_testing()

add_executable(test_graph test_graph.cpp)
add_test(NAME test_graph COMMAND test_graph)

add_executable(main main.cpp "undirected_graph.h" "undirected_tree.h" "search_graph.h" "dispenser.h" "stack.h" "queue.h")

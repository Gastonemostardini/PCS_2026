cmake_minimum_required(VERSION 3.20)
project(depina)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

enable_testing()

add_executable(main main.cpp "undirected_edge.h" "undirected_graph.h" "undirected_tree.h" "search_graph.h" "cycles.h" "functions.h")

cmake_minimum_required(VERSION 3.20)
project(esercitazione10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

enable_testing()

find_package(Eigen3 CONFIG REQUIRED)
list(APPEND libraries_list PUBLIC Eigen3::Eigen)

#add_executable(test_gradiente test_gradiente.cpp)
#add_test(NAME test_gradiente COMMAND test_gradiente)

add_executable(main main.cpp "edge.h" "graph.h" "tree.h" "weighed_edge.h" "weighed_graph.h" "search_graph.h"
	"dispenser.h" "fifo.h" "lifo.h" "gradiente.h")
target_link_libraries(main ${libraries_list})
#target_link_libraries(test_gradiente ${libraries_list})

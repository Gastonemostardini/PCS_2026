cmake_minimum_required(VERSION 3.20)
project(esercitazione10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

enable_testing()

find_package(Eigen3 REQUIRED)
list(APPEND libraries_list PUBLIC Eigen3::Eigen)

#add_executable(test_gradiente test_gradiente.cpp)
#add_test(NAME test_gradiente COMMAND test_gradiente)

add_executable(main main.cpp "edge.h" "graph.h" "tree.h" "weighed_edge.h" "weighed_graph.h" "search_graph.h"
	"dispenser.h" "fifo.h" "lifo.h" "functions.h" "cycles.h" "input.hpp" "matrices.h")
target_link_libraries(main ${libraries_list})
#target_link_libraries(test_gradiente ${libraries_list})

# Test dei componenti specifici del progetto (cicli / metodo delle maglie)
add_executable(test_search_graph tests/test_search_graph.cpp)
target_link_libraries(test_search_graph ${libraries_list})
add_test(NAME test_search_graph COMMAND test_search_graph)

add_executable(test_cycles tests/test_cycles.cpp)
target_link_libraries(test_cycles ${libraries_list})
add_test(NAME test_cycles COMMAND test_cycles)

add_executable(test_de_pina tests/test_de_pina.cpp)
target_link_libraries(test_de_pina ${libraries_list})
add_test(NAME test_de_pina COMMAND test_de_pina)

add_executable(test_functions tests/test_functions.cpp)
target_link_libraries(test_functions ${libraries_list})
add_test(NAME test_functions COMMAND test_functions)

add_executable(test_matrices tests/test_matrices.cpp)
target_link_libraries(test_matrices ${libraries_list})
add_test(NAME test_matrices COMMAND test_matrices)

add_executable(test_input tests/test_input.cpp)
target_link_libraries(test_input ${libraries_list})
add_test(NAME test_input COMMAND test_input)

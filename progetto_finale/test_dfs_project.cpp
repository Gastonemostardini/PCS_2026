//test per dfs-project
#include <iostream>
#include <set>
#include <vector>
#include <list>
#include <cstdlib>
#include <stdexcept>
#include "edge.h"
#include "graph.h"
#include "cycles.h"
#include "search_graph.h"
#include "pseudocodice_dfs_project.h"

//g++ -I /usr/include/eigen3 -std=c++20 test_dfs_project.cpp per compilare
int main() {
    
    std::set<int> nodes = { 1, 2, 3, 4 };
    
    //grafo completo
    std::set<Edge<int>> g_edges = { {1, 2}, {2, 4}, {1, 4}, {2, 3}, {3, 4} };
    Graph<int> G(nodes, g_edges);
    
    //prendo albero T
    //prendiamo 3 rami in modo che tocchino tutti i nodi senza formare cicli chiusi
    std::set<Edge<int>> t_edges = { {1, 2}, {2, 4}, {3, 4} };
    Graph<int> T_graph(nodes, t_edges);

    std::cout << "--- TEST CICLI MINIMI (METODO DFS) ---\n\n";
    std::cout << "Archi del Grafo G:\n" << G.all_edges() << "\n\n";
    std::cout << "Archi dell'Albero T:\n" << T_graph.all_edges() << "\n\n";

    auto cycles = find_minimal_cycles(G, T_graph);

    std::cout << "Numero di cicli trovati: " << cycles.size() << " (Attesi: 2)\n";
    std::cout << "Elenco dei cicli fondamentali:\n";
    
    int i = 1;
    for (const auto& cycle : cycles) {
        std::cout << "Ciclo " << i++ << ": " << cycle << "\n";
    }

    return 0;
}



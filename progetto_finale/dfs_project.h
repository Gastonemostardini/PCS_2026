// metodo dfs per progetto: cicli non necessariamente minimi
#pragma once
#include <ostream>
#include <queue>
#include <stack>
#include <list>
#include <limits>
#include <vector>
#include "graph.h"
#include "tree.h"
#include "dispenser.h"
#include "lifo.h"
#include "fifo.h"
#include "functions.h"
#include "cycles.h"

// ispirarto allo pseudocodice del file pdf datoci da caridi
// funzione per trovare le route nell'albero dfs
template <typename T, typename EdgeT>
bool find_path_dfs(const Graph<T, EdgeT> &T_graph, const T &start, const T &end, std::set<T> &visited_nodes, Lifo<T> &path)
{
    visited_nodes.insert(start);
    path.put(start);

    if (start == end)
    {
        return true;
    }

    for (const auto &neighbor : T_graph.neighbours(u))
    {
        if (!visited_nodes.contains(neighbor))
        {
            if (find_path_dfs(T_graph, neighbor, end, visited_nodes, path))
            {
                return true;
            }
        }
    }

    path.get();
    return false;
}

// funzione che cerca i cicli minimi
template <typename T, typename EdgeT = Edge<T>>
    requires std::totally_ordered<T>
std::list<Cycles<T, EdgeT>> find_minimal_cycles(Graph<T, EdgeT> G, Graph<T, EdgeT> T_graph)
{
    std::list<Cycles<T, EdgeT>> cycles_list;

    // co-albero C = G - T
    Graph<T, EdgeT> C = G - T_graph;

    auto all_graph_edges = G.all_edges();

    //"Ogni arco (u, v) del co-albero genera esattamente un ciclo fondamentale se unito a T"
    for (const auto &edge : C.all_edges())
    {
        std::set<T> visited;
        Lifo<T> path;

        // cerco il cammino tra i due estremi dell'arco sfruttando solo i rami dell'albero
        if (find_path_dfs(T_graph, edge.from(), edge.to(), visited, path))
        {
            std::set<EdgeT> cycle_edges;

            cycle_edges.insert(edge);

            // ridefinisco la nuova route in modo che sia vuota e riempita
            std::vector<T> route;
            while (!path.empty())
            {
                route.push_back(path.get());
            }

            // genera gli archi dell'albero a partire dai nodi estratti dal cammino
            for (size_t i = 0; i < route.size() - 1; ++i)
            {
                cycle_edges.insert(EdgeT(route[i], route[i + 1]));
            }

            // lo aggiungo all'insieme dei cicli trovati
            Cycles<T, EdgeT> cycle(all_graph_edges, cycle_edges);
            cycles_list.push_back(cycle);
        }
    }

    return cycles_list;
}

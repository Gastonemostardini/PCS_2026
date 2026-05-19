#pragma once
#include "undirected_graph.h"
#include "weighed_edge.h"

template<typename T> requires std::totally_ordered<T>
class WeighedGraph : public UndirectedGraph<T, WeighedEdge<T>> {
public:
    using UndirectedGraph<T, WeighedEdge<T>>::UndirectedGraph;

    void add_edge(T from, T to, int weight) {
        this->UndirectedGraph<T, WeighedEdge<T>>::add_edge(WeighedEdge<T>(from, to, weight));
    }

    using UndirectedGraph<T, WeighedEdge<T>>::add_edge;
};

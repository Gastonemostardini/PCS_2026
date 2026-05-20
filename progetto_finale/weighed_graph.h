#pragma once
#include "graph.h"
#include "weighed_edge.h"

template<typename T> requires std::totally_ordered<T>
class WeighedGraph : public Graph<T, WeighedEdge<T>> {
public:
    using Graph<T, WeighedEdge<T>>::Graph;

    void add_edge(T from, T to, int weight) {
        this->Graph<T, WeighedEdge<T>>::add_edge(WeighedEdge<T>(from, to, weight));
    }

    using Graph<T, WeighedEdge<T>>::add_edge;
};

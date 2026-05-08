#pragma once
#include <ostream>
#include "undirected_graph.h"
#include <set>
#include <map>
#include <queue>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::map<T, int > obj) {
    for (const auto& [key, value] : obj) {
        os << key << ":" << value << " ";
    }
    return os;
}


template<typename T> requires std::totally_ordered<T>
class TreeGraph : public UndirectedGraph<T> {
	T root_;
    std::map<T, int> layerOf_;

    friend std::ostream& operator<<(std::ostream& os, const TreeGraph<T>& obj) {
        os << "nodes: \n";
        os << obj.nodes_ << std::endl;
        os << "edges: \n";
        os << obj.edges_ << std::endl;
        os << "layer: \n";
        os << obj.layerOf_ << std::endl;
        return os;
    };

public:
	using UndirectedGraph<T>::UndirectedGraph;

    TreeGraph(std::set<T> nodes, std::set<UndirectedEdge<T>> edges)
        : UndirectedGraph<T>() {
        for (const auto& edge : edges) {
            add_edge(edge);
        }
    }

    TreeGraph(std::set<T> nodes, std::map<T, T> parent, T root)
        : UndirectedGraph<T>(), root_(std::move(root)) {
        if (!nodes.contains(root_)) {
            throw std::invalid_argument("root must be one of the nodes");
        }

        std::map<T, std::vector<T>> children;
        for (const auto& [child, par] : parent) {
            children[par].push_back(child);
        }

        this->nodes_.insert(root_); 

        std::queue<T> q;
        q.push(root_);
        while (!q.empty()) {
            T current = q.front();
            q.pop();
            for (const T& child : children[current]) {
                add_edge(current, child); 
                q.push(child);
            }
        }
    }

	using UndirectedGraph<T>::all_edges;
	using UndirectedGraph<T>::edges_;

    void add_edge(UndirectedEdge<T> edge) {
        add_edge(edge.from(), edge.to());
    }

    void add_edge(T from, T to) {
        if (this->nodes_.empty()) {
            layerOf_[from] = 0;
        }
        else if (this->nodes_.find(from) == this->nodes_.end()) {
            throw std::invalid_argument("from node must already exist in the tree");
        }

        if (this->nodes_.find(to) != this->nodes_.end()) {
            throw std::invalid_argument("to node must not already exist in the tree");
        }

        layerOf_[to] = layerOf_[from] + 1;
        this->UndirectedGraph<T>::add_edge(from, to);
    }

    void fixIsolated() {
        const auto& nodes = this->all_nodes();
        std::set<T> neighours;

        for (auto node : nodes) {
            if (this->UndirectedGraph<T>::neighours(node).size() == 0 && nodes.size() != 1)
                throw std::invalid_argument("Only the root alone can have 0 neighours");
            neighours = this->UndirectedGraph<T>::neighours(node);
            for (auto neighour : neighours) {
                if (layerOf_[neighour] >= layerOf_[node])
                    throw std::invalid_argument("neighours can only be child");
            }
        }

    }

};

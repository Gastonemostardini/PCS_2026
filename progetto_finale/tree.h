#pragma once
#include <ostream>
#include "graph.h"
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::map<T, int > obj) {
    for (const auto& [key, value] : obj) {
        os << key << ":" << value << " ";
    }
    return os;
}




template<typename T, typename EdgeT = Edge<T>>
    requires std::totally_ordered<T>
class TreeGraph : public Graph<T, EdgeT> {
	T root_;
    std::map<T, int> layerOf_;

    static std::map<T, T> with_parent(std::map<T, T> parent, T child, T new_parent) {
        parent[child] = new_parent;
        return parent;
    }

    friend std::ostream& operator<<(std::ostream& os, const TreeGraph<T, EdgeT>& obj) {
        auto parent = const_cast<TreeGraph<T, EdgeT>&>(obj).get_parent();

        std::map<T, std::vector<T>> children;
        for (const auto& [child, par] : parent) {
            children[par].push_back(child);
        }
        for (auto& [_, vec] : children) {
            std::sort(vec.begin(), vec.end());
        }

        os << "TreeGraph (root: " << obj.root_ << ", "
            << obj.all_nodes().size() << " nodes)\n";

        std::function<void(const T&, std::vector<bool>&, bool)> print_node =
            [&](const T& node, std::vector<bool>& is_last, bool last) {
            for (size_t i = 0; i + 1 < is_last.size(); ++i) {
                os << (is_last[i] ? "    " : "│   ");
            }
            if (!is_last.empty()) {
                os << (last ? "└── " : "├── ");
            }
            os << node;

            auto it = obj.layerOf_.find(node);
            if (it != obj.layerOf_.end()) {
                os << "  [layer " << it->second << "]";
            }
            os << '\n';

            auto child_it = children.find(node);
            if (child_it == children.end()) return;

            const auto& kids = child_it->second;
            is_last.push_back(false);
            for (size_t i = 0; i < kids.size(); ++i) {
                is_last.back() = (i == kids.size() - 1);
                print_node(kids[i], is_last, i == kids.size() - 1);
            }
            is_last.pop_back();
            };

        std::vector<bool> is_last;
        print_node(obj.root_, is_last, true);
        return os;
    }


protected:
    TreeGraph() = default;

public:
	using Graph<T, EdgeT>::Graph;

    TreeGraph(T root)
        : TreeGraph({ root }, {}, root) {
    }

    TreeGraph(std::set<EdgeT> edges)
        : TreeGraph(std::set<T>{}, edges) {
    }

    TreeGraph(std::set<T> nodes, std::set<EdgeT> edges)
        : Graph<T, EdgeT>() {
        for (const auto& edge : edges) {
            add_edge(edge);
        }
    }

    TreeGraph(std::set<T> nodes, std::map<T, T> parent, T root)
        : Graph<T, EdgeT>(), root_(std::move(root)) {
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

    // aggiungi una testa prima del tree
    TreeGraph(T root, TreeGraph tree):
        TreeGraph(tree.all_nodes(), with_parent(tree.get_parent(), tree.get_root()), root)
    {

    }

	using Graph<T, EdgeT>::all_edges;
	using Graph<T, EdgeT>::edges_;

    void add_edge(EdgeT edge) {
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
        this->Graph<T, EdgeT>::add_edge(from, to);
    }

    void fixIsolated() {
        const auto& nodes = this->all_nodes();
        std::set<T> neighbours;

        for (auto node : nodes) {
            if (this->Graph<T, EdgeT>::neighbours(node).size() == 0 && nodes.size() != 1)
                throw std::invalid_argument("Only the root alone can have 0 neighbours");
            neighbours = this->Graph<T, EdgeT>::neighbours(node);
            for (auto neighour : neighbours) {
                if (layerOf_[neighour] >= layerOf_[node])
                    throw std::invalid_argument("neighbours can only be child");
            }
        }

    }

    std::map<T, T> get_parent() const {
        std::map<T, T> parent;
        std::set<T> visited;
        std::queue<T> q;

        q.push(root_);
        visited.insert(root_);

        while (!q.empty()) {
            T current = q.front();
            q.pop();
            for (const T& neighbor : this->neighbours(current)) {
                if (!visited.contains(neighbor)) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        return parent;
    }

    T get_root() {
        return root_;
    }

    void add_tree(T node, TreeGraph other) {
        T other_root = other.get_root();
        add_edge(node, other_root);

        auto parent = other.get_parent();

        std::map<T, std::vector<T>> children;
        for (const auto& [child, par] : parent) {
            children[par].push_back(child);
        }

        std::queue<T> q;
        q.push(other_root);
        while (!q.empty()) {
            T current = q.front();
            q.pop();
            for (const T& child : children[current]) {
                add_edge(current, child);
                q.push(child);
            }
        }
    }

    std::vector<T> children(T value) const {
        std::vector<T> neighbours;
        int base_layer = layerOf_.at(value);
        for (auto edge : edges_){
            if (edge.has(value)) {
                if (edge.from() == value && layerOf_.at(edge.to()) > base_layer)
                    neighbours.push_back(edge.to());
                else if (edge.to() == value && layerOf_.at(edge.from()) > base_layer)
                    neighbours.push_back(edge.from());
            }
        }

        return neighbours;
    };

};

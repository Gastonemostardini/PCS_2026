#pragma once
#include <ostream>
#include "undirected_edge.h"
#include <set>
#include <utility>

template<typename T, typename EdgeT = UndirectedEdge<T>>
	requires std::totally_ordered<T>
class UndirectedGraph
{
	template<typename U, typename EdgeU> requires std::totally_ordered<U>
	friend class TreeGraph;

protected:
	std::set<T> nodes_;
	std::set<EdgeT> edges_;
	UndirectedGraph() = default;

	void normalize();

	void fixIsolated();

	friend std::ostream& operator<< <>(std::ostream& os, const UndirectedGraph<T, EdgeT>& obj);

public:
	/* COSTRUTTORI */
	UndirectedGraph(std::set<T> nodes);
	UndirectedGraph(std::set<EdgeT> edges);
	UndirectedGraph(std::set<T> nodes, std::set<EdgeT> edges);
	UndirectedGraph(const UndirectedGraph& other);

	/* FUNZIONI */
	void add_edge(T from, T to);
	void add_edge(EdgeT edge);
	void remove_node(T node);
	std::set<T> neighours(T value) const;
	std::set<EdgeT> all_edges() const;
	std::set<T> all_nodes() const;
	unsigned int edge_number(const EdgeT edge) const;
	const EdgeT& edge_at(unsigned int i) const;
	bool has_node(T& target) const;

	/* OPERATORI */
	UndirectedGraph<T, EdgeT> operator-(const UndirectedGraph<T, EdgeT>& other);
	bool operator==(const UndirectedGraph<T, EdgeT>& other);
	bool operator!=(const UndirectedGraph<T, EdgeT>& other);
};

template<typename EdgeT>
UndirectedGraph(std::set<EdgeT>) -> UndirectedGraph<decltype(std::declval<EdgeT>().from()), EdgeT>;

template<typename T, typename EdgeT>
UndirectedGraph(std::set<T>, std::set<EdgeT>) -> UndirectedGraph<T, EdgeT>;

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T> obj) {
	for (auto elm : obj) {
		os << elm << " ";
	}
	return os;
}

template<typename T, typename EdgeT>
std::ostream& operator<<(std::ostream& os, const UndirectedGraph<T, EdgeT>& obj) {
	os << "nodes: \n";
	os << obj.nodes_ << std::endl;
	os << "edges: \n";
	os << obj.edges_ << std::endl;
	return os;
}

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
UndirectedGraph<T, EdgeT>::UndirectedGraph(std::set<T> nodes) {
	nodes_ = nodes;
	normalize();
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
UndirectedGraph<T, EdgeT>::UndirectedGraph(std::set<EdgeT> edges) {
	for (auto edge : edges) {
		add_edge(edge);
	}
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
UndirectedGraph<T, EdgeT>::UndirectedGraph(std::set<T> nodes, std::set<EdgeT> edges) {
	nodes_ = nodes;

	for (auto edge : edges) {
		add_edge(edge);
	}

	normalize();
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
UndirectedGraph<T, EdgeT>::UndirectedGraph(const UndirectedGraph& other) {
	nodes_ = other.nodes_;
	for (auto edge : other.edges_) {
		add_edge(edge);
	}
	normalize();
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
std::set<T> UndirectedGraph<T, EdgeT>::neighours(T value) const {
	std::set<T> neighours;

	for (auto edge : edges_)
		if (edge.has(value))
			neighours.insert(edge.from() == value ? edge.to() : edge.from());

	return neighours;
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
void UndirectedGraph<T, EdgeT>::add_edge(T from, T to) {
	EdgeT edge(from, to);
	add_edge(edge);
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
void UndirectedGraph<T, EdgeT>::add_edge(EdgeT edge) {
	nodes_.insert(edge.from());
	nodes_.insert(edge.to());
	edges_.insert(edge);
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
void UndirectedGraph<T, EdgeT>::remove_node(T node) {
	nodes_.erase(node);
	for (auto edge: edges_){
		if (edge.from() == node || edge.to() == node)
			edges_.erase(edge);
	}
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
std::set<EdgeT> UndirectedGraph<T, EdgeT>::all_edges() const {
	return edges_;
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
std::set<T> UndirectedGraph<T, EdgeT>::all_nodes() const {
	return nodes_;
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
bool UndirectedGraph<T, EdgeT>::has_node(T& target) const {
	return nodes_.contains(target);
}

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
unsigned int UndirectedGraph<T, EdgeT>::edge_number(const EdgeT edge) const {
	auto it = edges_.find(edge);
	if (it == edges_.end()) {
		throw std::out_of_range("edge not found");
	}
	return static_cast<unsigned int>(std::distance(edges_.begin(), it));
}

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
const EdgeT& UndirectedGraph<T, EdgeT>::edge_at(unsigned int i) const {
	if (i >= edges_.size()) {
		throw std::out_of_range("edge index out of range");
	}
	auto it = edges_.begin();
	std::advance(it, i);
	return *it;
};


template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
void UndirectedGraph<T, EdgeT>::normalize() {
	fixIsolated();
}

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
void UndirectedGraph<T, EdgeT>::fixIsolated() {
	for (auto node : nodes_) {
		if (neighours(node).empty()) {
			add_edge(node, node);
		}
	}
}

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
UndirectedGraph<T, EdgeT> UndirectedGraph<T, EdgeT>::operator-(const UndirectedGraph<T, EdgeT>& other) {
	std::set<EdgeT> edges = edges_;
	std::set<T> nodes = nodes_;
	for (auto edge : other.edges_){
		edges.erase(edge);
	}

	nodes.clear();
	for (auto edge : edges) {
		nodes.insert(edge.from());
		nodes.insert(edge.to());
	}
	UndirectedGraph newObj(nodes, edges);
	return newObj;
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
bool UndirectedGraph<T, EdgeT>::operator==(const UndirectedGraph<T, EdgeT>& other) {
	if (all_nodes() != other.all_nodes()) return false;
	if (all_edges() != other.all_edges()) return false;
	return true;
}

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
bool UndirectedGraph<T, EdgeT>::operator!=(const UndirectedGraph<T, EdgeT>& other) {
	if (all_nodes() != other.all_nodes()) return true;
	if (all_edges() != other.all_edges()) return true;
	return false;
}


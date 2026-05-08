#pragma once
#include <ostream>
#include "undirected_edge.h"
#include <set>

template<typename T> requires std::totally_ordered<T>
class UndirectedGraph
{
	template<typename U> requires std::totally_ordered<U>
	friend class TreeGraph;

protected:
	std::set<T> nodes_;
	std::set<UndirectedEdge<T>> edges_; 
	UndirectedGraph() = default;

	void normalize();

	void fixIsolated();

	friend std::ostream& operator<< <>(std::ostream& os, const UndirectedGraph<T>& obj);

public:
	/* COSTRUTTORI */
	UndirectedGraph(std::set<T> nodes);
	UndirectedGraph(std::set<UndirectedEdge<T>> edges);
	UndirectedGraph(std::set<T> nodes, std::set<UndirectedEdge<T>> edges);
	UndirectedGraph(const UndirectedGraph& other);

	/* FUNZIONI */ 
	void add_edge(T from, T to);
	void add_edge(UndirectedEdge<T> edge);
	std::set<T> neighours(T value) const;
	std::set<UndirectedEdge<T>> all_edges() const;
	std::set<T> all_nodes() const;
	unsigned int edge_number(const UndirectedEdge<T> edge) const;
	const UndirectedEdge<T>& edge_at(unsigned int i) const;

	/* OPERATORI */
	UndirectedGraph<T> operator-(const UndirectedGraph<T>& other);
	bool operator==(const UndirectedGraph<T>& other);
	bool operator!=(const UndirectedGraph<T>& other);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T> obj) {
	for (auto elm : obj) {
		os << elm << " ";
	}
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const UndirectedGraph<T>& obj) {
	os << "nodes: \n";
	os << obj.nodes_ << std::endl;
	os << "edges: \n";
	os << obj.edges_ << std::endl;
	return os;
}

template<typename T> requires std::totally_ordered<T>
UndirectedGraph<T>::UndirectedGraph(std::set<T> nodes) {
	nodes_ = nodes;
	normalize();
};

template<typename T> requires std::totally_ordered<T>
UndirectedGraph<T>::UndirectedGraph(std::set<UndirectedEdge<T>> edges) {
	for (auto edge : edges) {
		add_edge(edge);
	}
};

template<typename T> requires std::totally_ordered<T>
UndirectedGraph<T>::UndirectedGraph(std::set<T> nodes, std::set<UndirectedEdge<T>> edges) {
	nodes_ = nodes;

	for (auto edge : edges) {
		add_edge(edge);
	}

	normalize();
};

template<typename T> requires std::totally_ordered<T>
UndirectedGraph<T>::UndirectedGraph(const UndirectedGraph& other) {
	nodes_ = other.nodes_;
	for (auto edge : other.edges_) {
		add_edge(edge);
	}
	normalize();
};

template<typename T> requires std::totally_ordered<T>
std::set<T> UndirectedGraph<T>::neighours(T value) const {
	std::set<T> neighours;

	for (auto edge : edges_)
		if (edge.has(value))
			neighours.insert(edge.from() == value ? edge.to() : edge.from());

	return neighours;
};

template<typename T> requires std::totally_ordered<T>
void UndirectedGraph<T>::add_edge(T from, T to) {
	UndirectedEdge edge(from, to);
	add_edge(edge);
};

template<typename T> requires std::totally_ordered<T>
void UndirectedGraph<T>::add_edge(UndirectedEdge<T> edge) {
	nodes_.insert(edge.from());
	nodes_.insert(edge.to());
	edges_.insert(edge);
};

template<typename T> requires std::totally_ordered<T>
std::set<UndirectedEdge<T>> UndirectedGraph<T>::all_edges() const {
	return edges_;
};

template<typename T> requires std::totally_ordered<T>
std::set<T> UndirectedGraph<T>::all_nodes() const {
	return nodes_;
};

template <typename T> requires std::totally_ordered<T>
unsigned int UndirectedGraph<T>::edge_number(const UndirectedEdge<T> edge) const {
	auto it = edges_.find(edge);
	if (it == edges_.end()) {
		throw std::out_of_range("edge not found");
	}
	return static_cast<unsigned int>(std::distance(edges_.begin(), it));
}

template <typename T> requires std::totally_ordered<T>
const UndirectedEdge<T>& UndirectedGraph<T>::edge_at(unsigned int i) const {
	if (i >= edges_.size()) {
		throw std::out_of_range("edge index out of range");
	}
	auto it = edges_.begin();
	std::advance(it, i);
	return *it;
};


template <typename T> requires std::totally_ordered<T>
void UndirectedGraph<T>::normalize() {
	fixIsolated();
}

template <typename T> requires std::totally_ordered<T>
void UndirectedGraph<T>::fixIsolated() {
	for (auto node : nodes_) {
		if (neighours(node).empty()) {
			add_edge(node, node);
		}
	}
}

template <typename T> requires std::totally_ordered<T>
UndirectedGraph<T> UndirectedGraph<T>::operator-(const UndirectedGraph<T>& other) {
	std::set<UndirectedEdge<T>> edges = edges_;
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

template <typename T> requires std::totally_ordered<T>
bool UndirectedGraph<T>::operator==(const UndirectedGraph<T>& other) {
	if (all_nodes() == other.all_nodes()) return false;
	if (all_edges() == other.all_edges()) return false;
	return true;
}

template <typename T> requires std::totally_ordered<T>
bool UndirectedGraph<T>::operator!=(const UndirectedGraph<T>& other) {
	if (all_nodes() != other.all_nodes()) return true;
	if (all_edges() != other.all_edges()) return true;
	return false;
}


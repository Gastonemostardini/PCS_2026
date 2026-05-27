#pragma once
#include "functions.h"
#include "edge.h"
#include <set>
#include <algorithm>

template<typename T, typename EdgeT = Edge<T>> requires std::totally_ordered<T>
class Cycles {

	std::vector<bool> adjacency;
	std::set<Edge<int>> real_edges;
	friend std::ostream& operator<<(std::ostream& os, const Cycles& obj) {
		//os << obj.adjacency;
		os << "{";
		bool first = true;
		std::size_t i = 0;
		for (const auto& edge : obj.real_edges) {
			if (obj.adjacency[i]) {
				if (!first) os << ", ";
				os << edge;
				first = false;
			}
			++i;
		}
		os << "}";
		return os;
	}

public:
	Cycles(std::set<Edge<int>> input_edges, std::set<Edge<int>> edges)
		: adjacency(input_edges.size(), false), real_edges(input_edges) {
		std::size_t i = 0;
		for (const auto& edge : real_edges) {
			if (edges.contains(edge)) {
				adjacency[i] = true;
			}
			i++;
		}
	}

	Cycles(std::set<Edge<int>> input_edges)
		: adjacency(input_edges.size(), false), real_edges(input_edges) {
	}


	std::list<EdgeT> all_edges() {
		std::list<EdgeT> edges;
		std::size_t i = 0;
		for (const auto& edge : real_edges) {
			if (adjacency[i])
				edges.insert(edge);
			i++;
		}
		return edges;
	};

	bool operator*(Cycles <T, EdgeT> other) {
		return adjacency * other.adjacency;
	};

	Cycles<T, EdgeT> operator^(Cycles<T, EdgeT> other) {
		return adjacency ^ other.adjacency;
	};

	//Cycles& operator=(Cycles<T, EdgeT> other) {
	//	adjacency = other.adjacency;
	//	real_edges = other.real_edges;
	//};

	Cycles<T, EdgeT>& operator^=(Cycles<T, EdgeT> other) {
		adjacency = adjacency ^ other.adjacency;
		return *this;
	};

	Cycles<T, EdgeT>& operator+=(Cycles<T, EdgeT> other) {
		adjacency += other.adjacency;
		return *this;
	};

	void clear() {
		std::fill(adjacency.begin(), adjacency.end(), false);
	}

	std::size_t weight() const {
		return std::count(adjacency.begin(), adjacency.end(), true);
	}

	bool is_active(EdgeT edge) {
		std::size_t i = 0;
		for (const auto& e : real_edges) {
			if (e == edge) {
				return adjacency[i];
			}
			i++;
		}
		return false;
	}

	void add_edge(EdgeT edge) {
		std::size_t i = 0;
		for (const auto& e : real_edges) {
			if (e == edge) {
				adjacency[i] = true;
			}
			i++;
		}
	}

	void remuve_edge(EdgeT edge) {
		std::size_t i = 0;
		for (const auto& e : real_edges) {
			if (e == edge) {
				adjacency[i] = false;
			}
			i++;
		}
	}

};
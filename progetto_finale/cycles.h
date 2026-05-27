#pragma once
#include "functions.h"

template<typename T, typename EdgeT = Edge<T>> requires std::totally_ordered<T>
class Cycles {

	std::vector<bool> adjacency;
	std::vector<T> real_edges;
	friend std::ostream& operator<<(std::ostream& os, const Cycles& obj) {
		os << "{";
		bool first = true;
		for (std::size_t i = 0; i < obj.real_edges.size(); ++i) {
			if (obj.adjacency[i]) {
				if (!first) os << ", ";
				os << obj.real_edges[i];
				first = false;
			}
		}
		os << "}";
		return os;
	}

public:

	std::list<EdgeT> all_edges() {
		std::list<EdgeT> edges;
		for (auto i = 0; i < real_edges.size(); i++) {
			if (adjacency[i])
				edges.insert(real_edges[i]);
		}
		return edges;
	};

	bool operator*(Cycles <T, EdgeT> other) {
		return adjacency * other.adjacency;
	};

	std::vector<bool> operator^(Cycles <T, EdgeT> other) {
		return adjacency ^ other.adjacency;
	};

};
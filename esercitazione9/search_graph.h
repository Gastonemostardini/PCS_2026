#pragma once
#include <ostream>
#include "undirected_graph.h"
#include "dispenser.h"
#include <queue>
#include <stack>
#include "lifo.h"
#include "fifo.h"

template<typename T> requires std::totally_ordered<T>
class SearchGraph
{
	UndirectedGraph<T> graph_;
	T start_;


public:

	SearchGraph(UndirectedGraph<T> graph, T start);
	void setGraph(UndirectedGraph<T> graph, T start);
	void setStart(T start);

	/* SEARCH */
	TreeGraph<T> depth_first_search() const;
	TreeGraph<T> breadth_first_search() const;
	TreeGraph<T> search(Dispenser<std::pair<T, T>>& dispenser) const;
	TreeGraph<T> search(Dispenser<T>& dispenser) const;

};

template<typename T> requires std::totally_ordered<T>
SearchGraph<T>::SearchGraph(UndirectedGraph<T> graph, T start)
	: graph_(std::move(graph))   
{
	setStart(start);
}

template<typename T> requires std::totally_ordered<T>
void SearchGraph<T>::setGraph(UndirectedGraph<T> graph, T start) {
	graph_ = graph;
	setStart(start);
};

template<typename T> requires std::totally_ordered<T>
void SearchGraph<T>::setStart(T start) {
	if (!graph_.has_node(start))
		throw std::out_of_range("node not found");
	start_ = start;
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> SearchGraph<T>::depth_first_search() const {
	Lifo<std::pair<T, T>> lifo;
	return search(lifo);
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> SearchGraph<T>::breadth_first_search() const {
	Fifo<std::pair<T, T>> fifo;
	return search(fifo);
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> SearchGraph<T>::search(Dispenser<std::pair<T, T>>& dispenser) const {
	std::set<T> reached_nodes;
	std::map<T, T> parent;  // parent[child] = padre nell'albero di ricerca

	dispenser.put({ start_, start_ });

	while (!dispenser.empty()) {
		auto [actual, pusher] = dispenser.get();

		if (reached_nodes.contains(actual)) continue;
		reached_nodes.insert(actual);
		if (actual != start_) parent[actual] = pusher;

		auto neighbours = graph_.neighours(actual);
		for (auto it = neighbours.rbegin(); it != neighbours.rend(); ++it) {
			if (!reached_nodes.contains(*it)) {
				dispenser.put({ *it, actual });
			}
		}
	}

	return TreeGraph<T>(reached_nodes, parent, start_);
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> SearchGraph<T>::search(Dispenser<T>& dispenser) const {
	std::set<T> reached_nodes;
	std::map<T, T> parent;  // parent[child] = padre nell'albero di ricerca

	dispenser.put(start_);

	while (!dispenser.empty()) {
		T actual = dispenser.get();

		if (reached_nodes.contains(actual)) continue;
		reached_nodes.insert(actual);

		for (auto neighbour : graph_.neighours(actual)) {
			if (!reached_nodes.contains(neighbour)) {
				if (!dispenser.contains(neighbour)) {
					parent[neighbour] = actual;
				}
				dispenser.put(neighbour);
			}
		}
	}

	return TreeGraph<T>(reached_nodes, parent, start_);
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> graph_visit(UndirectedGraph<T> graph, T start, Dispenser<std::pair<T, T>>& dispenser) {
	return SearchGraph(graph, start).search(dispenser);
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> graph_visit(UndirectedGraph<T> graph, T start, Dispenser<T>& dispenser) {
	return SearchGraph(graph, start).search(dispenser);
};

template<typename T> requires std::totally_ordered<T>
void dfs(const UndirectedGraph<T>& graph, T current,
	std::set<T>& visited, TreeGraph<T>& tree) {
	visited.insert(current);
	for (auto neighbour : graph.neighours(current)) {
		if (!visited.contains(neighbour)) {
			TreeGraph<T> subtree(neighbour);
			dfs(graph, neighbour, visited, subtree);
			tree.add_tree(current, subtree);
		}
	}
}

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> recursive_dfs(const UndirectedGraph<T>& graph, T start) {
	TreeGraph<T> result(start);
	std::set<T> visited;
	dfs(graph, start, visited, result);
	return result;
}

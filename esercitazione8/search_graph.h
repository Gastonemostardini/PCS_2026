#pragma once
#include <ostream>
#include "undirected_graph.h"
#include "dispenser.h"
#include <queue>
#include <stack>
#include "stack.h"
#include "queue.h"

template<typename T> requires std::totally_ordered<T>
class SearchGraph
{
	UndirectedGraph<T> graph_;
	T start_;

	TreeGraph<T> search(Dispenser<T>& dispenser) const;

public:

	SearchGraph(UndirectedGraph<T> graph, T start);
	void setGraph(UndirectedGraph<T> graph, T start);
	void setStart(T start);

	/* SEARCH */
	TreeGraph<T> depth_first_search() const;
	TreeGraph<T> breadth_first_search() const;
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
	Stack<T> stack;
	return search(stack);
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> SearchGraph<T>::breadth_first_search() const {
	Queue<T> queue;
	return search(queue);
};

template<typename T> requires std::totally_ordered<T>
TreeGraph<T> SearchGraph<T>::search(Dispenser<T>& dispenser) const {
	std::set<T> reached_nodes;
	std::map<T, T> parent;  // parent[child] = padre nell'albero di ricerca

	reached_nodes.insert(start_);
	dispenser.insert(start_);

	while (!dispenser.empty()) {
		T actual = dispenser.remove();

		for (auto neighbour : graph_.neighours(actual)) {
			if (!reached_nodes.contains(neighbour)) {
				reached_nodes.insert(neighbour);
				dispenser.insert(neighbour);
				parent[neighbour] = actual;
			}
		}
	}

	return TreeGraph<T>(reached_nodes, parent, start_);
};

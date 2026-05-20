#pragma once
#include <ostream>
#include <queue>
#include <stack>
#include <list>
#include <limits>
#include "graph.h"
#include "tree.h"
#include "dispenser.h"
#include "lifo.h"
#include "fifo.h"

template<typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
class SearchGraph
{
	Graph<T, EdgeT> graph_;
	T start_;


public:

	SearchGraph(Graph<T, EdgeT> graph, T start);
	void setGraph(Graph<T, EdgeT> graph, T start);
	void setStart(T start);

	/* SEARCH */
	TreeGraph<T, EdgeT> depth_first_search() const;
	TreeGraph<T, EdgeT> breadth_first_search() const;
	TreeGraph<T, EdgeT> search(Dispenser<std::pair<T, T>>& dispenser) const;
	TreeGraph<T, EdgeT> search(Dispenser<T>& dispenser) const;

};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
SearchGraph<T, EdgeT>::SearchGraph(Graph<T, EdgeT> graph, T start)
	: graph_(std::move(graph))
{
	setStart(start);
}

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
void SearchGraph<T, EdgeT>::setGraph(Graph<T, EdgeT> graph, T start) {
	graph_ = graph;
	setStart(start);
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
void SearchGraph<T, EdgeT>::setStart(T start) {
	if (!graph_.has_node(start))
		throw std::out_of_range("node not found");
	start_ = start;
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::depth_first_search() const {
	Lifo<std::pair<T, T>> lifo;
	return search(lifo);
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::breadth_first_search() const {
	Fifo<std::pair<T, T>> fifo;
	return search(fifo);
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::search(Dispenser<std::pair<T, T>>& dispenser) const {
	std::set<T> reached_nodes;
	std::map<T, T> parent;  // parent[child] = padre nell'albero di ricerca

	dispenser.put({ start_, start_ });

	while (!dispenser.empty()) {
		auto [actual, pusher] = dispenser.get();

		if (reached_nodes.contains(actual)) continue;
		reached_nodes.insert(actual);
		if (actual != start_) parent[actual] = pusher;

		auto neighbours = graph_.neighbours(actual);
		for (auto it = neighbours.rbegin(); it != neighbours.rend(); ++it) {
			if (!reached_nodes.contains(*it)) {
				dispenser.put({ *it, actual });
			}
		}
	}

	return TreeGraph<T, EdgeT>(reached_nodes, parent, start_);
};

template<typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::search(Dispenser<T>& dispenser) const {
	std::set<T> reached_nodes;
	std::map<T, T> parent;  // parent[child] = padre nell'albero di ricerca

	dispenser.put(start_);

	while (!dispenser.empty()) {
		T actual = dispenser.get();

		if (reached_nodes.contains(actual)) continue;
		reached_nodes.insert(actual);

		for (auto neighbour : graph_.neighbours(actual)) {
			if (!reached_nodes.contains(neighbour)) {
				if (!dispenser.contains(neighbour)) {
					parent[neighbour] = actual;
				}
				dispenser.put(neighbour);
			}
		}
	}

	return TreeGraph<T, EdgeT>(reached_nodes, parent, start_);
};

template<typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> graph_visit(Graph<T, EdgeT> graph, T start, Dispenser<std::pair<T, T>>& dispenser) {
	return SearchGraph<T, EdgeT>(graph, start).search(dispenser);
};

template<typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> graph_visit(Graph<T, EdgeT> graph, T start, Dispenser<T>& dispenser) {
	return SearchGraph<T, EdgeT>(graph, start).search(dispenser);
};

template<typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
void dfs(const Graph<T, EdgeT>& graph, T current, std::set<T>& visited, TreeGraph<T, EdgeT>& tree) {
	visited.insert(current);
	for (auto neighbour : graph.neighbours(current)) {
		if (!visited.contains(neighbour)) {
			TreeGraph<T, EdgeT> subtree(neighbour);
			dfs(graph, neighbour, visited, subtree);
			tree.add_tree(current, subtree);
		}
	}
}

template<typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> recursive_dfs(const Graph<T, EdgeT>& graph, T start) {
	TreeGraph<T, EdgeT> result(start);
	std::set<T> visited;
	dfs(graph, start, visited, result);
	return result;
}


template<typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> dijkstra(const Graph<T, EdgeT>& graph, T start) {
	using Entry = std::pair<int, T>;
	constexpr int INF = std::numeric_limits<int>::max();

	std::map<T, int> distanze;
	std::map<T, T> precedente;

	for (const auto& v : graph.all_nodes()) distanze[v] = INF;
	distanze[start] = 0;

	std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> q;
	q.emplace(0, start);

	while (!q.empty()) {
		auto [d, u] = q.top();
		q.pop();
		if (d > distanze[u]) continue;
		for (const auto& v : graph.neighbours(u)) {
			int nd;
			if constexpr (requires(EdgeT e) { e.getWeight(); }) {
				int w = 1;
				for (const auto& e : graph.all_edges()) {
					if (e.has(u) && e.has(v)) { w = e.getWeight(); break; }
				}
				nd = d + w;
			} else {
				nd = d + 1;
			}
			if (nd < distanze[v]) {
				distanze[v] = nd;
				precedente[v] = u;
				q.emplace(nd, v);
			}
		}
	}

	TreeGraph<T, EdgeT> res(graph.all_nodes(), precedente, start);
	return res;
}

class Cycles {};

template<typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
std::list<Cycles> de_pina(const Graph<T, EdgeT>& graph){

}

#pragma once
#include <ostream>
#include "undirected_graph.h"
#include "undirected_tree.h"
#include "undirected_edge.h"
#include <queue>
#include <limits>

template<typename T, typename EdgeT = UndirectedEdge<T>>
	requires std::totally_ordered<T>
void dfs(const UndirectedGraph<T, EdgeT>& graph, T current, std::set<T>& visited, TreeGraph<T, EdgeT>& tree) {
	visited.insert(current);
	for (auto neighbour : graph.neighours(current)) {
		if (!visited.contains(neighbour)) {
			TreeGraph<T, EdgeT> subtree(neighbour);
			dfs(graph, neighbour, visited, subtree);
			tree.add_tree(current, subtree);
		}
	}
}

template<typename T, typename EdgeT = UndirectedEdge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> recursive_dfs(const UndirectedGraph<T, EdgeT>& graph, T start) {
	TreeGraph<T, EdgeT> result(start);
	std::set<T> visited;
	dfs(graph, start, visited, result);
	return result;
}


template<typename T, typename EdgeT = UndirectedEdge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> dijkstra(const UndirectedGraph<T, EdgeT>& graph, T start) {
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
		for (const auto& v : graph.neighours(u)) {
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

	// std::cout << distanze << std::endl;

	TreeGraph<T, EdgeT> res(graph.all_nodes(), precedente, start);
	return res;
}

template<typename T, typename EdgeT = UndirectedEdge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> dijkstra(const UndirectedGraph<T, EdgeT>& graph, T start, T end) {
	TreeGraph<T, EdgeT> albero = dijkstra(graph, start);
	std::map<T, T> precedente = albero.get_parent();

	// risali i padri da end fino a start, ricostruendo il cammino
	std::set<T> nodi{ end };
	std::map<T, T> cammino; // cammino[figlio] = padre
	T current = end;
	while (current != start) {
		auto it = precedente.find(current);
		if (it == precedente.end())
			throw std::invalid_argument("end non raggiungibile da start");
		cammino[current] = it->second;
		current = it->second;
		nodi.insert(current);
	}

	return TreeGraph<T, EdgeT>(nodi, cammino, start);
}

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
#include "functions.h"
#include "cycles.h"

template <typename T, typename EdgeT = Edge<T>>
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
	TreeGraph<T, EdgeT> search(Dispenser<std::pair<T, T>> &dispenser) const;
	TreeGraph<T, EdgeT> search(Dispenser<T> &dispenser) const;
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T>
SearchGraph<T, EdgeT>::SearchGraph(Graph<T, EdgeT> graph, T start)
	: graph_(std::move(graph))
{
	setStart(start);
}

template <typename T, typename EdgeT>
	requires std::totally_ordered<T>
void SearchGraph<T, EdgeT>::setGraph(Graph<T, EdgeT> graph, T start)
{
	graph_ = graph;
	setStart(start);
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T>
void SearchGraph<T, EdgeT>::setStart(T start)
{
	if (!graph_.has_node(start))
		throw std::out_of_range("node not found");
	start_ = start;
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::depth_first_search() const
{
	Lifo<std::pair<T, T>> lifo;
	return search(lifo);
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::breadth_first_search() const
{
	Fifo<std::pair<T, T>> fifo;
	return search(fifo);
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::search(Dispenser<std::pair<T, T>> &dispenser) const
{
	std::set<T> reached_nodes;
	std::map<T, T> parent; // parent[child] = padre nell'albero di ricerca

	dispenser.put({start_, start_});

	while (!dispenser.empty())
	{
		auto [actual, pusher] = dispenser.get();

		if (reached_nodes.contains(actual))
			continue;
		reached_nodes.insert(actual);
		if (actual != start_)
			parent[actual] = pusher;

		auto neighbours = graph_.neighbours(actual);
		for (auto it = neighbours.rbegin(); it != neighbours.rend(); ++it)
		{
			if (!reached_nodes.contains(*it))
			{
				dispenser.put({*it, actual});
			}
		}
	}

	return TreeGraph<T, EdgeT>(reached_nodes, parent, start_);
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> SearchGraph<T, EdgeT>::search(Dispenser<T> &dispenser) const
{
	std::set<T> reached_nodes;
	std::map<T, T> parent; // parent[child] = padre nell'albero di ricerca

	dispenser.put(start_);

	while (!dispenser.empty())
	{
		T actual = dispenser.get();

		if (reached_nodes.contains(actual))
			continue;
		reached_nodes.insert(actual);

		for (auto neighbour : graph_.neighbours(actual))
		{
			if (!reached_nodes.contains(neighbour))
			{
				if (!dispenser.contains(neighbour))
				{
					parent[neighbour] = actual;
				}
				dispenser.put(neighbour);
			}
		}
	}

	return TreeGraph<T, EdgeT>(reached_nodes, parent, start_);
};

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> graph_visit(Graph<T, EdgeT> graph, T start, Dispenser<std::pair<T, T>> &dispenser)
{
	return SearchGraph<T, EdgeT>(graph, start).search(dispenser);
};

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> graph_visit(Graph<T, EdgeT> graph, T start, Dispenser<T> &dispenser)
{
	return SearchGraph<T, EdgeT>(graph, start).search(dispenser);
};

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
void dfs(const Graph<T, EdgeT> &graph, T current, std::set<T> &visited, TreeGraph<T, EdgeT> &tree)
{
	visited.insert(current);
	for (auto neighbour : graph.neighbours(current))
	{
		if (!visited.contains(neighbour))
		{
			TreeGraph<T, EdgeT> subtree(neighbour);
			dfs(graph, neighbour, visited, subtree);
			tree.add_tree(current, subtree);
		}
	}
}

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> recursive_dfs(const Graph<T, EdgeT> &graph, T start)
{
	TreeGraph<T, EdgeT> result(start);
	std::set<T> visited;
	dfs(graph, start, visited, result);
	return result;
}

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> dijkstra(const Graph<T, EdgeT> &graph, T start)
{
	using Entry = std::pair<int, T>;
	constexpr int INF = std::numeric_limits<int>::max();

	std::map<T, int> distanze;
	std::map<T, T> precedente;

	for (const auto &v : graph.all_nodes())
		distanze[v] = INF;
	distanze[start] = 0;

	std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> q;
	q.emplace(0, start);

	while (!q.empty())
	{
		auto [d, u] = q.top();
		q.pop();
		if (d > distanze[u])
			continue;
		for (const auto &v : graph.neighbours(u))
		{
			int nd;
			if constexpr (requires(EdgeT e) { e.getWeight(); })
			{
				int w = 1;
				for (const auto &e : graph.all_edges())
				{
					if (e.has(u) && e.has(v))
					{
						w = e.getWeight();
						break;
					}
				}
				nd = d + w;
			}
			else
			{
				nd = d + 1;
			}
			if (nd < distanze[v])
			{
				distanze[v] = nd;
				precedente[v] = u;
				q.emplace(nd, v);
			}
		}
	}

	TreeGraph<T, EdgeT> res(graph.all_nodes(), precedente, start);
	return res;
}

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
TreeGraph<T, EdgeT> dijkstra(const Graph<T, EdgeT> &graph, T start, T end)
{
	TreeGraph<T, EdgeT> albero = dijkstra(graph, start);
	std::map<T, T> precedente = albero.get_parent();

	// risali i padri da end fino a start, ricostruendo il cammino
	std::set<T> nodi{end};
	std::map<T, T> cammino; // cammino[figlio] = padre
	T current = end;
	while (current != start)
	{
		auto it = precedente.find(current);
		if (it == precedente.end())
			throw std::invalid_argument("end non raggiungibile da start");
		cammino[current] = it->second;
		current = it->second;
		nodi.insert(current);
	}

	return TreeGraph<T, EdgeT>(nodi, cammino, start);
}

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
Cycles<T, EdgeT> depina_helper(const Graph<T, EdgeT> &graph, Cycles<T, EdgeT> S)
{
	std::set<Signed<T>> duplicati;
	std::set<T> all_nodes = graph.all_nodes();
	std::set<EdgeT> all_edges = graph.all_edges();
	for (const auto &v : all_nodes)
	{
		duplicati.insert({v, true});  // v⁺
		duplicati.insert({v, false}); // v⁻
	}

	std::set<Edge<Signed<T>>> archi;
	for (auto edge : all_edges)
	{
		if (S.is_active(edge))
		{
			archi.insert({{edge.from(), true}, {edge.to(), false}});
			archi.insert({{edge.from(), false}, {edge.to(), true}});
		}
		else
		{
			archi.insert({{edge.from(), true}, {edge.to(), true}});
			archi.insert({{edge.from(), false}, {edge.to(), false}});
		}
	}

	Graph<Signed<T>, Edge<Signed<T>>> ausiliario(duplicati, archi);

	std::vector<TreeGraph<Signed<T>, Edge<Signed<T>>>> walks;
	walks.reserve(all_nodes.size());
	for (auto node : all_nodes)
		walks.push_back(dijkstra(ausiliario, {node, false}, {node, true}));

	Cycles<T, EdgeT> best(all_edges);
	Cycles<T, EdgeT> test(all_edges);
	Cycles<T, EdgeT> sum(all_edges);
	for (auto walk : walks)
	{
		test.clear();
		Signed<T> actual = walk.get_root();
		Signed<T> next = walk.get_root();
		while (!walk.children(actual).empty())
		{
			sum.clear();
			next = walk.children(actual)[0];
			sum.add_edge({actual.value, next.value});
			test += sum;
			actual = next;
		}

		if (best.weight() == 0 || test.weight() < best.weight())
			best = test;
	}

	return best;
}

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T>
std::list<Cycles<T, EdgeT>> de_pina(const Graph<T, EdgeT> &graph)
{
	// std::list<Cycles<T, EdgeT>> de_pina(const Graph<T, EdgeT>& graph, std::vector<Cycles<T, EdgeT>> S){
	std::list<Cycles<T, EdgeT>> base;
	std::set<EdgeT> all_edges = graph.all_edges();
	int k = all_edges.size() - graph.all_nodes().size() + 1;

	// albero ricoprente: i suoi archi sono i "tree edges",
	// i restanti k archi sono i "non-tree edges"
	TreeGraph<T, EdgeT> tree = recursive_dfs(graph, *graph.all_nodes().begin());
	std::set<EdgeT> tree_edges = tree.all_edges();

	// k vettori di supporto S_i, ciascuno booleano di lunghezza m,
	// inizializzati al versore dell'i-esimo arco non-tree
	std::vector<Cycles<T, EdgeT>> S;
	S.reserve(k);
	for (const auto &edge : all_edges)
		if (!tree_edges.contains(edge))
			S.push_back(Cycles<T, EdgeT>(all_edges, {edge}));

	Cycles<T, EdgeT> ciclo(all_edges);
	for (auto i = 0; i < k; i++)
	{
		ciclo = depina_helper(graph, S[i]);
		// std::cout << "i: " << i << ;
		base.push_back(ciclo);
		for (auto j = i + 1; j < k; j++)
			if (ciclo * S[j])
				S[j] ^= S[i];
	}

	return base;
}

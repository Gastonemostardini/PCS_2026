#pragma once
#include "functions.h"
#include "undirected_edge.h"
#include "undirected_graph.h"
#include <set>
#include <algorithm>
#include <map>
#include <cstdint>

template <typename T, typename EdgeT = UndirectedEdge<T>>
	requires std::totally_ordered<T> && Printable<T>
class Cycles
{

	std::vector<bool> adjacency;
	std::set<UndirectedEdge<T>> real_edges;
	friend std::ostream &operator<<(std::ostream &os, const Cycles &obj)
	{
		// os << obj.adjacency;
		os << "{";
		bool first = true;
		std::size_t i = 0;
		for (const auto &edge : obj.real_edges)
		{
			if (obj.adjacency[i])
			{
				if (!first)
					os << ", ";
				os << edge;
				first = false;
			}
			++i;
		}
		os << "}";
		return os;
	}

public:
	Cycles(std::set<UndirectedEdge<T>> input_edges, std::set<UndirectedEdge<T>> edges)
		: adjacency(input_edges.size(), false), real_edges(input_edges)
	{
		std::size_t i = 0;
		for (const auto &edge : real_edges)
		{
			if (edges.contains(edge))
			{
				adjacency[i] = true;
			}
			i++;
		}
	}

	Cycles(std::set<UndirectedEdge<T>> input_edges)
		: adjacency(input_edges.size(), false), real_edges(input_edges)
	{
	}

	std::list<EdgeT> all_edges()
	{
		std::list<EdgeT> edges;
		std::size_t i = 0;
		for (const auto &edge : real_edges)
		{
			if (adjacency[i])
				edges.push_back(edge);
			i++;
		}
		return edges;
	};

	bool operator*(Cycles<T, EdgeT> other)
	{
		return adjacency * other.adjacency;
	};

	Cycles<T, EdgeT> operator^(Cycles<T, EdgeT> other)
	{
		Cycles<T, EdgeT> result(real_edges);
		result.adjacency = adjacency ^ other.adjacency;
		return result;
	};

	// Cycles& operator=(Cycles<T, EdgeT> other) {
	//	adjacency = other.adjacency;
	//	real_edges = other.real_edges;
	// };

	Cycles<T, EdgeT> &operator^=(Cycles<T, EdgeT> other)
	{
		adjacency = adjacency ^ other.adjacency;
		return *this;
	};

	Cycles<T, EdgeT> &operator+=(Cycles<T, EdgeT> other)
	{
		adjacency += other.adjacency;
		return *this;
	};

	void clear()
	{
		std::fill(adjacency.begin(), adjacency.end(), false);
	}

	std::size_t weight() const
	{
		return std::count(adjacency.begin(), adjacency.end(), true);
	}

	bool is_active(EdgeT edge) const
	{
		std::size_t i = 0;
		for (const auto &e : real_edges)
		{
			if (e == edge)
			{
				return adjacency[i];
			}
			i++;
		}
		return false;
	}

	void add_edge(EdgeT edge)
	{
		std::size_t i = 0;
		for (const auto &e : real_edges)
		{
			if (e == edge)
			{
				adjacency[i] = true;
			}
			i++;
		}
	}

	void remove_edge(EdgeT edge)
	{
		std::size_t i = 0;
		for (const auto &e : real_edges)
		{
			if (e == edge)
			{
				adjacency[i] = false;
			}
			i++;
		}
	}
};

// orientare le maglie in un percorso con versi (+1 / -1)
template <typename T, typename EdgeT = UndirectedEdge<T>>
	requires std::totally_ordered<T>
std::map<UndirectedEdge<T>, std::int8_t> orienta_maglia(Cycles<T, UndirectedEdge<T>> &ciclo, const UndirectedGraph<T, UndirectedEdge<T>> &graph)
{
	std::map<UndirectedEdge<T>, std::int8_t> segni;
	std::vector<UndirectedEdge<T>> rami;

	for (auto e : graph.all_edges())
	{ // estraiamo solo i rami di questa maglia
		if (ciclo.is_active(e))
			rami.push_back(e);
	}

	// Tirare fuori qualche genere di errore
	if (rami.empty())
		return segni;

	// Questa potremmo estrarla
	std::map<T, std::vector<UndirectedEdge<T>>> adj; // mappa di adiacenza per i rami del ciclo
	for (auto ramo : rami)
	{
		adj[ramo.from()].push_back(ramo);
		adj[ramo.to()].push_back(ramo);
	}

	// percorriamo il ciclo
	T current_node = rami[0].from();
	UndirectedEdge<T> current_edge = rami[0];
	T start_node = current_node;

	while (true)
	{
		if (current_edge.from() == current_node)
		{
			segni[current_edge] = 1;
			current_node = current_edge.to();
		}
		else
		{
			segni[current_edge] = -1;
			current_node = current_edge.from();
		}

		if (current_node == start_node)
			break;

		// prossimo ramo
		for (auto e : adj[current_node])
		{
			if (segni.find(e) == segni.end())
			{
				current_edge = e;
				break;
			}
		}
	}

	return segni;
}

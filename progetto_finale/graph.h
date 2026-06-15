#pragma once
#include <ostream>
#include "edge.h"
#include "functions.h"
#include <set>
#include <utility>

template <typename T, typename EdgeT = Edge<T>>
	requires std::totally_ordered<T> && Printable<T>
class Graph;

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
std::ostream &operator<<(std::ostream &os, const Graph<T, EdgeT> &obj);

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
class Graph
{
	template <typename U, typename EdgeU>
		requires std::totally_ordered<U> && Printable<U>
	friend class TreeGraph;

protected:
	std::set<T> nodes_;
	std::set<EdgeT> edges_;
	Graph() = default;

	void normalize();

	void fixIsolated();

	friend std::ostream &operator<< <>(std::ostream &os, const Graph<T, EdgeT> &obj);

public:
	/* COSTRUTTORI */
	Graph(std::set<T> nodes);
	Graph(std::set<EdgeT> edges);
	Graph(std::set<T> nodes, std::set<EdgeT> edges);
	Graph(const Graph &other);

	/* FUNZIONI */
	void add_edge(T from, T to);
	void add_edge(EdgeT edge);
	void remove_node(T node);
	std::set<T> neighbours(T value) const;
	std::set<EdgeT> all_edges() const;
	std::set<T> all_nodes() const;
	unsigned int edge_number(const EdgeT edge) const;
	const EdgeT &edge_at(unsigned int i) const;
	bool has_node(T &target) const;

	/* OPERATORI */
	Graph<T, EdgeT> operator-(const Graph<T, EdgeT> &other);
	bool operator==(const Graph<T, EdgeT> &other);
	bool operator!=(const Graph<T, EdgeT> &other);
};

template <typename EdgeT>
Graph(std::set<EdgeT>) -> Graph<decltype(std::declval<EdgeT>().from()), EdgeT>;

template <typename T, typename EdgeT>
Graph(std::set<T>, std::set<EdgeT>) -> Graph<T, EdgeT>;

template <typename T, typename EdgeT>
	requires Printable<T>
std::ostream &operator<<(std::ostream &os, const Graph<T, EdgeT> &obj)
{
	os << "nodes: \n";
	os << obj.nodes_ << std::endl;
	os << "edges: \n";
	os << obj.edges_ << std::endl;
	return os;
}

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
Graph<T, EdgeT>::Graph(std::set<T> nodes)
{
	nodes_ = nodes;
	normalize();
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
Graph<T, EdgeT>::Graph(std::set<EdgeT> edges)
{
	for (auto edge : edges)
	{
		add_edge(edge);
	}
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
Graph<T, EdgeT>::Graph(std::set<T> nodes, std::set<EdgeT> edges)
{
	nodes_ = nodes;

	for (auto edge : edges)
	{
		add_edge(edge);
	}

	normalize();
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
Graph<T, EdgeT>::Graph(const Graph &other)
{
	nodes_ = other.nodes_;
	for (auto edge : other.edges_)
	{
		add_edge(edge);
	}
	normalize();
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
std::set<T> Graph<T, EdgeT>::neighbours(T value) const
{
	std::set<T> neighbours;

	for (auto edge : edges_)
		if (edge.has(value))
			neighbours.insert(edge.from() == value ? edge.to() : edge.from());

	return neighbours;
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
void Graph<T, EdgeT>::add_edge(T from, T to)
{
	EdgeT edge(from, to);
	add_edge(edge);
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
void Graph<T, EdgeT>::add_edge(EdgeT edge)
{
	if (edge.from() == edge.to())
		return;
	nodes_.insert(edge.from());
	nodes_.insert(edge.to());
	edges_.insert(edge);
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
void Graph<T, EdgeT>::remove_node(T node)
{
	nodes_.erase(node);
	for (auto edge : edges_)
	{
		if (edge.from() == node || edge.to() == node)
			edges_.erase(edge);
	}
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
std::set<EdgeT> Graph<T, EdgeT>::all_edges() const
{
	return edges_;
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
std::set<T> Graph<T, EdgeT>::all_nodes() const
{
	return nodes_;
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
bool Graph<T, EdgeT>::has_node(T &target) const
{
	return nodes_.contains(target);
}

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
unsigned int Graph<T, EdgeT>::edge_number(const EdgeT edge) const
{
	auto it = edges_.find(edge);
	if (it == edges_.end())
	{
		throw std::out_of_range("edge not found");
	}
	return static_cast<unsigned int>(std::distance(edges_.begin(), it));
}

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
const EdgeT &Graph<T, EdgeT>::edge_at(unsigned int i) const
{
	if (i >= edges_.size())
	{
		throw std::out_of_range("edge index out of range");
	}
	auto it = edges_.begin();
	std::advance(it, i);
	return *it;
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
void Graph<T, EdgeT>::normalize()
{
	fixIsolated();
}

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
void Graph<T, EdgeT>::fixIsolated()
{
	return;
	for (auto node : nodes_)
	{
		if (neighbours(node).empty())
		{
			add_edge(node, node);
		}
	}
}

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
Graph<T, EdgeT> Graph<T, EdgeT>::operator-(const Graph<T, EdgeT> &other)
{
	std::set<EdgeT> edges = edges_;
	std::set<T> nodes = nodes_;
	for (auto edge : other.edges_)
	{
		edges.erase(edge);
	}

	nodes.clear();
	for (auto edge : edges)
	{
		nodes.insert(edge.from());
		nodes.insert(edge.to());
	}
	Graph newObj(nodes, edges);
	return newObj;
};

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
bool Graph<T, EdgeT>::operator==(const Graph<T, EdgeT> &other)
{
	if (all_nodes() != other.all_nodes())
		return false;
	if (all_edges() != other.all_edges())
		return false;
	return true;
}

template <typename T, typename EdgeT>
	requires std::totally_ordered<T> && Printable<T>
bool Graph<T, EdgeT>::operator!=(const Graph<T, EdgeT> &other)
{
	if (all_nodes() != other.all_nodes())
		return true;
	if (all_edges() != other.all_edges())
		return true;
	return false;
}

#pragma once
#include <vector>
#include <set>
#include <list>
#include <istream>
#include <ostream>
#include <concepts>
#include "undirected_edge.h"
#include "undirected_graph.h"
#include "undirected_tree.h"

template <typename T>
concept Printable = requires(std::ostream &os, std::istream &is, T &value) {
    { os << value } -> std::same_as<std::ostream &>;
};

bool operator*(std::vector<bool> a, std::vector<bool> b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    int sum = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i] && b[i])
            sum++;
    }

    return sum % 2;
}

std::vector<bool> &operator+=(std::vector<bool> &a, const std::vector<bool> &b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    for (size_t i = 0; i < a.size(); i++)
    {
        a[i] = (a[i] + b[i]) % 2;
    }

    return a;
}

std::vector<bool> operator^(std::vector<bool> a, std::vector<bool> b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    std::vector<bool> res(a.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        res[i] = a[i] ^ b[i];
    }
    return res;
}

#include "cycles.h"

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::list<T> &obj)
{
    if (obj.empty())
    {
        os << "vettore vuoto";
        return os;
    }

    os << "(";
    for (auto it = obj.begin(); it != obj.end(); ++it)
    {
        if (it != obj.begin())
            os << ", ";
        os << *it;
    }
    os << ")";
    return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::set<T> obj)
{
    for (auto elm : obj)
    {
        os << elm << " ";
    }
    return os;
}

template <typename T>
struct Signed
{
    T value;
    bool positive;

    auto operator<=>(const Signed &) const = default;
};

template <typename T>
    requires Printable<T>
std::ostream &operator<<(std::ostream &os, const Signed<T> &obj)
{
    os << "(" << obj.value << (obj.positive ? "+" : "-") << ")";
    return os;
}

template <typename T, typename EdgeT = UndirectedEdge<T>>
        requires std::totally_ordered<T> && Printable<T>
Cycles<T, EdgeT> depina_helper(const UndirectedGraph<T, EdgeT> &graph, Cycles<T, EdgeT> S)
{
        std::set<Signed<T>> duplicati;
        std::set<T> all_nodes = graph.all_nodes();
        std::set<EdgeT> all_edges = graph.all_edges();
        for (const auto &v : all_nodes)
        {
                duplicati.insert({v, true});  // v⁺
                duplicati.insert({v, false}); // v⁻
        }

        std::set<UndirectedEdge<Signed<T>>> archi;
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

        UndirectedGraph<Signed<T>, UndirectedEdge<Signed<T>>> ausiliario(duplicati, archi);

        std::vector<TreeGraph<Signed<T>, UndirectedEdge<Signed<T>>>> walks;
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

template <typename T, typename EdgeT = UndirectedEdge<T>>
        requires std::totally_ordered<T>
std::list<Cycles<T, EdgeT>> de_pina(const UndirectedGraph<T, EdgeT> &graph)
{
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

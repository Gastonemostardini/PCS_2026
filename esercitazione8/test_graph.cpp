#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include "undirected_graph.h"
#include "undirected_edge.h"

using namespace std;

class UndirectedGraphTest {
    set<UndirectedEdge<int>> edges;
    set<int> nodes;

public:
    // COSTRUTTORI
    int test_constructor_from_nodes() {
        nodes = { 2, 5, 32, 675, 32, 0 };
        edges.clear();
        for (auto node : nodes) {
            edges.insert({ node, node });
        }

        UndirectedGraph graph(nodes);

        if(nodes != graph.all_nodes())
            return EXIT_FAILURE;

        if (edges != graph.all_edges())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_constructor_from_nodes_empty() {
        nodes = {};
        UndirectedGraph graph(nodes);

        if (!graph.all_nodes().empty())
            return EXIT_FAILURE;

        if (!graph.all_edges().empty())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_constructor_from_edges() {

        edges = { {2, 5}, {2, 32}, {675, 0}, {32, 0} };
        nodes = { 2, 5, 32, 675, 32, 0 };

        UndirectedGraph graph(edges);

        if (nodes != graph.all_nodes())
            return EXIT_FAILURE;

        if (edges != graph.all_edges())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_constructor_from_nodes_and_edges() {

        edges = { {2, 5}, {2, 32}, {675, 0}, {32, 0}, {89, 62} };
        nodes = { 2, 5, 32, 675, 32, 0, 4, 2, 77 };

        UndirectedGraph graph(nodes, edges);

        edges.insert({ 4, 4 });
        edges.insert({ 77, 77 });
        nodes.insert({ 32, 0, 4, 77, 89, 62 });

        if (nodes != graph.all_nodes())
            return EXIT_FAILURE;

        if (edges != graph.all_edges())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_copy_constructor() {

        edges = { {2, 5}, {2, 32}, {675, 0}, {32, 0}, {89, 62} };
        nodes = { 2, 5, 32, 675, 32, 0, 4, 2, 77 };

        UndirectedGraph graph(nodes, edges);
        UndirectedGraph graph2(graph);

        if (graph != graph2)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_copy_constructor_independence() {

        edges = { {2, 5}, {2, 32}, {675, 0}, {32, 0}, {89, 62} };
        nodes = { 2, 5, 32, 675, 32, 0, 4, 2, 77 };

        UndirectedGraph graph(nodes, edges);
        UndirectedGraph graph2(graph);

        edges.insert({ 4, 4 });
        edges.insert({ 77, 77 });
        edges.insert({ 1, 5 });
        nodes.insert({ 1, 5, 32, 0, 4, 77, 89, 62 });
        graph.add_edge({ 1, 2222 });
        graph2.add_edge({ 1, 5 });

        if (nodes != graph2.all_nodes())
            return EXIT_FAILURE;

        if (edges != graph2.all_edges())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // add_edge
    int test_add_edge_by_values() {
        edges = { {2, 5}, {2, 32}, {675, 0}, {32, 0} };
        nodes = { 2, 5, 32, 675, 32, 0, 2 };

        UndirectedGraph graph(nodes, edges);

        graph.add_edge({ 7, 2 });

        edges.insert({ 2, 7 });
        nodes.insert({ 7 });

        if (nodes != graph.all_nodes())
            return EXIT_FAILURE;
        

        if (edges != graph.all_edges())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_add_edge_by_object() {

        edges = { {2, 5}, {2, 32}, {675, 0}, {32, 0} };
        nodes = { 2, 5, 32, 675, 32, 0, 2 };

        UndirectedGraph graph(nodes, edges);

        UndirectedEdge edge = { 7, 2 };

        graph.add_edge(edge);

        edges.insert({ 2, 7 });
        nodes.insert({ 7 });

        if (nodes != graph.all_nodes())
            return EXIT_FAILURE;

        if (edges != graph.all_edges())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_add_edge_duplicate() {

        edges = { {2, 5}, {2, 32}, {675, 0}, {32, 0} };
        nodes = { 2, 5, 32, 675, 32, 0, 2 };

        UndirectedGraph graph(nodes, edges);

        UndirectedEdge edge = { 2, 5 };

        graph.add_edge(edge);

        if (nodes != graph.all_nodes())
            return EXIT_FAILURE;

        if (edges != graph.all_edges())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_add_edge_symmetry() {
        nodes = { 1, 2 };
        UndirectedGraph graph(nodes);

        graph.add_edge({ 1, 2 });
        graph.add_edge({ 2, 1 });

        if (graph.all_edges().size() != 3) 
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // neighours / all_nodes / all_edges
    int test_neighbours_basic() {
        nodes = { 1, 2, 3, 4 };
        edges = { {1, 2}, { 2, 3 }, {3, 4}, {4, 1} };
        UndirectedGraph graph(nodes, edges);

        set<int> expected = { 2, 4 };

        if (graph.neighours(1) != expected) 
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_neighbours_isolated_node() {
        nodes = { 1, 2, 3, 4 };
        edges = { {1, 2}, { 2, 3 } };
        UndirectedGraph graph(nodes, edges);

        set<int> expected = { 4 };

        if (graph.neighours(4) != expected)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_neighbours_symmetry() {
        nodes = { 1, 2 };
        edges = { {1, 2} };
        UndirectedGraph graph(nodes, edges);

        set<int> expected1 = { 2 };
        set<int> expected2 = { 1 };

        if (graph.neighours(1) != expected1 || (graph.neighours(2)) != expected2)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // edge_number / edge_at
    int test_edge_number_existing() {
        nodes = { 1, 2, 3 };
        UndirectedGraph<int> graph(nodes);

        graph.add_edge(1, 2);
        UndirectedEdge<int> edge(1, 2);

        if (graph.edge_number(edge) != 1) 
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }

    int test_edge_number_missing() {
        nodes = { 1, 2, 3 };
        UndirectedGraph<int> graph(nodes);

        graph.add_edge(1, 2);
        UndirectedEdge<int> edge(1, 99);

        try {
            graph.edge_number(edge);
            return EXIT_FAILURE;
        }
        catch (const std::out_of_range&) {
            return EXIT_SUCCESS;
        }
        catch (...) {
            return EXIT_FAILURE;
        }
    }

    int test_edge_at_valid_index() {

        nodes = { 1, 2, 3 };
        UndirectedGraph<int> graph(nodes);

        graph.add_edge(1, 2);
        UndirectedEdge<int> edge(1, 2);

        if (graph.edge_at(1) != edge)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_edge_at_out_of_range() {

        nodes = { 1, 2, 3 };
        UndirectedGraph<int> graph(nodes);

        graph.add_edge(1, 2);

        try {
            graph.edge_at(99);
            return EXIT_FAILURE;
        }
        catch (const std::out_of_range&) {
            return EXIT_SUCCESS;
        }
        catch (...) {
            return EXIT_FAILURE;
        }
    }

    // operator-
    int test_operator_minus_basic() {

        int a = 1;
        int b = 4;
        int c = 5;

        UndirectedEdge<int> ub(b, c);
        UndirectedEdge<int> uc(a, c);

        nodes = { c, 86 };
        edges = { ub, uc };
        set<UndirectedEdge<int>> edges2 = { ub };

        UndirectedGraph<int> graph(nodes);
        UndirectedGraph<int> graph2(edges); 
        UndirectedGraph<int> graph3 = graph - graph2;
        UndirectedGraph<int> graph4({ c, 86 }, { uc });

        if (graph3 == graph4)
            return EXIT_SUCCESS;
        else
            return EXIT_FAILURE;
    }

    int test_operator_minus_empty_result() {
        nodes = { 1, 2, 3 };
        UndirectedGraph<int> graph(nodes);
        UndirectedGraph<int> graph2(nodes);

        UndirectedGraph<int> graph3 = graph - graph2;

        if (!graph3.all_edges().empty() || !graph3.all_nodes().empty())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int test_operator_minus_no_overlap() {

        nodes = { 1, 2, 3 };
        UndirectedGraph<int> graph(nodes);
        nodes = { 4, 5, 6 };
        UndirectedGraph<int> graph2(nodes);

        UndirectedGraph<int> graph3 = graph - graph2;

        if (graph3 != graph)
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }

    int test_operator_minus_does_not_modify_operands() {
        nodes = { 1, 2, 3 };
        UndirectedGraph<int> graph(nodes);
        nodes = { 4, 5, 6 };
        UndirectedGraph<int> graph2(nodes);

        UndirectedGraph<int> graph3 = graph - graph2;

        set<int> expected_nodes = { 1, 2, 3 };
        set<UndirectedEdge<int>> expected_edges = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
        if (graph.all_nodes() != expected_nodes || graph.all_edges() != expected_edges)
            return EXIT_FAILURE;

        expected_nodes = { 4, 5, 6 };
        expected_edges = { { 4, 4 }, { 5, 5 }, { 6, 6 } };
        if (graph2.all_nodes() != expected_nodes || graph2.all_edges() != expected_edges)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

};

int main() {
    UndirectedGraphTest t;

    if (t.test_constructor_from_nodes() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_constructor_from_nodes_empty() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_constructor_from_edges() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_constructor_from_nodes_and_edges() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_copy_constructor() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_copy_constructor_independence() != EXIT_SUCCESS) return EXIT_FAILURE;

    if (t.test_add_edge_by_values() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_add_edge_by_object() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_add_edge_duplicate() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_add_edge_symmetry() != EXIT_SUCCESS) return EXIT_FAILURE;

    if (t.test_neighbours_basic() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_neighbours_isolated_node() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_neighbours_symmetry() != EXIT_SUCCESS) return EXIT_FAILURE;

    if (t.test_edge_number_existing() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_edge_number_missing() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_edge_at_valid_index() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_edge_at_out_of_range() != EXIT_SUCCESS) return EXIT_FAILURE;

    if (t.test_operator_minus_basic() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_operator_minus_empty_result() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_operator_minus_no_overlap() != EXIT_SUCCESS) return EXIT_FAILURE;
    if (t.test_operator_minus_does_not_modify_operands() != EXIT_SUCCESS) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
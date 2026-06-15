#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <cstdlib>
#include <stdexcept>
#include "../edge.h"
#include "../graph.h"
#include "../tree.h"
#include "../search_graph.h"

using namespace std;

// Test per SearchGraph e per l'individuazione dei cicli fondamentali tramite
// albero di supporto (DFS / BFS) e coalbero (C = G \ T).
class SearchGraphTest
{
    // Grafo di esempio del progetto (4 nodi, 5 archi):
    // due triangoli 1-2-4 e 2-3-4 che condividono il lato (2,4).
    static Graph<int> example_graph()
    {
        return Graph<int>({1, 2, 3, 4},
                          {{1, 2}, {2, 4}, {1, 4}, {2, 3}, {3, 4}});
    }

public:
    /* ---------- Albero di supporto (DFS / BFS) ---------- */

    // La DFS deve produrre un albero che copre tutti i nodi del grafo connesso.
    int test_dfs_spanning_tree_covers_all_nodes()
    {
        try
        {
            Graph<int> graph = example_graph();
            SearchGraph<int> sg(graph, 1);
            TreeGraph<int> tree = sg.depth_first_search();
            if (tree.all_nodes() != graph.all_nodes())
            {
                cerr << "  l'albero DFS non copre tutti i nodi" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Un albero di supporto di un grafo connesso ha esattamente |V| - 1 archi.
    int test_dfs_spanning_tree_edge_count()
    {
        try
        {
            Graph<int> graph = example_graph();
            SearchGraph<int> sg(graph, 1);
            TreeGraph<int> tree = sg.depth_first_search();
            size_t expected = graph.all_nodes().size() - 1;
            if (tree.all_edges().size() != expected)
            {
                cerr << "  attesi " << expected << " archi, ottenuti "
                     << tree.all_edges().size() << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // La BFS deve produrre un albero che copre tutti i nodi del grafo connesso.
    int test_bfs_spanning_tree_covers_all_nodes()
    {
        try
        {
            Graph<int> graph = example_graph();
            SearchGraph<int> sg(graph, 1);
            TreeGraph<int> tree = sg.breadth_first_search();
            if (tree.all_nodes() != graph.all_nodes())
            {
                cerr << "  l'albero BFS non copre tutti i nodi" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Impostare uno start non presente nel grafo deve lanciare out_of_range.
    int test_search_invalid_start_throws()
    {
        Graph<int> graph = example_graph();
        try
        {
            SearchGraph<int> sg(graph, 99);
            (void)sg;
            return EXIT_FAILURE; // doveva lanciare
        }
        catch (const out_of_range &)
        {
            return EXIT_SUCCESS;
        }
    }

    // Il coalbero C = G \ T contiene esattamente |E| - |V| + 1 archi,
    // ovvero il numero di cicli fondamentali (maglie).
    int test_coalbero_edge_count()
    {
        try
        {
            Graph<int> graph = example_graph();
            SearchGraph<int> sg(graph, 1);
            TreeGraph<int> tree = sg.depth_first_search();
            Graph<int> coalbero = graph - tree;

            size_t expected = graph.all_edges().size() - graph.all_nodes().size() + 1;
            if (coalbero.all_edges().size() != expected)
            {
                cerr << "  attesi " << expected << " archi nel coalbero, ottenuti "
                     << coalbero.all_edges().size() << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // operator- deve restituire il grafo dei soli archi non presenti nell'albero.
    int test_coalbero_is_graph_minus_tree()
    {
        try
        {
            Graph<int> graph = example_graph();
            SearchGraph<int> sg(graph, 1);
            TreeGraph<int> tree = sg.depth_first_search();
            Graph<int> coalbero = graph - tree;

            set<Edge<int>> tree_edges = tree.all_edges();
            set<Edge<int>> expected;
            for (const auto &e : graph.all_edges())
                if (!tree_edges.contains(e))
                    expected.insert(e);

            if (coalbero.all_edges() != expected)
            {
                cerr << "  il coalbero non coincide con G \\ T" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Numero di cicli fondamentali su un grafo noto (es. circuito di Figura 3).
    int test_fundamental_cycle_count_on_example()
    {
        try
        {
            Graph<int> graph = example_graph();
            SearchGraph<int> sg(graph, 1);
            TreeGraph<int> tree = sg.depth_first_search();
            Graph<int> coalbero = graph - tree;

            // |E| - |V| + 1 = 5 - 4 + 1 = 2
            size_t expected = graph.all_edges().size() - graph.all_nodes().size() + 1;
            if (expected != 2)
                return EXIT_FAILURE;
            if (coalbero.all_edges().size() != expected)
            {
                cerr << "  attese " << expected << " maglie, ottenute "
                     << coalbero.all_edges().size() << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }
};

int main()
{
    SearchGraphTest t;

    if (t.test_dfs_spanning_tree_covers_all_nodes() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_dfs_spanning_tree_covers_all_nodes" << endl;
    if (t.test_dfs_spanning_tree_edge_count() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_dfs_spanning_tree_edge_count" << endl;
    if (t.test_bfs_spanning_tree_covers_all_nodes() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_bfs_spanning_tree_covers_all_nodes" << endl;
    if (t.test_search_invalid_start_throws() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_search_invalid_start_throws" << endl;
    if (t.test_coalbero_edge_count() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_coalbero_edge_count" << endl;
    if (t.test_coalbero_is_graph_minus_tree() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_coalbero_is_graph_minus_tree" << endl;
    if (t.test_fundamental_cycle_count_on_example() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_fundamental_cycle_count_on_example" << endl;

    return EXIT_SUCCESS;
}

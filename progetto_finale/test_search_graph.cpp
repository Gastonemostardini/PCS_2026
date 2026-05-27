#include <iostream>
#include <set>
#include <stdexcept>
#include "edge.h"
#include "graph.h"
#include "tree.h"
#include "search_graph.h"

using namespace std;

// Scheletro dei test per SearchGraph e per l'individuazione dei cicli
// fondamentali tramite albero di supporto (DFS) e coalbero (C = G \ T).
// Riferimento: sezione 5.2 del progetto.
//
// Tutti i metodi ritornano EXIT_FAILURE finche non sono implementati:
// in questo modo i test "non implementati" risultano subito rossi.
class SearchGraphTest {
    set<int> nodes;
    set<Edge<int>> edges;

public:
    /* ---------- Albero di supporto (DFS / BFS) ---------- */

    // La DFS deve produrre un albero che copre tutti i nodi del grafo connesso.
    int test_dfs_spanning_tree_covers_all_nodes() {
        // TODO: costruire un grafo connesso, eseguire depth_first_search()
        //       e verificare che all_nodes() dell'albero == all_nodes() del grafo.
        return EXIT_FAILURE;
    }

    // Un albero di supporto di un grafo connesso ha esattamente |V| - 1 archi.
    int test_dfs_spanning_tree_edge_count() {
        // TODO: verificare che l'albero DFS abbia |V| - 1 archi.
        return EXIT_FAILURE;
    }

    // La BFS deve produrre un albero che copre tutti i nodi del grafo connesso.
    int test_bfs_spanning_tree_covers_all_nodes() {
        // TODO: costruire un grafo connesso, eseguire breadth_first_search()
        //       e verificare la copertura dei nodi.
        return EXIT_FAILURE;
    }

    // DFS iterativa e DFS ricorsiva devono coprire gli stessi nodi.
    int test_recursive_dfs_covers_all_nodes() {
        // TODO: confrontare i nodi raggiunti da recursive_dfs() e da depth_first_search().
        return EXIT_FAILURE;
    }

    // Impostare uno start non presente nel grafo deve lanciare out_of_range.
    int test_search_invalid_start_throws() {
        // TODO: try { SearchGraph(graph, nodo_inesistente); } catch (out_of_range) { ... }
        return EXIT_FAILURE;
    }

    /* ---------- Coalbero e cicli fondamentali ---------- */

    // Il coalbero C = G \ T contiene esattamente |E| - |V| + 1 archi,
    // ovvero il numero di cicli fondamentali (maglie).
    int test_coalbero_edge_count() {
        // TODO: calcolare T con la DFS, poi C = graph - T e verificare
        //       che il numero di archi di C sia |E| - |V| + 1.
        return EXIT_FAILURE;
    }

    // operator- deve restituire il grafo dei soli archi non presenti nell'albero.
    int test_coalbero_is_graph_minus_tree() {
        // TODO: verificare che (graph - T) contenga tutti e soli gli archi del coalbero attesi.
        return EXIT_FAILURE;
    }

    // Reinserire un arco del coalbero nell'albero T deve chiudere esattamente una maglia.
    int test_reinsert_coalbero_edge_closes_cycle() {
        // TODO: scegliere un arco del coalbero, individuare il cammino tra i suoi
        //       estremi nell'albero e verificare che formi un ciclo valido del grafo.
        return EXIT_FAILURE;
    }

    // Numero di cicli fondamentali su un grafo noto (es. circuito di Figura 3).
    int test_fundamental_cycle_count_on_example() {
        // TODO: costruire il grafo del circuito d'esempio e verificare
        //       che i cicli fondamentali siano |E| - |V| + 1 = 2.
        return EXIT_FAILURE;
    }
};

int main() {
    SearchGraphTest t;

    if (t.test_dfs_spanning_tree_covers_all_nodes() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_dfs_spanning_tree_covers_all_nodes" << endl;
    if (t.test_dfs_spanning_tree_edge_count() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_dfs_spanning_tree_edge_count" << endl;
    if (t.test_bfs_spanning_tree_covers_all_nodes() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_bfs_spanning_tree_covers_all_nodes" << endl;
    if (t.test_recursive_dfs_covers_all_nodes() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_recursive_dfs_covers_all_nodes" << endl;
    if (t.test_search_invalid_start_throws() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_search_invalid_start_throws" << endl;
    if (t.test_coalbero_edge_count() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_coalbero_edge_count" << endl;
    if (t.test_coalbero_is_graph_minus_tree() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_coalbero_is_graph_minus_tree" << endl;
    if (t.test_reinsert_coalbero_edge_closes_cycle() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_reinsert_coalbero_edge_closes_cycle" << endl;
    if (t.test_fundamental_cycle_count_on_example() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_fundamental_cycle_count_on_example" << endl;

    return EXIT_SUCCESS;
}

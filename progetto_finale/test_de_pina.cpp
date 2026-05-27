#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <cstdlib>
#include <stdexcept>
#include "edge.h"
#include "graph.h"
#include "cycles.h"
#include "search_graph.h"

using namespace std;

// Grafo di esempio (4 nodi, 5 archi)
static Graph<int> example_graph() {
    return Graph<int>({ 1, 2, 3, 4 },
                      { {1, 2}, {2, 4}, {1, 4}, {2, 3}, {3, 4} });
}

// Vero se il ciclo `c` e' attivo esattamente sugli archi di `expected`.
static bool cycle_is(Cycles<int> c, const set<Edge<int>>& expected) {
    if (c.weight() != expected.size()) return false;
    for (const auto& e : expected)
        if (!c.is_active(e)) return false;
    return true;
}

// Vettore di incidenza di `c` secondo l'ordine degli archi in `edges`.
static vector<bool> incidence(Cycles<int> c, const set<Edge<int>>& edges) {
    vector<bool> v;
    v.reserve(edges.size());
    for (const auto& e : edges)
        v.push_back(c.is_active(e));
    return v;
}

// Rango in GF(2) di un insieme di vettori (eliminazione di Gauss mod 2).
static size_t gf2_rank(vector<vector<bool>> rows) {
    if (rows.empty()) return 0;
    size_t cols = rows.front().size();
    size_t rank = 0;
    for (size_t col = 0; col < cols && rank < rows.size(); ++col) {
        size_t pivot = rank;
        while (pivot < rows.size() && !rows[pivot][col]) ++pivot;
        if (pivot == rows.size()) continue;
        swap(rows[rank], rows[pivot]);
        for (size_t r = 0; r < rows.size(); ++r) {
            if (r != rank && rows[r][col]) {
                for (size_t b = 0; b < cols; ++b)
                    rows[r][b] = rows[r][b] ^ rows[rank][b];
            }
        }
        ++rank;
    }
    return rank;
}

class DePinaTest {
public:
    // La base restituita deve avere cardinalita |E| - |V| + 1.
    int test_returns_correct_cycle_count() {
        try {
            Graph<int> graph = example_graph();
            auto base = de_pina(graph);
            size_t expected = graph.all_edges().size()
                            - graph.all_nodes().size() + 1;
            if (base.size() != expected) {
                cerr << "  attesi " << expected << " cicli, ottenuti "
                     << base.size() << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // I cicli della base devono essere linearmente indipendenti in GF(2):
    // il rango della matrice di incidenza deve eguagliare il numero di cicli.
    int test_cycles_are_linearly_independent() {
        try {
            Graph<int> graph = example_graph();
            auto base = de_pina(graph);
            set<Edge<int>> edges = graph.all_edges();

            vector<vector<bool>> rows;
            for (const auto& c : base)
                rows.push_back(incidence(c, edges));

            size_t rank = gf2_rank(rows);
            if (rank != base.size()) {
                cerr << "  rango " << rank << " != numero cicli "
                     << base.size() << " (cicli dipendenti)" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // De Pina deve trovare i cicli MINIMI: sul grafo di esempio la base e'
    // costituita dai due triangoli (peso 3 ciascuno), per un peso totale 6.
    // Una base non minima (es. con il quadrilatero, peso 4) darebbe peso 7.
    int test_finds_minimal_cycle_basis() {
        try {
            Graph<int> graph = example_graph();
            auto base = de_pina(graph);
            if (base.size() != 2) {
                cerr << "  attesi 2 cicli, ottenuti " << base.size() << endl;
                return EXIT_FAILURE;
            }
            size_t total = 0;
            for (const auto& c : base) total += c.weight();
            if (total != 6) {
                cerr << "  peso totale base = " << total
                     << ", atteso 6 (base non minima)" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Su un grafo che e' un solo ciclo (triangolo), la base deve contenere
    // esattamente quel ciclo.
    int test_single_cycle_graph() {
        try {
            Graph<int> graph({ 1, 2, 3 }, { {1, 2}, {2, 3}, {1, 3} });
            auto base = de_pina(graph);
            if (base.size() != 1) {
                cerr << "  attesi 1 ciclo, ottenuti " << base.size() << endl;
                return EXIT_FAILURE;
            }
            if (!cycle_is(base.front(), { {1, 2}, {2, 3}, {1, 3} })) {
                cerr << "  il ciclo trovato non e' il triangolo 1-2-3" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Un grafo aciclico (un cammino) non ha cicli: la base deve essere vuota.
    int test_tree_graph_has_empty_basis() {
        try {
            Graph<int> graph({ 1, 2, 3, 4 }, { {1, 2}, {2, 3}, {3, 4} });
            auto base = de_pina(graph);
            if (!base.empty()) {
                cerr << "  attesa base vuota, ottenuti " << base.size()
                     << " cicli" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Sul circuito d'esempio del progetto la base deve essere esattamente
    // {triangolo 1-2-4, triangolo 2-3-4}.
    int test_on_example_circuit() {
        try {
            Graph<int> graph = example_graph();
            auto base = de_pina(graph);
            if (base.size() != 2) {
                cerr << "  attesi 2 cicli, ottenuti " << base.size() << endl;
                return EXIT_FAILURE;
            }
            set<Edge<int>> tri124 = { {1, 2}, {2, 4}, {1, 4} };
            set<Edge<int>> tri234 = { {2, 3}, {3, 4}, {2, 4} };
            bool found124 = false, found234 = false;
            for (const auto& c : base) {
                if (cycle_is(c, tri124)) found124 = true;
                if (cycle_is(c, tri234)) found234 = true;
            }
            if (!found124 || !found234) {
                cerr << "  base != {triangolo 1-2-4, triangolo 2-3-4}" << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        } catch (const exception& e) {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }
};

int main() {
    DePinaTest t;

    if (t.test_returns_correct_cycle_count() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_returns_correct_cycle_count" << endl;
    if (t.test_cycles_are_linearly_independent() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_cycles_are_linearly_independent" << endl;
    if (t.test_finds_minimal_cycle_basis() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_finds_minimal_cycle_basis" << endl;
    if (t.test_single_cycle_graph() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_single_cycle_graph" << endl;
    if (t.test_tree_graph_has_empty_basis() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_tree_graph_has_empty_basis" << endl;
    if (t.test_on_example_circuit() != EXIT_SUCCESS) return EXIT_FAILURE;
    cout << "test_on_example_circuit" << endl;

    return EXIT_SUCCESS;
}

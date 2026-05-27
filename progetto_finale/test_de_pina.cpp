#include <iostream>
#include <list>
#include <set>
#include "edge.h"
#include "graph.h"
#include "cycles.h"
#include "search_graph.h"

using namespace std;

// Scheletro dei test per l'algoritmo di De Pina (de_pina), che calcola una
// base di cicli minimi del grafo. Riferimento: sezione 5.2 del progetto.
//
// Una base di cicli di un grafo connesso ha cardinalita |E| - |V| + 1 e i suoi
// elementi devono essere linearmente indipendenti in GF(2).
//
// Tutti i metodi ritornano EXIT_FAILURE finche non sono implementati.
class DePinaTest {
    set<int> nodes;
    set<Edge<int>> edges;

public:
    // La base restituita deve avere cardinalita |E| - |V| + 1.
    int test_returns_correct_cycle_count() {
        // TODO: costruire un grafo connesso e verificare che de_pina(graph).size()
        //       sia uguale a |E| - |V| + 1.
        return EXIT_FAILURE;
    }

    // I cicli della base devono essere linearmente indipendenti in GF(2).
    int test_cycles_are_linearly_independent() {
        // TODO: verificare l'indipendenza lineare dei cicli (nessuno ottenibile
        //       come xor degli altri).
        return EXIT_FAILURE;
    }

    // De Pina deve trovare i cicli MINIMI (a differenza del metodo coalbero).
    int test_finds_minimal_cycle_basis() {
        // TODO: su un grafo con cicli di lunghezze diverse, verificare che la base
        //       trovata sia quella di peso/lunghezza minima.
        return EXIT_FAILURE;
    }

    // Su un grafo con un solo ciclo, la base deve contenere esattamente quel ciclo.
    int test_single_cycle_graph() {
        // TODO: grafo che e un singolo ciclo -> base di un elemento pari al ciclo.
        return EXIT_FAILURE;
    }

    // Un albero (aciclico) non ha cicli: la base deve essere vuota.
    int test_tree_graph_has_empty_basis() {
        // TODO: grafo aciclico -> de_pina restituisce una lista vuota.
        return EXIT_FAILURE;
    }

    // Verifica sul circuito d'esempio del progetto (Figura 3 / netlist di prova).
    int test_on_example_circuit() {
        // TODO: costruire il grafo del circuito d'esempio e confrontare le maglie
        //       trovate con quelle attese (es. C1 e C2 della Figura 3).
        return EXIT_FAILURE;
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

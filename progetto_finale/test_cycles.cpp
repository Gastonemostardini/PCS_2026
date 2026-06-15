#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <stdexcept>
#include "edge.h"
#include "cycles.h"

using namespace std;

// Scheletro dei test per la classe Cycles, che rappresenta un ciclo come
// vettore di incidenza (adjacency) sugli archi del grafo. Su questi vettori
// si definiscono il prodotto interno in GF(2) (operator*) e la differenza
// simmetrica / xor (operator^), usati dall'algoritmo di De Pina.
//
// NOTA: per costruire i Cycles di test potrebbe servire esporre un
//       costruttore o un setter di adjacency/real_edges nella classe Cycles.
//
// Tutti i metodi ritornano EXIT_FAILURE finche non sono implementati.
class CyclesTest
{
public:
    // all_edges() deve restituire solo gli archi marcati a true nell'adjacency.
    int test_all_edges_returns_marked_edges()
    {
        // TODO: costruire un Cycles con adjacency nota e verificare gli archi restituiti.
        return EXIT_FAILURE;
    }

    // Prodotto interno in GF(2): due cicli ortogonali danno 0 (false).
    int test_inner_product_orthogonal_is_zero()
    {
        // TODO: due cicli con numero PARI di archi condivisi -> operator* == false.
        return EXIT_FAILURE;
    }

    // Prodotto interno in GF(2): sovrapposizione dispari da 1 (true).
    int test_inner_product_non_orthogonal_is_one()
    {
        // TODO: due cicli con numero DISPARI di archi condivisi -> operator* == true.
        return EXIT_FAILURE;
    }

    // Il prodotto interno tra cicli di lunghezza diversa deve lanciare invalid_argument.
    int test_inner_product_size_mismatch_throws()
    {
        // TODO: adjacency di dimensioni diverse -> deve lanciare std::invalid_argument.
        return EXIT_FAILURE;
    }

    // xor (operator^): differenza simmetrica dei due vettori di incidenza.
    int test_xor_symmetric_difference()
    {
        // TODO: verificare che (c1 ^ c2) sia la differenza simmetrica attesa.
        return EXIT_FAILURE;
    }

    // Lo xor di un ciclo con se stesso deve dare il vettore nullo.
    int test_xor_with_self_is_zero()
    {
        // TODO: (c ^ c) deve risultare tutto false.
        return EXIT_FAILURE;
    }

    // Lo xor tra cicli di lunghezza diversa deve lanciare invalid_argument.
    int test_xor_size_mismatch_throws()
    {
        // TODO: adjacency di dimensioni diverse -> deve lanciare std::invalid_argument.
        return EXIT_FAILURE;
    }

    // L'operator<< deve stampare gli archi marcati nel formato { e1, e2, ... }.
    int test_output_format()
    {
        // TODO: stampare un Cycles in uno stringstream e confrontare con il formato atteso.
        return EXIT_FAILURE;
    }
};

int main()
{
    CyclesTest t;

    if (t.test_all_edges_returns_marked_edges() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_all_edges_returns_marked_edges" << endl;
    if (t.test_inner_product_orthogonal_is_zero() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_inner_product_orthogonal_is_zero" << endl;
    if (t.test_inner_product_non_orthogonal_is_one() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_inner_product_non_orthogonal_is_one" << endl;
    if (t.test_inner_product_size_mismatch_throws() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_inner_product_size_mismatch_throws" << endl;
    if (t.test_xor_symmetric_difference() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_xor_symmetric_difference" << endl;
    if (t.test_xor_with_self_is_zero() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_xor_with_self_is_zero" << endl;
    if (t.test_xor_size_mismatch_throws() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_xor_size_mismatch_throws" << endl;
    if (t.test_output_format() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_output_format" << endl;

    return EXIT_SUCCESS;
}

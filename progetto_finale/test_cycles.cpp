#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include "edge.h"
#include "cycles.h"

using namespace std;

// Test per la classe Cycles, che rappresenta un ciclo come vettore di
// incidenza (adjacency) sugli archi del grafo. Su questi vettori si
// definiscono il prodotto interno in GF(2) (operator*) e la differenza
// simmetrica / xor (operator^), usati dall'algoritmo di De Pina.
class CyclesTest
{
    // universo degli archi di riferimento, ordinato:
    // (1,2) (1,4) (2,3) (2,4) (3,4)
    static set<Edge<int>> universe()
    {
        return {{1, 2}, {1, 4}, {2, 3}, {2, 4}, {3, 4}};
    }

public:

    int test_all_edges_returns_marked_edges()
    {
        try
        {
            Cycles<int> c(universe(), {{1, 2}, {2, 4}});
            list<Edge<int>> edges = c.all_edges();
            list<Edge<int>> expected = {Edge<int>(1, 2), Edge<int>(2, 4)};
            if (edges != expected)
            {
                cerr << "  archi attesi (1->2, 2->4), ottenuti " << edges << endl;
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

    // Prodotto interno in GF(2): due cicli ortogonali danno 0 (false).
    int test_inner_product_orthogonal_is_zero()
    {
        try
        {
            Cycles<int> c1(universe(), {{1, 2}, {2, 4}});
            Cycles<int> c2(universe(), {{1, 4}, {3, 4}});
            // nessun arco in comune -> 0 (pari) -> false
            if ((c1 * c2) != false)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Prodotto interno in GF(2): sovrapposizione dispari da 1 (true).
    int test_inner_product_non_orthogonal_is_one()
    {
        try
        {
            Cycles<int> c1(universe(), {{1, 2}, {2, 4}});
            Cycles<int> c2(universe(), {{2, 4}, {3, 4}});
            // arco comune (2,4) -> 1 (dispari) -> true
            if ((c1 * c2) != true)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Il prodotto interno tra cicli di lunghezza diversa deve lanciare invalid_argument.
    int test_inner_product_size_mismatch_throws()
    {
        set<Edge<int>> smaller = {{1, 2}, {2, 3}, {3, 4}};
        Cycles<int> c1(universe()); // adjacency di 5 archi
        Cycles<int> c2(smaller);    // adjacency di 3 archi
        try
        {
            (void)(c1 * c2);
            return EXIT_FAILURE; // doveva lanciare
        }
        catch (const invalid_argument &)
        {
            return EXIT_SUCCESS;
        }
    }

    // xor (operator^): differenza simmetrica dei due vettori di incidenza.
    int test_xor_symmetric_difference()
    {
        try
        {
            Cycles<int> c1(universe(), {{1, 2}, {1, 4}, {2, 4}}); // triangolo 1-2-4
            Cycles<int> c2(universe(), {{2, 3}, {2, 4}, {3, 4}}); // triangolo 2-3-4
            Cycles<int> result = c1 ^ c2;
            // l'arco condiviso (2,4) si elimina; restano (1,2) (1,4) (2,3) (3,4)
            if (result.weight() != 4)
                return EXIT_FAILURE;
            if (!result.is_active(Edge<int>(1, 2)) || !result.is_active(Edge<int>(1, 4)) ||
                !result.is_active(Edge<int>(2, 3)) || !result.is_active(Edge<int>(3, 4)))
                return EXIT_FAILURE;
            if (result.is_active(Edge<int>(2, 4)))
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Lo xor di un ciclo con se stesso deve dare il vettore nullo.
    int test_xor_with_self_is_zero()
    {
        try
        {
            Cycles<int> c(universe(), {{1, 2}, {2, 4}});
            Cycles<int> result = c ^ c;
            if (result.weight() != 0)
                return EXIT_FAILURE;
            if (!result.all_edges().empty())
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Lo xor tra cicli di lunghezza diversa deve lanciare invalid_argument.
    int test_xor_size_mismatch_throws()
    {
        set<Edge<int>> smaller = {{1, 2}, {2, 3}, {3, 4}};
        Cycles<int> c1(universe()); // adjacency di 5 archi
        Cycles<int> c2(smaller);    // adjacency di 3 archi
        try
        {
            Cycles<int> r = c1 ^ c2;
            (void)r;
            return EXIT_FAILURE; // doveva lanciare
        }
        catch (const invalid_argument &)
        {
            return EXIT_SUCCESS;
        }
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
    
    return EXIT_SUCCESS;
}

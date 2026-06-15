#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include "functions.h"

using namespace std;

// Scheletro dei test per gli operatori su std::vector<bool> definiti in
// functions.h, che implementano l'algebra in GF(2) usata per i cicli:
//   operator*  -> prodotto interno mod 2 (1 se le posizioni a true comuni sono dispari)
//   operator^  -> xor / differenza simmetrica
//
// Tutti i metodi ritornano EXIT_FAILURE finche non sono implementati.
class FunctionsTest
{
public:
    // Numero PARI di posizioni true comuni -> prodotto interno false (0).
    int test_inner_product_even_overlap_is_false()
    {
        // TODO: due vettori con sovrapposizione pari -> (a * b) == false.
        return EXIT_FAILURE;
    }

    // Numero DISPARI di posizioni true comuni -> prodotto interno true (1).
    int test_inner_product_odd_overlap_is_true()
    {
        // TODO: due vettori con sovrapposizione dispari -> (a * b) == true.
        return EXIT_FAILURE;
    }

    // Il prodotto interno tra vettori di dimensioni diverse deve lanciare invalid_argument.
    int test_inner_product_size_mismatch_throws()
    {
        // TODO: a.size() != b.size() -> deve lanciare std::invalid_argument.
        return EXIT_FAILURE;
    }

    // xor elemento per elemento.
    int test_xor_basic()
    {
        // TODO: verificare (a ^ b) su un caso noto.
        return EXIT_FAILURE;
    }

    // xor di vettori disgiunti -> unione delle posizioni a true.
    int test_xor_disjoint()
    {
        // TODO: a e b senza true in comune -> (a ^ b) ha true in tutte le posizioni di a o b.
        return EXIT_FAILURE;
    }

    // Lo xor tra vettori di dimensioni diverse deve lanciare invalid_argument.
    int test_xor_size_mismatch_throws()
    {
        // TODO: a.size() != b.size() -> deve lanciare std::invalid_argument.
        return EXIT_FAILURE;
    }
};

int main()
{
    FunctionsTest t;

    if (t.test_inner_product_even_overlap_is_false() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_inner_product_even_overlap_is_false" << endl;
    if (t.test_inner_product_odd_overlap_is_true() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_inner_product_odd_overlap_is_true" << endl;
    if (t.test_inner_product_size_mismatch_throws() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_inner_product_size_mismatch_throws" << endl;
    if (t.test_xor_basic() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_xor_basic" << endl;
    if (t.test_xor_disjoint() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_xor_disjoint" << endl;
    if (t.test_xor_size_mismatch_throws() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_xor_size_mismatch_throws" << endl;

    return EXIT_SUCCESS;
}

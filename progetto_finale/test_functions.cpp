#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include "functions.h"

using namespace std;

// Test per gli operatori su std::vector<bool> definiti in functions.h, che
// implementano l'algebra in GF(2) usata per i cicli:
//   operator*  -> prodotto interno mod 2 (1 se le posizioni a true comuni sono dispari)
//   operator^  -> xor / differenza simmetrica
class FunctionsTest
{
public:
    // Numero PARI di posizioni true comuni -> prodotto interno false (0).
    int test_inner_product_even_overlap_is_false()
    {
        try
        {
            vector<bool> a = {true, true, false};
            vector<bool> b = {true, true, false};
            // posizioni a true comuni: 0 e 1 -> 2 (pari) -> 0
            if ((a * b) != false)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Numero DISPARI di posizioni true comuni -> prodotto interno true (1).
    int test_inner_product_odd_overlap_is_true()
    {
        try
        {
            vector<bool> a = {true, true, false};
            vector<bool> b = {true, false, false};
            // posizione a true comune: solo 0 -> 1 (dispari) -> 1
            if ((a * b) != true)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Il prodotto interno tra vettori di dimensioni diverse deve lanciare invalid_argument.
    int test_inner_product_size_mismatch_throws()
    {
        vector<bool> a = {true, false, true};
        vector<bool> b = {true, false};
        try
        {
            (void)(a * b);
            return EXIT_FAILURE; // doveva lanciare
        }
        catch (const invalid_argument &)
        {
            return EXIT_SUCCESS;
        }
    }

    // xor elemento per elemento.
    int test_xor_basic()
    {
        try
        {
            vector<bool> a = {true, false, true};
            vector<bool> b = {true, true, false};
            vector<bool> expected = {false, true, true};
            if ((a ^ b) != expected)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // xor di vettori disgiunti -> unione delle posizioni a true.
    int test_xor_disjoint()
    {
        try
        {
            vector<bool> a = {true, false, false};
            vector<bool> b = {false, true, false};
            vector<bool> expected = {true, true, false};
            if ((a ^ b) != expected)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Lo xor tra vettori di dimensioni diverse deve lanciare invalid_argument.
    int test_xor_size_mismatch_throws()
    {
        vector<bool> a = {true, false, true};
        vector<bool> b = {true, false};
        try
        {
            (void)(a ^ b);
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

#include <iostream>
#include <vector>
#include <algorithm>
#include "sorts.hpp"
#include "randfiller.h"

int test_quicksort() {
    randfiller rf;
    std::vector<int> v;
    int size = 1;

    // Giro 100 volte
    for (int i = 0; i < 100; i++) {
        size = rand() % 1000 + 2;
        v.resize(size); 
        rf.fill(v, -100, 100); // riempi il vettore casualmente 
        quicksort(v); // Chiami la funzione
        if (!std::is_sorted(v.begin(), v.end())) // SE NON è sortato
            return EXIT_FAILURE; // ALLORA ritorna il fallimento del test
    }

    // Test su un vettore di stringhe 
    std::vector<std::string> vs = { "G", "a", "s", "t", "o", "n", "e", "gastone", "Gastone", "PCS", "pcs", "2", "2e32", "0x1234" };
    quicksort(vs, 0, 13);
    if (!std::is_sorted(vs.begin(), vs.end()))
        return EXIT_FAILURE; // ritorna il fallimento del test in caso di NON sort 

    return EXIT_SUCCESS; // Se arriva qui ritorni successo perché è andato tutto bene
}

int main() {
    return test_quicksort();
}

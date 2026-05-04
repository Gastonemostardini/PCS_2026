#include <iostream>
#include <vector>
#include <algorithm>
#include "sorts.hpp"
#include "randfiller.h"

int test_mergesort() {
    randfiller rf;
    std::vector<int> v;
    int size = 1;

    for (int i = 0; i < 100; i++) {
        size = rand() % 1000 + 2;
        v.resize(size);
        rf.fill(v, -100, 100);
        mergesort(v);
        if (!std::is_sorted(v.begin(), v.end()))
            return EXIT_FAILURE;
    }

    std::vector<std::string> vs = { "G", "a", "s", "t", "o", "n", "e", "gastone", "Gastone", "PCS", "pcs", "2", "2e32", "0x1234" };
    mergesort(vs, 0, 13);
    if (!std::is_sorted(vs.begin(), vs.end()))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int main() {
    return test_mergesort();
}

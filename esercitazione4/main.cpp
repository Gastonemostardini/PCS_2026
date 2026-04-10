#include <iostream>
#include <vector>
#include "sorts.hpp"
#include "randfiller.h"

using namespace std;


// Overload per print optional
template<typename T>
ostream& operator<<(ostream& os, const optional<T>& obj) {
	if (obj.has_value())
		os << obj.value();
	else
		os << "vettore vuoto";
	return os;
}


int main() {
	randfiller rf;

	vector<int> ciao = { 1, 3, 2, 4, 7, -1 };
	vector<string> ciao2 = { "gastone", "brontolo" };
	vector<string> vs = { "G", "a", "s", "t", "o", "n", "e", "gastone", "Gastone", "PCS", "pcs", "2", "2e32", "0x1234" };

	cout << vs;
	bubble_sort(ciao);
	cout << ciao;

	cout << endl;
	return 0;
}

#include <iostream>
#include "rational.cpp"


int main() {
	Rational<int> a(2, -3);
	Rational<int> b(-6, 3);
	Rational<int> c(-7, -3);
	Rational<int> d(72, 3);
	d += c + b;
	cout << a << " " << b << " " << c << " " << d << endl;
	return 0;
}

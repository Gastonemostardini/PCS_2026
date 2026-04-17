#include <iostream>
#include <cmath>

using namespace std;

static const int N = 5;

/* Overload della funzione min */
double min(double x[N]) {
	double minimo = x[0];
	for (int i = 0; i < N; i++) {
		minimo = min(x[i], minimo);
	}
	return minimo;
}

/* Overload della funzione max */
double max(double x[N]) {
	double massimo = x[0];
	for (int i = 0; i < N; i++) {
		massimo = max(x[i], massimo);
	}
	return massimo;
}

double sum(double x[N]) {
	double somma = 0;
	for (int i = 0; i < N; i++) {
		somma += x[i];
	}
	return somma;
}

double avg(double x[N]) {
	return sum(x) / N;
}

double std_func(double x[N]) {
	double S = 0;
	double media = avg(x);
	for (int i = 0; i < N; i++) {
		S += (x[i] - media) * (x[i] - media);
	}
	return sqrt(S / N);
}

void print(double x[N]) {
	cout << "[";
	for (int i = 0; i < N; i++) {
		cout << x[i];
		if (i < N-1) {
			cout << ", ";
		}
	}
	cout << "]\n";
}

double* bubble_sort(double x[N]) {
	bool change = true;
	float x_temp = 0;
	while (change) {
		int i = 0;
		change = false;
		while(i < N-1) {
			if (x[i] > x[i + 1]) {
				x_temp = x[i + 1];
				x[i + 1] = x[i];
				x[i] = x_temp;
				change = true;
			}
			i++;
		}
	}
	return x;
}

int main()
{
	double x[N] = {-10, 12.4, 2.9, -15.3, 1.0};
	cout << "min: \t" << min(x)		<< "\n";
	cout << "max: \t" << max(x)		<< "\n";
	cout << "avg: \t" << avg(x)		<< "\n";
	cout << "std: \t" << std_func(x) << "\n";
	cout << "sort:\t"; print(bubble_sort(x));
	cout << "\n";

}

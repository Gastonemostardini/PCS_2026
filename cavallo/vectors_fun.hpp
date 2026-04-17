#pragma once

#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	os << "[";
	for (std::size_t i = 0; i < v.size(); i++) {
		os << v[i];
		if (i + 1 < v.size()) {
			os << ", ";
		}
	}
	os << "]";
	return os;
}

int pos(int N, int r, int c) {
	return r * N + c;
}

std::vector<std::vector<int>> horseMovableCells(int N, int r, int c) {
	std::vector<std::vector<int>> celle = {
		{r + 2, c + 1 }, {r + 2, c - 1 },
		{r + 1, c + 2 }, {r - 1, c + 2 },
		{r - 2, c + 1 }, {r - 2, c - 1},
		{r + 1, c - 2 }, {r - 1, c - 2 },
	};

	for (int i = 7; i >= 0; i--){
		if (celle[i][0] < 0 || celle[i][0] >= N ||
			celle[i][1] < 0 || celle[i][1] >= N) {
			celle.erase(celle.begin() + i);
		}
	}

	return celle;
}

void initializeArchi(std::vector<int>& v, int N) {
	std::fill(v.begin(), v.end(), 8);
	for (int i = 2; i < N - 2; i++) {
		v[pos(N, 0, i)] = 4;		// riga alto
		v[pos(N, N - 1, i)] = 4;	// riga basso
		v[pos(N, i, 0)] = 4;		// colonna sinistra
		v[pos(N, i, N - 1)] = 4;	// colonna destra
	}

	// Seconda fascia (una casella dentro rispetto al bordo)
	for (int i = 2; i < N - 2; i++) {
		v[pos(N, 1, i)] = 6;
		v[pos(N, N - 2, i)] = 6;
		v[pos(N, i, 1)] = 6;
		v[pos(N, i, N - 2)] = 6;
	}

	// Angoli: 2 mosse
	v[pos(N, 0, 0)] = 2;
	v[pos(N, 0, N - 1)] = 2;
	v[pos(N, N - 1, 0)] = 2;
	v[pos(N, N - 1, N - 1)] = 2;

	// Adiacenti agli angoli lungo i bordi: 3 mosse
	v[pos(N, 0, 1)] = 3;
	v[pos(N, 1, 0)] = 3;
	v[pos(N, 0, N - 2)] = 3;
	v[pos(N, 1, N - 1)] = 3;
	v[pos(N, N - 2, 0)] = 3;
	v[pos(N, N - 1, 1)] = 3;
	v[pos(N, N - 2, N - 1)] = 3;
	v[pos(N, N - 1, N - 2)] = 3;

	// Diagonali interne agli angoli: 4 mosse
	v[pos(N, 1, 1)] = 4;
	v[pos(N, 1, N - 2)] = 4;
	v[pos(N, N - 2, 1)] = 4;
	v[pos(N, N - 2, N - 2)] = 4;
}

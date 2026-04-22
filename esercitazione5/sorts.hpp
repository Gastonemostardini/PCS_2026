#pragma once
#include <optional>
#include <algorithm>
#include <cmath>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& obj) {
	if (obj.size() == 0) {
		os << "vettore vuoto";
	}
	os << "[";
	for (std::size_t i = 0; i < obj.size(); i++) {
		os << obj[i];
		if (i < obj.size() - 1)
			os << ", ";
	}
	os << "]";
	return os;
}

template<typename T>
void fusion(std::vector<T>& vec, int sx, int center, int dx) {
	std::vector<T> temp;
	int i = sx, j = center + 1;

	while (i <= center && j <= dx) {
		if (vec[i] <= vec[j])
			temp.push_back(vec[i++]);
		else
			temp.push_back(vec[j++]);
	}

	while (i <= center) temp.push_back(vec[i++]);
	while (j <= dx)     temp.push_back(vec[j++]);

	for (int k = 0; k < static_cast<int>(temp.size()); k++)
		vec[sx + k] = temp[k];
}

template<typename T>
void mergesort(std::vector<T>& vec, int sx, int dx) {
	if (sx < dx) {
		int center = std::floor((sx + dx) / 2);
		mergesort(vec, sx, center);
		mergesort(vec, center + 1, dx);
		fusion(vec, sx, center, dx);
	}
}

template<typename T>
void mergesort(std::vector<T>& vec) {
	int sx = 0;
	int dx = vec.size();
	return mergesort(vec, sx, dx);
}


template<typename T>
int partition(std::vector<T>& vec, int p, int r) {
	T x = vec[r];
	size_t i = p - 1;

	for (auto j = p; j < r; j++) {
		if (vec[j] <= x) {
			i++;
			std::swap(vec[i], vec[j]);
		}
	}

	std::swap(vec[i + 1], vec[r]);
	return ++i;
}

template<typename T>
void quicksort(std::vector<T>& vec, int p, int r) {
	if (p < r){
		int q = partition(vec, p, r);
		quicksort(vec, p, q - 1);
		quicksort(vec, q, r);
	}
}

template<typename T>
void quicksort(std::vector<T>& vec) {
	int p = 0;
	int r = vec.size();
	quicksort(vec, p, r);
}

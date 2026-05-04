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

	for (size_t k = 0; k < temp.size(); k++)
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
	int dx = vec.size() - 1;
	return mergesort(vec, sx, dx);
}


template<typename T>
int partition(std::vector<T>& vec, int p, int r) {
	T x = vec[r];
	int i = p - 1;

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
void insertion_sort(std::vector<T>& vec, int p, int r)
{
	if (p >= r) return;

	T key;
	int i = 0;
	for (int j = p + 1; j <= r; j++)
	{
		key = vec[j];
		i = j - 1;
		while (i >= p && vec[i] > key)
		{
			vec[i + 1] = vec[i];
			i--;
		}
		vec[i + 1] = key;
	}
}

template<typename T>
void insertion_sort(std::vector<T>& vec)
{
	if (vec.size() == 0) {
		return;
	}

	T key;
	int i = 0;
	for (std::size_t j = 1; j < vec.size(); j++)
	{
		key = vec[j];
		i = j - 1;
		while (i >= 0 && vec[i] > key)
		{
			vec[i + 1] = vec[i];
			i--;
		}
		vec[i + 1] = key;
	}
}

template<typename T>
void quicksort(std::vector<T>& vec, int p, int r) {
	if (p < r) {
		if (r - p + 1 < 175) {
			insertion_sort(vec, p, r);
			return;
		}

		int q = partition(vec, p, r);
		quicksort(vec, p, q - 1);
		quicksort(vec, q + 1, r);
	}
}


template<typename T>
void quicksort(std::vector<T>& vec) {
	int p = 0;
	int r = vec.size() - 1;
	quicksort(vec, p, r);
}

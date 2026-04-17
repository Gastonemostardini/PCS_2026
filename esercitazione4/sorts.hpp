#pragma once
#include <optional>
#include <algorithm>
#include <cmath>

using namespace std;

template<typename T>
std::optional<T> max_elem(const std::vector<T>& vec)
{
	if (vec.size() == 0) {
		return {};
	}
	T vec_max = vec[0];

	for (int i = 1; i < vec.size(); i++) {
		vec_max = std::max(vec_max, vec[i]);
	}

	return vec_max;
}
template<typename T>
ostream& operator<<(ostream& os, const vector<T>& obj) {
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
void bubble_sort(std::vector<T>& vec)
{
	if (vec.size() == 0) {
		return;
	}

	for (std::size_t i = 0; i < vec.size() - 1; i++)
	{
		for (std::size_t j = vec.size() - 1; j > i; j--) {
			if (vec[j] < vec[j - 1])
			{
				std::swap(vec[j], vec[j - 1]);
			}
		}
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
void selection_sort(std::vector<T>& vec)
{
	if (vec.size() == 0) {
		return;
	}

	std::size_t min;
	for (std::size_t i = 0; i < vec.size() - 1; i++)
	{
		min = i;
		for (auto j = i + 1; j < vec.size(); j++) {
			if (vec[j] < vec[min])
			{
				min = j;
			}
		}
		std::swap(vec[i], vec[min]);
	}
}

template<typename T>
void fusion(std::vector<T>& vec, int sx, int center, int dx){
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
void mergesort(std::vector<T>& vec, int sx, int dx){
	if (sx < dx){
		int center = std::floor((sx + dx)/2);
	    mergesort(vec, sx, center);
		mergesort(vec, center + 1, dx);
		fusion(vec, sx, center, dx);	
	}
}


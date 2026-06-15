#pragma once
#include "edge.h"

template <typename T>
	requires std::totally_ordered<T>
class WeighedEdge : public Edge<T>
{
	int weight_;

	friend std::ostream &operator<<(std::ostream &os, const WeighedEdge<T> &obj)
	{
		os << obj.from() << "->" << obj.to() << " PESO: " << obj.weight_ << "; ";
		return os;
	}

public:
	WeighedEdge(T from, T to, int weight = 0) : Edge<T>(from, to), weight_(weight) {}

	int getWeight() const
	{
		return weight_;
	}
};

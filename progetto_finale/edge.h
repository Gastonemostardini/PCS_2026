#pragma once
#include <ostream>
#include "functions.h"

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
class Edge
{
	T from_;
	T to_;
	friend std::ostream &operator<<(std::ostream &os, const Edge<T> &obj)
	{
		os << obj.from_ << "->" << obj.to_;
		return os;
	}

	void normalize();

public:
	Edge(T from, T to);

	T from() const;
	T to() const;
	bool has(T value) const;

	bool operator<(Edge<T> const &other) const;
	bool operator==(Edge<T> const &other) const;
};

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
T Edge<T>::from() const
{
	return from_;
}

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
T Edge<T>::to() const
{
	return to_;
}

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
bool Edge<T>::has(T value) const
{
	return (from_ == value || to_ == value);
}

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
Edge<T>::Edge(T from, T to)
{
	from_ = from;
	to_ = to;
	normalize();
}

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
void Edge<T>::normalize()
{
	if (from_ > to_)
	{
		T temp_ = from_;
		from_ = to_;
		to_ = temp_;
	}
}

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
bool Edge<T>::operator<(Edge<T> const &other) const
{
	if (from_ < other.from_)
		return true;
	if (other.from_ < from_)
		return false;

	if (to_ < other.to_)
		return true;
	if (other.to_ < to_)
		return false;

	return false;
}

template <typename T>
	requires std::totally_ordered<T> && Printable<T>
bool Edge<T>::operator==(Edge<T> const &other) const
{
	return (from_ == other.from_ && other.to_ == to_);
}

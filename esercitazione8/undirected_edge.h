#pragma once
#include <ostream>

template<typename T> requires std::totally_ordered<T>
class UndirectedEdge
{
	T from_;
	T to_;
	friend std::ostream& operator<<(std::ostream& os, const UndirectedEdge<T>& obj) {
		os << obj.from_ << "->" << obj.to_;
		return os;
	}

	void normalize();

public:
	UndirectedEdge(T from, T to);

	T from();
	T to();
	bool has(T value) const;

	bool operator<(UndirectedEdge<T> const& other) const;
	bool operator==(UndirectedEdge<T> const& other) const;
};

template<typename T> requires std::totally_ordered<T>
T UndirectedEdge<T>::from() {
	return from_;
}

template<typename T> requires std::totally_ordered<T>
T UndirectedEdge<T>::to() {
	return to_;
}

template<typename T> requires std::totally_ordered<T>
bool UndirectedEdge<T>::has(T value) const {
	return (from_ == value || to_ == value);
}

template<typename T> requires std::totally_ordered<T>
UndirectedEdge<T>::UndirectedEdge(T from, T to) {
	from_ = from;
	to_ = to;
	normalize();
}

template<typename T> requires std::totally_ordered<T>
void UndirectedEdge<T>::normalize() {
	if (from_ > to_) {
		T temp_ = from_;
		from_ = to_;
		to_ = temp_;
	}
}

template<typename T> requires std::totally_ordered<T>
bool UndirectedEdge<T>::operator<(UndirectedEdge<T> const& other) const {
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

template<typename T> requires std::totally_ordered<T>
bool UndirectedEdge<T>::operator==(UndirectedEdge<T> const& other) const {
	return (from_ == other.from_ && other.to_ == to_);
}

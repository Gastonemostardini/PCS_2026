#pragma once
#include <vector>
#include <set>
#include <list>

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
std::ostream& operator<<(std::ostream& os, const std::list<T>& obj) {
    if (obj.empty()) {
        os << "vettore vuoto";
        return os;
    }
    os << "(";
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        if (it != obj.begin())
            os << ", ";
        os << *it;
    }
    os << ")";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T> obj) {
    for (auto elm : obj) {
        os << elm << " ";
    }
    return os;
}

bool operator*(std::vector<bool> a, std::vector<bool> b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    int sum = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] && b[i])
            sum++;
    }

    return sum % 2;

}

std::vector<bool>& operator+=(std::vector<bool>& a, const std::vector<bool>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    for (size_t i = 0; i < a.size(); i++) {
        a[i] = (a[i] + b[i]) % 2;
    }

    return a;
}

std::vector<bool> operator^(std::vector<bool> a, std::vector<bool> b) {
	if (a.size() != b.size()) {
		throw std::invalid_argument("Vector MUST be the same size.");
	}

	std::vector<bool> res(a.size());
	for (size_t i = 0; i < a.size(); i++){
		res[i] = a[i] ^ b[i];
	}
	return res;
}

template<typename T>
struct Signed {
    T value;
    bool positive;

    auto operator<=>(const Signed&) const = default;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Signed<T>& obj) {
    os << "(" << obj.value << (obj.positive ? "+" : "-") << ")";
    return os;
}

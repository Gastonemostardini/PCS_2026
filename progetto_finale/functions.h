#pragma once
#include <vector>
#include <set>
#include <list>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <istream>
#include <ostream>
#include <sstream>
#include <concepts>
#include <string>
#include <utility>

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &obj)
{
    if (obj.size() == 0)
    {
        os << "vettore vuoto";
    }

    os << "[";
    for (std::size_t i = 0; i < obj.size(); i++)
    {
        os << obj[i];
        if (i < obj.size() - 1)
            os << ", ";
    }
    os << "]";

    return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::list<T> &obj)
{
    if (obj.empty())
    {
        os << "vettore vuoto";
        return os;
    }

    os << "(";
    for (auto it = obj.begin(); it != obj.end(); ++it)
    {
        if (it != obj.begin())
            os << ", ";
        os << *it;
    }
    os << ")";
    return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::set<T> obj)
{
    for (auto elm : obj)
    {
        os << elm << " ";
    }
    return os;
}

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &p)
{
    os << "(" << p.first << "," << p.second << ")";
    return os;
}

// Parsing di una std::pair nel formato "(first,second)", speculare a operator<<.
// Esempio:  (1,A)  ->  p.first = 1, p.second = "A"
template <typename T1, typename T2>
std::istream &operator>>(std::istream &is, std::pair<T1, T2> &p)
{
    char c;

    // salta gli spazi e si aspetta la parentesi di apertura '('
    if (!(is >> c))
        return is; // EOF o errore: lascia il failbit per fermare il while del parser
    if (c != '(')
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    // legge il primo elemento (es. int)
    is >> p.first;

    // si aspetta il separatore ','
    if (!(is >> c) || c != ',')
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    // legge il secondo elemento: tutto ciò che resta fino alla ')'
    std::string token;
    std::getline(is, token, ')');
    std::istringstream ss(token);
    ss >> p.second;

    return is;
}

template <typename T>
concept  Streamable = requires(std::ostream &os, std::istream &is, T &value) {
    { os << value } -> std::same_as<std::ostream &>;
    { is >> value } -> std::same_as<std::istream &>;
};

template <typename T>
concept Printable = requires(std::ostream &os, std::istream &is, T &value) {
    { os << value } -> std::same_as<std::ostream &>;
};

bool operator*(std::vector<bool> a, std::vector<bool> b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    int sum = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i] && b[i])
            sum++;
    }

    return sum % 2;
}

std::vector<bool> &operator+=(std::vector<bool> &a, const std::vector<bool> &b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    for (size_t i = 0; i < a.size(); i++)
    {
        a[i] = (a[i] + b[i]) % 2;
    }

    return a;
}

std::vector<bool> operator^(std::vector<bool> a, std::vector<bool> b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Vector MUST be the same size.");
    }

    std::vector<bool> res(a.size());
    for (size_t i = 0; i < a.size(); i++)
    {
        res[i] = a[i] ^ b[i];
    }
    return res;
}

template <typename T>
struct Signed
{
    T value;
    bool positive;

    auto operator<=>(const Signed &) const = default;
};

template <typename T>
    requires Printable<T>
std::ostream &operator<<(std::ostream &os, const Signed<T> &obj)
{
    os << "(" << obj.value << (obj.positive ? "+" : "-") << ")";
    return os;
}

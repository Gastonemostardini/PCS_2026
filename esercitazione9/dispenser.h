#pragma once
#include <ostream>
#include <utility>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << "(" << p.first << "," << p.second << ")";
    return os;
}

template <typename T>
class Dispenser {

public:
    virtual ~Dispenser() = default;

    // Inserimento
    virtual void put(const T& item) = 0;
    virtual void put(T&& item) = 0;       // overload per move

    // Estrazione (rimuove e restituisce)
    virtual T get() = 0;

    // Ispezione del prossimo elemento (senza rimuoverlo)
    virtual const T& peek() const = 0;

    // Stato
    virtual bool empty() const noexcept = 0;
    virtual std::size_t size() const noexcept = 0;
    virtual bool contains(T target) const noexcept = 0;

    virtual void print(std::ostream& os) const = 0;  // virtuale


    friend std::ostream& operator<<(std::ostream& os, const Dispenser<T>& obj) {
        obj.print(os);
        return os;
    }
};
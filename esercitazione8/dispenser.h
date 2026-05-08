#pragma once

template <typename T>
class Dispenser {
public:
    virtual ~Dispenser() = default;

    // Inserimento
    virtual void insert(const T& item) = 0;
    virtual void insert(T&& item) = 0;       // overload per move

    // Estrazione (rimuove e restituisce)
    virtual T remove() = 0;

    // Ispezione del prossimo elemento (senza rimuoverlo)
    virtual const T& peek() const = 0;

    // Stato
    virtual bool empty() const noexcept = 0;
    virtual std::size_t size() const noexcept = 0;
};
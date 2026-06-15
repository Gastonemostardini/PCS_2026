#pragma once
#include <queue>
#include <stdexcept>
#include <utility>
#include "dispenser.h"

template <typename T>
class Fifo : public Dispenser<T>
{
    std::queue<T> data_;

public:
    void put(const T &item) override
    {
        data_.push(item);
    }

    void put(T &&item) override
    {
        data_.push(std::move(item));
    }

    T get() override
    {
        if (data_.empty())
            throw std::out_of_range("Queue::remove on empty queue");
        T tmp = std::move(data_.front());
        data_.pop();
        return tmp;
    }

    const T &peek() const override
    {
        if (data_.empty())
            throw std::out_of_range("Queue::peek on empty queue");
        return data_.front();
    }

    bool empty() const noexcept override
    {
        return data_.empty();
    }

    std::size_t size() const noexcept override
    {
        return data_.size();
    }

    bool contains(T target) const noexcept override
    {
        std::queue<T> copy = data_;

        while (!copy.empty())
        {
            if (copy.front() == target)
                return true;
            copy.pop();
        }

        return false;
    }

    void print(std::ostream &os) const override
    {
        std::queue<T> copy = data_;
        os << "USCITA<-";
        while (!copy.empty())
        {
            os << copy.front();
            copy.pop();
            if (!copy.empty())
                os << "<-";
        }
        os << "<-ingresso";
    }
};
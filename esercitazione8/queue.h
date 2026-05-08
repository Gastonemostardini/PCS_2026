#pragma once
#include <queue>
#include <stdexcept>
#include <utility>
#include "dispenser.h"

template <typename T>
class Queue : public Dispenser<T> {
    std::queue<T> data_;

public:
    void insert(const T& item) override {
        data_.push(item);
    }

    void insert(T&& item) override {
        data_.push(std::move(item));
    }

    T remove() override {
        if (data_.empty())
            throw std::out_of_range("Queue::remove on empty queue");
        T tmp = std::move(data_.front());
        data_.pop();
        return tmp;
    }

    const T& peek() const override {
        if (data_.empty())
            throw std::out_of_range("Queue::peek on empty queue");
        return data_.front();
    }

    bool empty() const noexcept override {
        return data_.empty();
    }

    std::size_t size() const noexcept override {
        return data_.size();
    }
};
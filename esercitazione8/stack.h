#pragma once
#include <stack>
#include "dispenser.h"

template <typename T>
class Stack : public Dispenser<T> {
    std::stack<T> data_;

public:
    void insert(const T& item) override {
        data_.push(item);
    }

    void insert(T&& item) override {
        data_.push(std::move(item));
    }

    T remove() override {
        if (data_.empty())
            throw std::out_of_range("Stack::remove on empty stack");
        T tmp = std::move(data_.top());
        data_.pop();
        return tmp;
    }

    const T& peek() const override {
        if (data_.empty())
            throw std::out_of_range("Stack::peek on empty stack");
        return data_.top();
    }

    bool empty() const noexcept override {
        return data_.empty();
    }

    std::size_t size() const noexcept override {
        return data_.size();
    }
};
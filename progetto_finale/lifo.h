#pragma once
#include <stack>
#include "dispenser.h"

template <typename T>
class Lifo : public Dispenser<T> {
    std::stack<T> data_;

public:
    void put(const T& item) override {
        data_.push(item);
    }

    void put(T&& item) override {
        data_.push(std::move(item));
    }

    T get() override {
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

    bool contains(T target) const noexcept override {
        std::stack<T> copy = data_;

        while (!copy.empty()) {
            if (copy.top() == target)
                return true;
            copy.pop();
        }

        return false;
    }

    void print(std::ostream& os) const override {
        std::stack<T> copy = data_; 
        os << "Coperchio->[";
        while (!copy.empty()) {
            os << copy.top();
            copy.pop();
            if (!copy.empty()) os << ", ";
        }
        os << "]";
    }
};
#ifndef TF_STACK_H
#define TF_STACK_H

#include "tf_vector.hpp"

namespace tf {

/*
* Stack: Wrapper for tf::vector
*/
template <typename T>
class stack {
private:
    vector<T> vec;

public:
    // constructor
    stack(const size_t initial_capacity = 10):
        vec(vector<T>(initial_capacity)) {}

    // copy constructor
    stack(const stack &other):
        vec(other.vec) {}

    friend void swap(stack &first, stack &second) noexcept {
        using std::swap;
        swap(first.vec, second.vec);
    }

    // copy assignment operator
    stack &operator=(stack other) {
        swap(*this, other);
        return *this;
    }

    // move constructor
    stack(stack &&other) noexcept : stack(1) {
        swap(*this, other);
    }

    // move assignment operator
    stack &operator=(stack &&other) {
        swap(*this, other);
        return *this;
    }

    // O(1) / O(n) if capacity is full
    void put(const T &value) {
        vec.add(value);
    }

    // O(1)
    T &peek() {
        if (empty())
            throw exception("stack: peek: stack is empty");

        return vec[vec.size() - 1];
    }

    // O(1)
    const T &peek() const {
        if (empty())
            throw exception("stack: peek: stack is empty");
        
        return vec[vec.size() - 1];
    }

    // O(1)
    T pop() {
        if (empty())
            throw exception("stack: pop: stack is empty");

        return vec.remove(vec.size() - 1);
    }

    // O(1)
    size_t size() const {
        return vec.size();
    }

    // O(1)
    bool empty() const {
        return vec.empty();
    }

    // O(1)
    void clear() {
        vec.clear();
    }
};

}

#endif
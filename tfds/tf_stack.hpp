#ifndef TF_STACK_H
#define TF_STACK_H

#include <algorithm> // std::copy_n, std::swap
#include "tf_exception.hpp"

namespace tf {

/*
* Stack.
*/
template <typename T>
class stack {
private:
    size_t capacity;
    size_t top_index;
    T *buffer;

public:
    // constructor
    stack(const size_t initial_capacity = 10):
        capacity((initial_capacity > 0) ? initial_capacity : 1),
        top_index(0),
        buffer(new T[capacity]) {}
    
    // copy constructor
    stack(const stack &other):
        capacity(other.capacity),
        top_index(other.top_index),
        buffer(new T[capacity])
    {
        std::copy_n(other.buffer, capacity, buffer);
    }

    // destructor
    ~stack() {
        delete[] buffer;
    }

    friend void swap(stack &first, stack &second) noexcept {
        using std::swap;
        swap(first.capacity, second.capacity);
        swap(first.top_index, second.top_index);
        swap(first.buffer, second.buffer);
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
        if (top_index >= capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity <= top_index)
                throw exception("stack: put: capacity too large");

            T *new_buffer = new T[new_capacity];
            std::copy_n(buffer, capacity, new_buffer);
            capacity = new_capacity;
            delete[] buffer;
            buffer = new_buffer;
        }
        
        buffer[top_index++] = value;
    }

    // O(1)
    T &peek() const {
        if (empty())
            throw exception("stack: peek: stack is empty");
        
        return buffer[top_index - 1];
    }

    // O(1)
    T pop() {
        if (empty())
            throw exception("stack: pop: stack is empty");

        return buffer[--top_index];
    }

    // O(1)
    size_t size() const {
        return top_index;
    }

    // O(1)
    size_t current_capacity() const {
        return capacity;
    }

    // O(1)
    bool empty() const {
        return top_index == 0;
    }

    // O(1)
    void clear() {
        top_index = 0;
    }
};

}

#endif
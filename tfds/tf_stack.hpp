#ifndef TF_STACK_H
#define TF_STACK_H

#include <algorithm> // std::copy_n
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
    stack(const size_t initial_capacity = 10):
        capacity(initial_capacity),
        top_index(0),
        buffer(new T[capacity]) {}

    ~stack() {
        delete[] buffer;
    }

    // O(1) / O(n) if capacity is full
    void put(const T &value) {
        if (top_index >= capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity < capacity)
                throw tf::exception("stack: put: capacity too large");

            T *new_buffer = new T[new_capacity];
            std::copy_n(buffer, capacity, new_buffer);
            capacity = new_capacity;
            delete[] buffer;
            buffer = new_buffer;
        }
        
        buffer[top_index++] = value;
    }

    // O(1)
    T &peek() {
        if (empty())
            throw tf::exception("stack: peek: stack is empty");
        
        return buffer[top_index - 1];
    }

    // O(1)
    T pop() {
        if (empty())
            throw tf::exception("stack: pop: stack is empty");

        return buffer[--top_index];
    }

    // O(1)
    size_t size() const {
        return top_index;
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
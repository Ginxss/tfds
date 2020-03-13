//////////
// DONE //
//////////

#ifndef TF_STACK_H
#define TF_STACK_H

#include "tf_exception.hpp"

namespace tf {

/*
* Standard Stack structure (Put, Peek and Pop).
* Implemented as a dynamic array, which makes it significantly faster, but less memory efficient.
*/
template <typename T>
class stack {
private:
    int capacity;
    int top_index;
    T *buffer;

public:
    stack(const int initial_capacity = 10):
        capacity(initial_capacity),
        top_index(-1),
        buffer((T *)malloc(capacity * sizeof(T))) {}

    ~stack() {
        free(buffer);
    }

    // O(1)
    void put(const T &value) {
        if (++top_index >= capacity) {
            capacity *= 2;
            buffer = (T *)realloc(buffer, capacity * sizeof(T));
        }
        
        buffer[top_index] = value;
    }

    // O(1)
    T &peek() {
        if (empty())
            throw tf::exception("stack: peek: stack is empty");
        
        return buffer[top_index];
    }

    // O(1)
    T pop() {
        if (empty())
            throw tf::exception("stack: pop: stack is empty");

        return buffer[top_index--];
    }

    int size() const {
        return top_index + 1;
    }

    // O(1)
    bool empty() const {
        return top_index < 0;
    }
};

}

#endif
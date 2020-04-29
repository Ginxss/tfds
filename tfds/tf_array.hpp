#ifndef TF_ARRAY_H
#define TF_ARRAY_H

#include <cstdlib> // malloc
#include <cstring> // memset
#include "tf_exception.hpp"

namespace tf {

/*
* Array that by default reallocates when accessed out of bounds.
*/
template <typename T>
class array {
private:
    // VARIABLES

    int capacity_;
    bool autom_realloc;
    T *buffer;

    // METHODS

    // reallocation size is the smallest multiple of the current capacity that can hold the index.
    void check_index(const int index) {
        if (index >= capacity_) {
            if (autom_realloc) {
                capacity_ *= (int)((index / capacity_) + 1);
                buffer = (T *)realloc(buffer, capacity_ * sizeof(T));
            }
            else {
                throw tf::exception("array: index larger than capacity");
            }
        }
        else if (index < 0) {
            throw tf::exception("array: index negative");
        }
    }

public:
    array(const int initial_capacity = 10, const bool autom_realloc = true):
        capacity_(initial_capacity),
        autom_realloc(autom_realloc),
        buffer((T *)malloc(capacity_ * sizeof(T))) {}

    ~array() {
        free(buffer);
    }

    // O(1) / O(n) if index > capacity
    void insert(const int index, const T &value) {
        check_index(index);
        buffer[index] = value;
    }

    // O(1) / O(n) if index > capacity
    const T &get(const int index) {
        check_index(index);
        return buffer[index];
    }

    // O(1) / O(n) if index > capacity
    T &operator[](const int index) {
        check_index(index);
        return buffer[index];
    }

    // O(1)
    int capacity() const {
        return capacity_;
    }

    // O(1)
    bool autom_reallocating() const {
        return autom_realloc;
    }

    // O(n)
    void reallocate(const int new_capacity) {
        capacity_ = new_capacity;
        buffer = (T *)realloc(buffer, capacity_ * sizeof(T));
    }

    // O(n)
    void clear() {
        memset(buffer, 0, capacity_ * sizeof(T));
    }
};

}

#endif
#ifndef TF_ARRAY_H
#define TF_ARRAY_H

#include <algorithm> // std::copy_n
#include "tf_exception.hpp"

namespace tf {

/*
* Array that by default reallocates when accessed out of bounds.
*/
template <typename T>
class array {
private:
    // VARIABLES

    size_t capacity_;
    bool autom_realloc;
    T *buffer;

    // METHODS

    // reallocation size is the smallest multiple of the current capacity that can hold the index.
    void check_index(const size_t index) {
        if (index >= capacity_) {
            if (autom_realloc) {
                size_t new_capacity = capacity_ * ((size_t)(index / capacity_) + 1);
                if (new_capacity < capacity_)
                    throw tf::exception("array: capacity too large");
                
                reallocate(new_capacity);
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
    array(const size_t initial_capacity = 10, const bool autom_realloc = true):
        capacity_(initial_capacity),
        autom_realloc(autom_realloc),
        buffer(new T[capacity_]) {}

    ~array() {
        delete[] buffer;
    }

    // O(1) / O(n) if index > capacity and reallocating turned on
    void insert(const size_t index, const T &value) {
        check_index(index);
        buffer[index] = value;
    }

    // O(1) / O(n) if index > capacity and reallocating turned on
    const T &get(const size_t index) {
        check_index(index);
        return buffer[index];
    }

    // O(1) / O(n) if index > capacity and reallocating turned on
    T &operator[](const size_t index) {
        check_index(index);
        return buffer[index];
    }

    // O(1)
    size_t capacity() const {
        return capacity_;
    }

    // O(1)
    bool autom_reallocating() const {
        return autom_realloc;
    }

    // O(n)
    void reallocate(const size_t new_capacity) {
        T *new_buffer = new T[new_capacity];
        std::copy_n(buffer, std::min(capacity_, new_capacity), new_buffer);
        capacity_ = new_capacity;
        delete[] buffer;
        buffer = new_buffer;
    }
};

}

#endif
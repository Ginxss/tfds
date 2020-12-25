#ifndef TF_ARRAY_H
#define TF_ARRAY_H

#include <algorithm> // std::copy_n, std::swap, std::min
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
    bool auto_realloc;
    T *buffer;

    // METHODS

    // reallocation size is the smallest multiple of the current capacity that can hold the index.
    void check_index_realloc_if_on(const size_t index) {
        if (index < 0) {
            throw exception("array: negative index");
        }
        else if (index >= capacity_) {
            if (auto_realloc) {
                size_t new_capacity = capacity_ * ((index / capacity_) + 1);
                if (new_capacity <= index)
                    throw exception("array: capacity too large");
                
                reallocate(new_capacity);
            }
            else {
                throw exception("array: index larger than capacity");
            }
        }
    }

    void check_index_always_except(const size_t index) const {
        if (index < 0) {
            throw exception("array: negative index");
        }
        else if (index >= capacity_) {
            throw exception("array: index larger than capacity");
        }
    }

public:
    // constructor
    array(const size_t initial_capacity = 10, const bool auto_realloc = true):
        capacity_((initial_capacity > 0) ? initial_capacity : 1),
        auto_realloc(auto_realloc),
        buffer(new T[capacity_]) {}

    // copy constructor
    array(const array &other):
        capacity_(other.capacity_),
        auto_realloc(other.auto_realloc),
        buffer(new T[capacity_])
    {
        std::copy_n(other.buffer, other.capacity_, buffer);
    }

    // destructor
    ~array() {
        delete[] buffer;
    }

    friend void swap(array &first, array &second) noexcept {
        using std::swap;
        swap(first.capacity_, second.capacity_);
        swap(first.auto_realloc, second.auto_realloc);
        swap(first.buffer, second.buffer);
    }

    // copy assignment operator
    array &operator=(array other) {
        swap(*this, other);
        return *this;
    }

    // move constructor
    array(array &&other) noexcept : array(1) {
        swap(*this, other);
    }

    // move assignment operator
    array &operator=(array &&other) {
        swap(*this, other);
        return *this;
    }

    // O(1) / O(n) if index > capacity and reallocating turned on
    void insert(const size_t index, const T &value) {
        check_index_realloc_if_on(index);
        buffer[index] = value;
    }

    // O(1) / O(n) if index > capacity and reallocating turned on
    const T &get(const size_t index) const {
        check_index_always_except(index);
        return buffer[index];
    }

    // O(1) / O(n) if index > capacity and reallocating turned on
    T &operator[](const size_t index) {
        check_index_realloc_if_on(index);
        return buffer[index];
    }

    // O(1)
    size_t capacity() const {
        return capacity_;
    }

    // O(1)
    bool auto_reallocating() const {
        return auto_realloc;
    }

    // O(n)
    void reallocate(const size_t new_capacity) {
        if (new_capacity > 0) {
            T *new_buffer = new T[new_capacity];
            std::copy_n(buffer, std::min(capacity_, new_capacity), new_buffer);
            capacity_ = new_capacity;
            delete[] buffer;
            buffer = new_buffer;
        }
    }
};

}

#endif
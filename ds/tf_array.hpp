#ifndef TF_ARRAY_H
#define TF_ARRAY_H

#include <cstring> // memset

namespace tf {

/*
* Dynamic array that reallocates when accessed out of bounds.
*/
template <typename T>
class array {
private:
    // VARIABLES

    int capacity_;
    T *buffer;

    // METHODS

    // reallocation size is the smallest multiple of the current capacity that can hold the index.
    void check_size(const int index) {
        if (index >= capacity_) {
            capacity_ *= (int)((index / capacity_) + 1);
            buffer = (T *)realloc(buffer, capacity_ * sizeof(T));
        }
    }

public:
    array(const int initial_capacity = 10):
        capacity_(initial_capacity),
        buffer((T *)malloc(capacity_ * sizeof(T)))
    {
        clear();
    }

    ~array() {
        free(buffer);
    }

    // O(1) - O(n) if index > capacity
    void insert(const int index, const T &value) {
        check_size(index);
        buffer[index] = value;
    }

    // O(1) - O(n) if index > capacity
    const T &get(const int index) {
        check_size(index);
        return buffer[index];
    }

    // O(1) - O(n) if index > capacity
    T &operator[](const int index) {
        check_size(index);
        return buffer[index];
    }

    // O(1)
    int capacity() const {
        return capacity_;
    }

    // O(n)
    void clear() {
        memset(buffer, 0, capacity_ * sizeof(T));
    }
};

}

#endif
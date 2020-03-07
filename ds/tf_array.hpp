//////////
// DONE //
//////////

#ifndef TF_ARRAY_H
#define TF_ARRAY_H

#include <cstdlib> // malloc

namespace tf {

/*
* Dynamic array that reallocates with twice the size when accessed out of bounds.
* Slightly slower than a c-array, but more secure.
*/
template <class T>
class array {
private:
    int capacity_;
    T *buffer;

    // O(1) - O(n) if index > capacity
    void check_size(const int index) {
        if (index >= capacity_) {
            capacity_ *= 2;
            buffer = (T *)realloc(buffer, capacity_ * sizeof(T));
        }
    }

public:
    array(const int capacity = 10):
        capacity_(capacity),
        buffer((T *)malloc(capacity_ * sizeof(T))) {}

    ~array() {
        clear();
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

    int capacity() const {
        return capacity_;
    }

    // O(1)
    void clear() {
        if (buffer)
            free(buffer);
    }
};

}

#endif
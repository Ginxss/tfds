#ifndef TF_ARRAY_H
#define TF_ARRAY_H

#include <cstdlib> // malloc

namespace tf {

template <class T>
class array {
private:
    int capacity;
    T *buffer;

    // O(1) - O(n) if index > capacity
    void check_size(const int index) {
        if (index >= capacity) {
            capacity *= 2;
            buffer = (T *)realloc(buffer, capacity * sizeof(T));
        }
    }

public:
    array(const int capacity = 10):
        capacity(capacity),
        buffer((T *)malloc(capacity * sizeof(T))) {}

    ~array() {
        close();
    }

    // O(1) - O(n) if index > capacity
    void insert(const int index, const T &value) {
        check_size(index);
        buffer[index] = value;
    }

    // O(1) - O(n) if index > capacity
    T &get(const int index) const {
        check_size(index);
        return buffer[index];
    }

    // O(1) - O(n) if index > capacity
    T &operator[](const int index) const {
        check_size(index);
        return buffer[index];
    }

    // O(1)
    void close() {
        if (buffer)
            free(buffer);
    }
};

}

#endif
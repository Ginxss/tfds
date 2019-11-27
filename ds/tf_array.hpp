#ifndef TF_ARRAY_H
#define TF_ARRAY_H

#include <cstdio> // printf
#include <cstdlib> // malloc

namespace tf {

template <class T>
class array {
private:
    int capacity;
    T *buffer;

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

    void insert(const int index, const T &value) {
        check_size(index);
        buffer[index] = value;
    }

    T &get(const int index) const {
        check_size(index);
        return buffer[index];
    }

    void close() {
        if (buffer)
            free(buffer);
    }

    T &operator[](const int index) const {
        check_size(index);
        return buffer[index];
    }
};

}

#endif
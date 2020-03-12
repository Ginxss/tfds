//////////
// DONE //
//////////

#ifndef TF_ARRAY_H
#define TF_ARRAY_H

namespace tf {

/*
* Dynamic array that reallocates when accessed out of bounds.
* New allocation size is the smallest multiple of the current capacity, that can fit the index.
* Slightly slower than a c-array, but more secure.
*/
template <typename T>
class array {
private:
    // VARIABLES

    int capacity_;
    T *buffer;

    // METHODS

    void check_size(const int index) {
        if (index >= capacity_) {
            capacity_ *= (int)((index / capacity_) + 1);
            buffer = (T *)realloc(buffer, capacity_ * sizeof(T));
        }
    }

public:
    array(const int capacity = 10):
        capacity_(capacity),
        buffer((T *)malloc(capacity_ * sizeof(T))) {}

    ~array() {
        if (buffer)
            free(buffer);       
        buffer = nullptr;
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
};

}

#endif
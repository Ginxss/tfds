#ifndef TF_VECTOR_H
#define TF_VECTOR_H

#include <new> // std:bad_alloc
#include <algorithm> // std::copy_n, std::swap
#include "tf_exception.hpp"
#include "tf_compare_functions.hpp"

namespace tf {

/*
* Dynamic array (similar to std::vector).
*/
template <typename T>
class vector {
private:
    // VARIABLES

    size_t capacity;
	size_t size_;
    T *buffer;

public:
    // constructor
    vector(const size_t initial_capacity = 10):
        capacity((initial_capacity > 0) ? initial_capacity : 1),
		size_(0),
        buffer(new T[capacity]) {}

    // copy constructor
    vector(const vector &other):
        capacity(other.capacity),
		size_(other.size_),
        buffer(new T[capacity])
    {
        std::copy_n(other.buffer, size_, buffer);
    }

    // destructor
    ~vector() {
        delete[] buffer;
    }

    friend void swap(vector &first, vector &second) noexcept {
        using std::swap;
        swap(first.capacity, second.capacity);
        swap(first.size_, second.size_);
        swap(first.buffer, second.buffer);
    }

    // copy assignment operator
    vector &operator=(vector other) {
        swap(*this, other);
        return *this;
    }

    // move constructor
    vector(vector &&other) noexcept : vector(1) {
        swap(*this, other);
    }

    // move assignment operator
    vector &operator=(vector &&other) {
        swap(*this, other);
        return *this;
    }

	// O(1) / O(n) on reallocation
	void add(const T &value) {
		if (size_ >= capacity) {
			size_t new_capacity = capacity * 2;
			if (new_capacity <= size_) {
				throw exception("vector: add: too large, new capacity created buffer overflow");
			}

			try {
				T *new_buffer = new T[new_capacity];
				std::copy_n(buffer, size_, new_buffer);
				capacity = new_capacity;
				delete[] buffer;
				buffer = new_buffer;
			}
			catch (std::bad_alloc &) {
				throw exception("vector: add: bad_alloc caught, vector is probably too big");
			}
        }

		buffer[size_++] = value;
	}

    // O(1)
    const T &get(const size_t index) const {
		if (index < size_)
        	return buffer[index];
		else
			throw exception("vector: get: index larger than size");
    }

    // O(1)
    T &operator[](const size_t index) {
		if (index < size_)
        	return buffer[index];
		else
			throw exception("vector: []: index larger than size");
    }

    // O(1)
    const T &operator[](const size_t index) const {
        if (index < size_)
        	return buffer[index];
		else
			throw exception("vector: []: index larger than size");
    }

    // O(n)
    void set_all(const T &value) {
        for (size_t i = 0; i < size_; ++i) {
            buffer[i] = value;
        }
    }

	// O(n)
	T remove(const size_t &index) {
		if (index < size_) {
			T result = buffer[index];

			--size_;
			for (size_t i = index; i < size_; ++i) {
				buffer[i] = buffer[i+1];
			}

			return result;
		}

		throw exception("vector: remove: index larger than size");
	}

    // O(n)
    bool contains(const T &value) const {
        for (size_t i = 0; i < size_; ++i) {
            if (compare<T>(value, buffer[i])) {
                return true;
            }
        }

        return false;
    }

	// O(1)
    size_t size() const {
        return size_;
    }

    // O(1)
    size_t current_capacity() const {
        return capacity;
    }

	// O(1)
	bool empty() const {
		return size_ == 0;
	}

	// O(1)
	void clear() {
		size_ = 0;
	}
};

}

#endif
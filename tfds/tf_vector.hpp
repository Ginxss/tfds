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

    size_t capacity_;
	size_t size_;
    T *buffer;

public:
    // constructor
    vector(const size_t initial_capacity = 10):
        capacity_((initial_capacity > 0) ? initial_capacity : 1),
		size_(0),
        buffer(new T[capacity_]) {}

    // copy constructor
    vector(const vector &other):
        capacity_(other.capacity_),
		size_(other.size_),
        buffer(new T[capacity_])
    {
        std::copy_n(other.buffer, size_, buffer);
    }

    // destructor
    ~vector() {
        delete[] buffer;
    }

    friend void swap(vector &first, vector &second) noexcept {
        using std::swap;
        swap(first.capacity_, second.capacity_);
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
		if (size_ >= capacity_) {
			size_t new_capacity = capacity_ * 2;
			if (new_capacity <= size_) {
				throw exception("vector: add: too large, new capacity created buffer overflow");
			}

            reallocate(new_capacity);
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
	T remove(const size_t index) {
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
    void reallocate(const size_t new_capacity) {
        if (new_capacity == 0)
            throw exception("vector: reallocate: new_capacity must be > 0");
        
        try {
            T *new_buffer = new T[new_capacity];
            std::copy_n(buffer, std::min(capacity_, new_capacity), new_buffer);
            capacity_ = new_capacity;
            delete[] buffer;
            buffer = new_buffer;
        }
        catch (std::bad_alloc &) {
            throw exception("vector: reallocate: bad_alloc caught, vector is probably too big");
        }
    }

    // O(n)
    bool contains(const T &value) const {
        for (size_t i = 0; i < size_; ++i) {
            if (equals<T>(value, buffer[i])) {
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
    size_t capacity() const {
        return capacity_;
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
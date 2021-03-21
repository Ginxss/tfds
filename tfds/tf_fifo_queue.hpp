#ifndef TF_FIFO_QUEUE_H
#define TF_FIFO_QUEUE_H

#include "tf_linked_list.hpp"

namespace tf {

/*
* FIFO Queue: Wrapper for tf::linked_list.
*/
template <typename T>
class fifo_queue {
private:
    linked_list<T> list;

public:
    // constructor
    fifo_queue() {}

    // copy constructor
    fifo_queue(const fifo_queue &other):
        list(other.list) {}

    friend void swap(fifo_queue &first, fifo_queue &second) noexcept {
        using std::swap;
        swap(first.list, second.list);
    }

    // move constructor
    fifo_queue(fifo_queue &&other) noexcept : fifo_queue() {
        swap(*this, other);
    }

    // copy assignment operator
    fifo_queue &operator=(fifo_queue other) {
        swap(*this, other);
        return *this;
    }

    // O(1)
    void add(const T &value) {
        list.add_front(value);
    }

    // O(1)
    T next() {
        if (empty())
            throw exception("fifo queue: next: queue is empty");

        return list.pop_back();
    }

    // O(n)
    bool contains(const T &value) const {
        return list.contains(value);
    }

    // O(n)
    void clear() {
        list.clear();
    }

    // O(1)
    size_t length() const {
        return list.length();
    }

    // O(1)
    bool empty() const {
        return list.empty();
    }
};

}

#endif
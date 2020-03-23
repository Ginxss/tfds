#ifndef TF_FIFO_QUEUE_H
#define TF_FIFO_QUEUE_H

#include "../ds/tf_linked_list.hpp"

namespace tf {

/*
* FIFO-Queue: Wrapper for tf::linked_list
*/
template <typename T>
class fifo_queue {
private:
    tf::linked_list<T> list;

public:
    // O(1)
    void add(const T &value) {
        list.add_front(value);
    }

    // O(1)
    T next() {
        return list.pop_back();
    }

    // O(n)
    bool contains(const T &value) const {
        return list.contains(value);
    }

    // O(1)
    int length() const {
        return list.length();
    }

    // O(1)
    bool empty() const {
        return list.empty();
    }

    // O(n)
    void clear() {
        list.clear();
    }
};

}

#endif
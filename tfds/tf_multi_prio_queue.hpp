#ifndef TF_MULTI_PRIO_QUEUE_H
#define TF_MULTI_PRIO_QUEUE_H

#include "tf_multi_search_tree.hpp"

namespace tf {

/*
* Priority Queue: Wrapper for tf::multi_search_tree.
*/
template <typename K, typename V>
class multi_prio_queue {
private:
    multi_search_tree<K, V> tree;

public:
    // constructor
    multi_prio_queue() {}

    // copy constructor
    multi_prio_queue(const multi_prio_queue &other) {
        tree = other.tree;
    }

    friend void swap(multi_prio_queue &first, multi_prio_queue &second) noexcept {
        using std::swap;
        swap(first.tree, second.tree);
    }

    // copy assignment operator
    multi_prio_queue &operator=(multi_prio_queue other) {
        swap(*this, other);
        return *this;
    }

    // move constructor
    multi_prio_queue(multi_prio_queue &&other) noexcept : multi_prio_queue() {
        swap(*this, other);
    }

    // move assignment operator
    multi_prio_queue &operator=(multi_prio_queue &&other) {
        swap(*this, other);
        return *this;
    }

    // O(log(n))
    void insert(const K &key, const V &value) {
        tree.insert(key, value);
    }

    // O(log(n))
    V next_min() {
        try {
            return tree.pop_min();
        }
        catch (exception &e) {
            throw exception("prio queue: next_min: queue is empty");
        }
    }

    // O(log(n))
    V next_max() {
        try {
            return tree.pop_max();
        }
        catch (exception &e) {
            throw exception("prio queue: next_max: queue is empty");
        }
    }

    // O(log(n))
    bool contains(const K &key) const {
        return tree.contains(key);
    }

    // O(1)
    size_t length() const {
        return tree.size();
    }

    // O(1)
    bool empty() const {
        return tree.empty();
    }

    // O(n)
    void clear() {
        tree.clear();
    }
};

}

#endif
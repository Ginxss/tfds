#ifndef TF_PRIO_QUEUE_H
#define TF_PRIO_QUEUE_H

#include "tf_search_tree.hpp"

namespace tf {

/*
* Priority Queue: Wrapper for tf::search_tree.
*/
template <typename K, typename V>
class prio_queue {
private:
    search_tree<K, V> tree;

public:
    prio_queue() {}

    // O(n)
    prio_queue(const prio_queue &other) {
        tree = other.tree;
    }

    // O(n)
    prio_queue &operator=(const prio_queue &other) {
        tree = other.tree;
    }

    // O(log(n))
    void insert(const K &key, const V &value) {
        try {
            tree.insert(key, value);
        }
        catch (exception &e) {
            throw exception("prio queue: insert: key already exists");
        }
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
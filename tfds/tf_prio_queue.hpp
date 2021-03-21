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
    // constructor
    prio_queue(const bool allow_duplicate_keys = false):
        tree(search_tree<K, V>(allow_duplicate_keys)) {}

    // copy constructor
    prio_queue(const prio_queue &other):
        tree(other.tree) {}

    friend void swap(prio_queue &first, prio_queue &second) noexcept {
        using std::swap;
        swap(first.tree, second.tree);
    }

    // move constructor
    prio_queue(prio_queue &&other) noexcept : prio_queue() {
        swap(*this, other);
    }

    // copy assignment operator
    prio_queue &operator=(prio_queue other) {
        swap(*this, other);
        return *this;
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
        if (empty())
            throw exception("prio queue: next_min: queue is empty");

        return tree.pop_min();
    }

    // O(log(n))
    V next_max() {
        if (empty())
            throw exception("prio queue: next_max: queue is empty");

        return tree.pop_max();
    }

    // O(log(n))
    bool contains(const K &key) const {
        return tree.contains(key);
    }

    // O(n)
    void clear() {
        tree.clear();
    }

    // O(1)
    size_t length() const {
        return tree.size();
    }

    // O(1)
    bool empty() const {
        return tree.empty();
    }
};

}

#endif
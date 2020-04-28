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
    tf::multi_search_tree<K, V> tree;

public:
    // O(log(n))
    void insert(const K &key, const V &value) {
        tree.insert(key, value);
    }

    // O(log(n))
    V next_min() {
        try {
            return tree.pop_min();
        }
        catch (tf::exception &e) {
            throw tf::exception("prio queue: next_min: queue is empty");
        }
    }

    // O(log(n))
    V next_max() {
        try {
            return tree.pop_max();
        }
        catch (tf::exception &e) {
            throw tf::exception("prio queue: next_max: queue is empty");
        }
    }

    // O(log(n))
    bool contains(const K &key) const {
        return tree.contains(key);
    }

    // O(1)
    int length() const {
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
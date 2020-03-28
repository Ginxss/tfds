#ifndef TF_PRIO_QUEUE_H
#define TF_PRIO_QUEUE_H

#include "../ds/tf_search_tree.hpp"

namespace tf {

/*
* Priority-Queue: Wrapper for tf::search_tree
*/
template <typename K, typename V>
class prio_queue {
private:
    tf::search_tree<K, V> tree;

public:
    // O(log(n))
    void insert(const K &key, const V &value) {
        tree.insert(key, value);
    }

    // O(log(n))
    V next_min() {
        return tree.pop_min();
    }

    // O(log(n))
    V next_max() {
        return tree.pop_max();
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
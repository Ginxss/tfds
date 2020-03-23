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
    void insert(const K &key, const V &value) {
        tree.insert(key, value);
    }

    V next_min() {
        return tree.pop_min();
    }

    V next_max() {
        return tree.pop_max();
    }

    bool contains(const K &key) const {
        return tree.contains(key);
    }

    int length() const {
        return tree.size();
    }

    bool empty() const {
        return tree.empty();
    }

    void clear() {
        tree.clear();
    }
};

}

#endif
////////////
// USABLE //
////////////

#ifndef TF_QUEUE_H
#define TF_QUEUE_H

#include "tf_exception.hpp"

namespace tf {

template <typename T>
class queue {
private:
    // NODE

    struct node {
        T value;
        node *prev;
    };

    node *alloc_node(const T &value, node *prev) {
        node *n = (node *)malloc(sizeof(node));
        n->value = value;
        n->prev = prev;
        return n;
    }

    // VARIABLES

    node *input;
    node *output;
    int length_;

public:

    queue():
        input(nullptr),
        output(nullptr) {}

    ~queue() {
        clear();
    }

    // O(1)
    void add(const T &value) {
        if (empty()) {
            input = alloc_node(value, nullptr);
            output = input;
        }
        else {
            input->prev = alloc_node(value, nullptr);
            input = input->prev;
        }

        length_++;
    }

    // O(1)
    T get() {
        if (empty())
            throw tf::exception("queue: get: queue is empty");
        
        T result = output->value;

        node *to_delete = output;
        output = output->prev;
        free(to_delete);

        if (!output)
            input = nullptr;

        return result;
    }

    // O(1)
    int length() const {
        return length_;
    }

    // O(1)
    bool empty() const {
        return input == nullptr;
    }

    // O(n)
    void clear() {
        node *it = output;
        while (it) {
            node *to_delete = it;
            it = it->prev;
            free(to_delete);
        }
        
        input = nullptr;
        output = nullptr;
    }

};

}

#endif
#ifndef TF_DOUBLY_LINKED_LIST
#define TF_DOUBLY_LINKED_LIST

#include <cstdio>

namespace tf {

template <class T>
class doubly_linked_list {
private:
    class node {
    public:
        T value;
        node *prev;
        node *next;

        node(const T &value, node *prev = nullptr, node *next = nullptr):
            value(value),
            prev(prev),
            next(next) {}
    };

    node *start_node;
    node *end_node;

public:
    doubly_linked_list():
        start_node(nullptr),
        end_node(nullptr) {}

    doubly_linked_list(const T& value):
        start_node(new node(value)),
        end_node(start_node) {}

    ~doubly_linked_list() {
        close();
    }

    // O(1)
    void add(const T &value) {
        if (empty()) {
            start_node = new node(value);
            end_node = start_node;
        }
        else {
            end_node->next = new node(value, end_node);
            end_node = end_node->next;
        }
    }

    // O(n) - O(1) for first and last element
    void remove(const T &value) {
        if (empty())
            return;
        
        if (start_node->value == value) {
            node *to_delete = start_node;
            start_node = start_node->next;
            start_node->prev = nullptr;
            delete to_delete;
        }
        else if (end_node->value == value) {
            node *to_delete = end_node;
            end_node = end_node->prev;
            end_node->next = nullptr;
            delete to_delete;
        }
        else {
            node *it = start_node;

            while (it) {
                if (it->value == value) {
                    it->prev->next = it->next;
                    it->next->prev = it->prev;
                    delete it;
                    break;
                }

                it = it->next;
            }
        }
    }

    // O(1)
    bool empty() const {
        return start_node == nullptr;
    }

    // O(n)
    void close() {
        node *it = start_node;

        while (it) {
            node *to_delete = it;
            it = it->next;
            delete to_delete;
        }
    }
};

}


#endif
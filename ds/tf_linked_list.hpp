////////////
// USABLE //
////////////

#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

#include "tf_exception.hpp"

namespace tf {

/*
* Doubly Linked List that allows efficient iteration both forwards and backwards.
* Elements can be added at the end and removed via value.
* Recommended only if the order has to be preserved and only iteration over the entire list is required.
*/
template <typename T>
class linked_list {
private:
    struct node {
        T value;
        node *prev;
        node *next;
    };

    node *alloc_node(const T &value, node *prev, node *next) {
        node *n = (node *)malloc(sizeof(node));
        n->value = value;
        n->prev = prev;
        n->next = next;
    }

public:
    // Provides standard iteration interface - everything O(1)
    class iterator {
    public:
        node *nd;

        T &operator*() { return nd->value; }
        void operator++() { nd = nd->next; }
        void operator--() { nd = nd->prev; }
        bool condition() { return nd != nullptr; }
    };

private:
    node *start_node;
    node *end_node;
    int length_;
    
    iterator start_it;
    iterator end_it;

public:
    linked_list():
        start_node(nullptr),
        end_node(nullptr),
        length_(0) {}

    ~linked_list() {
        clear();
    }

    // O(1)
    void add(const T &value) {
        if (empty()) {
            start_node = alloc_node(value, nullptr, nullptr);
            end_node = start_node;
        }
        else {
            end_node->next = alloc_node(value, end_node, nullptr);
            end_node = end_node->next;
        }

        length_++;
    }

    // O(n)
    void remove(const T &value) {
        node *it = start_node;

        while (it) {
            if (value == it->value) {
                if (it == start_node || it == end_node) {
                    if (it == start_node) {
                        start_node = it->next;
                        if (start_node)
                            start_node->prev = nullptr;
                        else
                            end_node = nullptr;
                    }
                    else { //if (it == end_node)
                        end_node = it->prev;
                        if (end_node)
                            end_node->next = nullptr;
                        else
                            start_node = nullptr;
                    }
                }
                else {
                    it->prev->next = it->next;
                    it->next->prev = it->prev;
                }
                
                free(it);
                length_--;
                return;
            }

            it = it->next;
        }

        throw tf::exception("linked list (c): remove: invalid value");
    }

    // O(1)
    iterator begin() {
        start_it.nd = start_node;
        return start_it;
    }

    // O(1)
    iterator end() {
        end_it.nd = end_node;
        return end_it;
    }

    // O(n)
    bool contains(const T &value) const {
        node *it = start_node;

        while (it) {
            if (value == it->value) {
                return true;
            }

            it = it->next;
        }

        return false;
    }

    // O(1)
    int length() const {
        return length_;
    }

    // O(1)
    bool empty() const {
        return start_node == nullptr;
    }

    // O(n)
    void clear() {
        node *it = start_node;

        while (it) {
            node *to_delete = it;
            it = it->next;
            free(to_delete);
            length_--;
        }

        start_node = nullptr;
        end_node = nullptr;
    }
};

}

#endif
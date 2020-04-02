#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

#include <cstdlib> // malloc
#include "tf_exception.hpp"

namespace tf {

/*
* Doubly Linked List.
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
        return n;
    }

public:
    // Provides standard iteration interface - everything O(1)
    class iterator {
    public:
        node *nd;

        T &operator*() { return nd->value; }
        T &value() { return nd->value; }
        void operator++() { nd = nd->next; }
        void operator--() { nd = nd->prev; }
        bool condition() { return nd != nullptr; }
    };

private:
    // VARIABLES

    int length_;
    node *start_node;
    node *end_node;
    
    iterator start_it;
    iterator end_it;

    // METHODS
    
    void new_end_node(const T &value) {
        end_node->next = alloc_node(value, end_node, nullptr);
        end_node = end_node->next;
    }

    void new_start_node(const T &value) {
        start_node->prev = alloc_node(value, nullptr, start_node);
        start_node = start_node->prev;
    }

    void slide_start_node() {
        start_node = start_node->next;
        if (start_node)
            start_node->prev = nullptr;
        else
            end_node = nullptr;
    }

    void slide_end_node() {
        end_node = end_node->prev;
        if (end_node)
            end_node->next = nullptr;
        else
            start_node = nullptr;
    }

public:
    linked_list():
        length_(0),
        start_node(nullptr),
        end_node(nullptr) {}

    ~linked_list() {
        clear();
    }

    // O(1)
    void add_back(const T &value) {
        if (empty())
            end_node = start_node = alloc_node(value, nullptr, nullptr);
        else
            new_end_node(value);

        length_++;
    }

    // O(1)
    void add_front(const T &value) {
        if (empty())
            end_node = start_node = alloc_node(value, nullptr, nullptr);
        else
            new_start_node(value);

        length_++;
    }

    // O(n)
    void add_after(const T &new_value, const T &existing_value) {
        node *it = start_node;
        while (it) {
            if (existing_value == it->value) {
                if (it == end_node) {
                    new_end_node(new_value);
                }
                else {
                    node *new_node = alloc_node(new_value, it, it->next);
                    it->next->prev = new_node;
                    it->next = new_node;
                }
            
                length_++;
                return;
            }

            it = it->next;
        }

        throw tf::exception("linked list: add_after: existing_value not found");
    }

    // O(n)
    void add_before(const T &new_value, const T &existing_value) {
        node *it = start_node;
        while (it) {
            if (existing_value == it->value) {
                if (it == start_node) {
                    new_start_node(new_value);
                }
                else {
                    node *new_node = alloc_node(new_value, it->prev, it);
                    it->prev->next = new_node;
                    it->prev = new_node;
                }
            
                length_++;
                return;
            }

            it = it->next;
        }

        throw tf::exception("linked list: add_before: existing_value not found");
    }

    // O(n)
    void remove(const T &value) {
        node *it = start_node;

        while (it) {
            if (value == it->value) {
                if (it == start_node || it == end_node) {
                    if (it == start_node)
                        slide_start_node();
                    else //if (it == end_node)
                        slide_end_node();
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

        throw tf::exception("linked list: remove: value not found");
    }

    // O(1)
    T &back() {
        if (empty())
            throw tf::exception("linked list: pop_front: list is empty");
        
        return end_node->value;
    }

    // O(1)
    T &front() {
        if (empty())
            throw tf::exception("linked list: pop_front: list is empty");
        
        return start_node->value;
    }

    // O(1)
    T pop_back() {
        if (empty())
            throw tf::exception("linked list: pop_back: list is empty");
        
        T result = end_node->value;

        node *to_delete = end_node;
        slide_end_node();
        free(to_delete);
        length_--;

        return result;
    }

    // O(1)
    T pop_front() {
        if (empty())
            throw tf::exception("linked list: pop_front: list is empty");
        
        T result = start_node->value;

        node *to_delete = start_node;
        slide_start_node();
        free(to_delete);
        length_--;

        return result;
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
        }

        length_ = 0;
        start_node = nullptr;
        end_node = nullptr;
    }
};

}

#endif
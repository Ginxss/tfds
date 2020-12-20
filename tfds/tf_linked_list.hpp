#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

#include <algorithm> // std::swap
#include "tf_exception.hpp"
#include "tf_compare_functions.hpp"

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

        node(const T &value_) : value(value_) {}
    };

    node *alloc_node(const T &value, node *prev, node *next) {
        node *n = new node(value);
        n->prev = prev;
        n->next = next;
        ++length_;
        return n;
    }

    void free_node(node *n) {
        --length_;
        delete n;
    }

public:
    class iterator {
    public:
        node *nd;

        T &operator*() { return nd->value; }
        T &value() { return nd->value; }
        T *next_value() { return (nd->next) ? &nd->next->value : nullptr; };
        T *prev_value() { return (nd->prev) ? &nd->prev->value : nullptr; };
        void operator++() { nd = nd->next; }
        void operator--() { nd = nd->prev; }
        bool condition() { return nd != nullptr; }
    };

private:
    // VARIABLES

    size_t length_;
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
    // constructor
    linked_list():
        length_(0),
        start_node(nullptr),
        end_node(nullptr) {}

    // copy constructor
    linked_list(const linked_list &other):
        length_(0),
        start_node(nullptr),
        end_node(nullptr)
    {
        node *it = other.start_node;
        while (it) {
            add_back(it->value);
            it = it->next;
        }
    }

    // destructor
    ~linked_list() {
        clear();
    }

    friend void swap(linked_list &first, linked_list & second) {
        using std::swap;
        swap(first.length_, second.length_);
        swap(first.start_node, second.start_node);
        swap(first.end_node, second.end_node);
        swap(first.start_it, second.start_it);
        swap(first.end_it, second.end_it);
    }

    // copy assignment operator
    linked_list &operator=(linked_list other) {
        swap(*this, other);
        return *this;
    }

    // move constructor
    linked_list(linked_list && other) noexcept : linked_list() {
        swap(*this, other);
    }

    // move assignment operator
    linked_list &operator=(linked_list &&other) {
        swap(*this, other);
        return *this;
    }

    // O(1)
    void add_back(const T &value) {
        if (empty())
            end_node = start_node = alloc_node(value, nullptr, nullptr);
        else
            new_end_node(value);
    }

    // O(1)
    void add_front(const T &value) {
        if (empty())
            end_node = start_node = alloc_node(value, nullptr, nullptr);
        else
            new_start_node(value);
    }

    // O(n)
    void add_after(const T &new_value, const T &existing_value) {
        node *it = start_node;
        while (it) {
            if (compare<T>(existing_value, it->value)) {
                if (it == end_node) {
                    new_end_node(new_value);
                }
                else {
                    node *new_node = alloc_node(new_value, it, it->next);
                    it->next->prev = new_node;
                    it->next = new_node;
                }
            
                return;
            }

            it = it->next;
        }

        throw exception("linked list: add_after: existing_value not found");
    }

    // O(n)
    void add_before(const T &new_value, const T &existing_value) {
        node *it = start_node;
        while (it) {
            if (compare<T>(existing_value, it->value)) {
                if (it == start_node) {
                    new_start_node(new_value);
                }
                else {
                    node *new_node = alloc_node(new_value, it->prev, it);
                    it->prev->next = new_node;
                    it->prev = new_node;
                }
            
                return;
            }

            it = it->next;
        }

        throw exception("linked list: add_before: existing_value not found");
    }

    // O(n)
    void remove(const T &value) {
        node *it = start_node;

        while (it) {
            if (compare<T>(value, it->value)) {
                if (it == start_node) {
                    slide_start_node();
                }
                else if (it == end_node) {
                    slide_end_node();
                }
                else {
                    it->prev->next = it->next;
                    it->next->prev = it->prev;
                }
                
                free_node(it);
                return;
            }

            it = it->next;
        }

        throw exception("linked list: remove: value not found");
    }

    // O(1)
    T &back() {
        if (empty())
            throw exception("linked list: pop_front: list is empty");
        
        return end_node->value;
    }

    // O(1)
    T &front() {
        if (empty())
            throw exception("linked list: pop_front: list is empty");
        
        return start_node->value;
    }

    // O(1)
    T pop_back() {
        if (empty())
            throw exception("linked list: pop_back: list is empty");
        
        T result = end_node->value;

        node *to_delete = end_node;
        slide_end_node();
        free_node(to_delete);

        return result;
    }

    // O(1)
    T pop_front() {
        if (empty())
            throw exception("linked list: pop_front: list is empty");
        
        T result = start_node->value;

        node *to_delete = start_node;
        slide_start_node();
        free_node(to_delete);

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
            if (compare<T>(value, it->value)) {
                return true;
            }

            it = it->next;
        }

        return false;
    }

    // O(1)
    size_t length() const {
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
            free_node(to_delete);
        }

        start_node = nullptr;
        end_node = nullptr;
    }
};

}

#endif
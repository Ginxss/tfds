#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

#include <algorithm> // std::swap
#include "utils/tf_exception.hpp"
#include "utils/tf_compare_functions.hpp"

namespace tf {

/*
* Doubly Linked List.
*/
template <typename T>
class linked_list {
private:
    // NODE

    struct node {
        T value;
        node *prev;
        node *next;

        node(const T &value, node *prev, node *next):
            value(value), prev(prev), next(next) {}
    };

    // ITERATORS

    class iterator {
    private:
        node *nd;

    public:
        iterator(node *nd):
            nd(nd) {}
        
        T &operator*() { return nd->value; }
        T &value() { return nd->value; }
        T *next_value() { return (nd->next) ? &nd->next->value : nullptr; }
        T *prev_value() { return (nd->prev) ? &nd->prev->value : nullptr; }
        void operator++() { nd = nd->next; }
        void operator--() { nd = nd->prev; }
        bool condition() const { return nd != nullptr; }
    };

    class const_iterator {
    private:
        const node *nd;

    public:
        const_iterator(const node *nd):
            nd(nd) {}
        
        const T &operator*() const { return nd->value; }
        const T &value() const { return nd->value; }
        const T *next_value() const { return (nd->next) ? &nd->next->value : nullptr; }
        const T *prev_value() const { return (nd->prev) ? &nd->prev->value : nullptr; }
        void operator++() { nd = nd->next; }
        void operator--() { nd = nd->prev; }
        bool condition() const { return nd != nullptr; }
    };

    // VARIABLES

    size_t length_;
    node *start_node;
    node *end_node;

    // METHODS

    node *create_node(const T &value, node *prev, node *next) {
        node *n = new node(value, prev, next);
        ++length_;
        return n;
    }

    void destroy_node(node *n) {
        --length_;
        delete n;
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
        const node *it = other.start_node;
        while (it) {
            add_back(it->value);
            it = it->next;
        }
    }

    // destructor
    ~linked_list() {
        clear();
    }

    friend void swap(linked_list &first, linked_list &second) noexcept {
        using std::swap;
        swap(first.length_, second.length_);
        swap(first.start_node, second.start_node);
        swap(first.end_node, second.end_node);
    }

    // copy assignment operator
    linked_list &operator=(linked_list other) {
        swap(*this, other);
        return *this;
    }

    // move constructor
    linked_list(linked_list &&other) noexcept : linked_list() {
        swap(*this, other);
    }

    // move assignment operator
    linked_list &operator=(linked_list &&other) {
        swap(*this, other);
        return *this;
    }

    // O(1)
    void add_back(const T &value) {
        if (end_node) {
            end_node->next = create_node(value, end_node, nullptr);
            end_node = end_node->next;
        }
        else {
            end_node = start_node = create_node(value, nullptr, nullptr);
        }
    }

    // O(1)
    void add_front(const T &value) {
        if (start_node) {
            start_node->prev = create_node(value, nullptr, start_node);
            start_node = start_node->prev;
        }
        else {
            start_node = end_node = create_node(value, nullptr, nullptr);
        }
    }

    // O(n)
    void add_after(const T &new_value, const T &existing_value) {
        node *it = start_node;
        while (it) {
            if (equals<T>(existing_value, it->value)) {
                if (it == end_node)
                    add_back(new_value);
                else
                    it->next = it->next->prev = create_node(new_value, it, it->next);
            
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
            if (equals<T>(existing_value, it->value)) {
                if (it == start_node)
                    add_front(new_value);
                else
                    it->prev = it->prev->next = create_node(new_value, it->prev, it);
            
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
            if (equals<T>(value, it->value)) {
                if (it == start_node) {
                    pop_front();
                }
                else if (it == end_node) {
                    pop_back();
                }
                else {
                    it->prev->next = it->next;
                    it->next->prev = it->prev;
                    destroy_node(it);
                }
                
                return;
            }

            it = it->next;
        }

        throw exception("linked list: remove: value not found");
    }

    // O(1)
    T &back() {
        if (end_node)
            return end_node->value;
        else
            throw exception("linked list: back: list is empty");
    }

    // O(1)
    const T &back() const {
        if (end_node)
            return end_node->value;
        else
            throw exception("linked list: back: list is empty");
    }

    // O(1)
    T &front() {
        if (start_node)
            return start_node->value;
        else
            throw exception("linked list: front: list is empty");
    }

    // O(1)
    const T &front() const {
        if (start_node)
            return start_node->value;
        else
            throw exception("linked list: front: list is empty");
    }

    // O(1)
    T pop_back() {
        if (end_node) {
            T result = end_node->value;
            node *to_delete = end_node;

            if (end_node->prev) {
                end_node = end_node->prev;
                end_node->next = nullptr;
            }
            else {
                end_node = start_node = nullptr;
            }
            
            destroy_node(to_delete);
            return result;
        }

        throw exception("linked list: pop_back: list is empty");        
    }

    // O(1)
    T pop_front() {
        if (start_node) {
            T result = start_node->value;
            node *to_delete = start_node;

            if (start_node->next) {
                start_node = start_node->next;
                start_node->prev = nullptr;
            }
            else {
                start_node = end_node = nullptr;
            }

            destroy_node(to_delete);
            return result;
        }

        throw exception("linked list: pop_front: list is empty");        
    }

    // O(1)
    iterator begin() {
        return iterator(start_node);
    }

    // O(1)
    const_iterator begin() const {
        return const_iterator(start_node);
    }

    // O(1)
    iterator end() {
        return iterator(end_node);
    }

    // O(1)
    const_iterator end() const {
        return const_iterator(end_node);
    }

    // O(n)
    bool contains(const T &value) const {
        node *it = start_node;
        while (it) {
            if (equals<T>(value, it->value)) {
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
            destroy_node(to_delete);
        }

        start_node = nullptr;
        end_node = nullptr;
    }
};

}

#endif
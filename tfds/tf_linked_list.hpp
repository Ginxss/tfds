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

        node(const T &value, node *prev, node *next):
            value(value), prev(prev), next(next) {}
    };

    node *new_node(const T &value, node *prev, node *next) {
        node *n = new node(value, prev, next);
        ++length_;
        return n;
    }

    void delete_node(node *n) {
        --length_;
        delete n;
    }

public:
    class iterator {
    private:
        node *nd;

    public:
        iterator(node *nd):
            nd(nd) {}
        
        T &operator*() { return nd->value; }
        T &value() { return nd->value; }
        T *next_value() { return (nd->next) ? &nd->next->value : nullptr; };
        T *prev_value() { return (nd->prev) ? &nd->prev->value : nullptr; };
        void operator++() { nd = nd->next; }
        void operator--() { nd = nd->prev; }
        bool condition() const { return nd != nullptr; }
    };

    class const_iterator {
    private:
        node *nd;

    public:
        const_iterator(node *nd):
            nd(nd) {}
        
        const T &operator*() const { return nd->value; }
        const T &value() const { return nd->value; }
        const T *next_value() const { return (nd->next) ? &nd->next->value : nullptr; };
        const T *prev_value() const { return (nd->prev) ? &nd->prev->value : nullptr; };
        void operator++() { nd = nd->next; }
        void operator--() { nd = nd->prev; }
        bool condition() const { return nd != nullptr; }
    };

private:
    // VARIABLES

    size_t length_;
    node *start_node;
    node *end_node;
    
    // METHODS
    
    void new_end_node(const T &value) {
        end_node->next = new_node(value, end_node, nullptr);
        end_node = end_node->next;
    }

    void new_start_node(const T &value) {
        start_node->prev = new_node(value, nullptr, start_node);
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
        if (empty())
            end_node = start_node = new_node(value, nullptr, nullptr);
        else
            new_end_node(value);
    }

    // O(1)
    void add_front(const T &value) {
        if (empty())
            end_node = start_node = new_node(value, nullptr, nullptr);
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
                    node *new_node_ = new_node(new_value, it, it->next);
                    it->next->prev = new_node_;
                    it->next = new_node_;
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
                    node *new_node_ = new_node(new_value, it->prev, it);
                    it->prev->next = new_node_;
                    it->prev = new_node_;
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
                
                delete_node(it);
                return;
            }

            it = it->next;
        }

        throw exception("linked list: remove: value not found");
    }

    // O(1)
    T &back() {
        if (empty())
            throw exception("linked list: back: list is empty");
        
        return end_node->value;
    }

    // O(1)
    const T &back() const {
        if (empty())
            throw exception("linked list: back: list is empty");
        
        return end_node->value;
    }

    // O(1)
    T &front() {
        if (empty())
            throw exception("linked list: front: list is empty");
        
        return start_node->value;
    }

    // O(1)
    const T &front() const {
        if (empty())
            throw exception("linked list: front: list is empty");
        
        return start_node->value;
    }

    // O(1)
    T pop_back() {
        if (empty())
            throw exception("linked list: pop_back: list is empty");
        
        T result = end_node->value;

        node *to_delete = end_node;
        slide_end_node();
        delete_node(to_delete);

        return result;
    }

    // O(1)
    T pop_front() {
        if (empty())
            throw exception("linked list: pop_front: list is empty");
        
        T result = start_node->value;

        node *to_delete = start_node;
        slide_start_node();
        delete_node(to_delete);

        return result;
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
            delete_node(to_delete);
        }

        start_node = nullptr;
        end_node = nullptr;
    }
};

}

#endif
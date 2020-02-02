//////////
// DONE //
//////////

#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

namespace tf {

/*
* Doubly Linked List that allows efficient iteration both forwards and backwards.
* Elements can be added at the end and removed via value.
* Recommended only if the order has to be preserved and only iteration over the entire list is required.
*/
template <class T>
class linked_list {
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

    linked_list(const T& value):
        start_node(new node(value)),
        end_node(start_node),
        length_(1) {}

    ~linked_list() {
        clear();
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

        length_++;
    }

    // O(n) - O(1) for first and last element
    void remove(const T &value) {
        if (empty())
            return;
        
        if (start_node->value == value) {
            length_--;
            node *to_delete = start_node;
            if (start_node->next) {
                start_node = start_node->next;
                start_node->prev = nullptr;
            }
            delete to_delete;
        }
        else if (end_node->value == value) {
            length_--;
            node *to_delete = end_node;
            end_node = end_node->prev;
            end_node->next = nullptr;
            delete to_delete;
        }
        else {
            node *it = start_node;

            while (it) {
                if (it->value == value) {
                    length_--;
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
            if (it->value == value) {
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
        return length_ == 0;
    }

    // O(n)
    void clear() {
        node *it = start_node;

        while (it) {
            length_--;
            node *to_delete = it;
            it = it->next;
            delete to_delete;
        }
    }
};

}

#endif
#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

namespace tf {

template <class T>
class linked_list {
private:
    class node {
    public:
        T value;
        node *next;

        node(const T &value):
            value(value),
            next(nullptr) {}
    };

    node *start_node;
    node *end_node;

public:
    linked_list():
        start_node(nullptr),
        end_node(nullptr) {}

    linked_list(const T &value):
        start_node(new node(value)),
        end_node(start_node) {}

    ~linked_list() {
        close();
    }

    // O(1)
    void add(const T &value) {
        if (empty()) {
            start_node = new node(value);
            end_node = start_node;
        }
        else {
            end_node->next = new node(value);
            end_node = end_node->next;
        }
    }

    // O(n) - O(1) for first element
    void remove(const T &value) {
        if (empty())
            return;

        if (start_node->value == value) {
            node *prev_start = start_node;
            start_node = start_node->next;
            delete prev_start;
        }
        else {
            node *it = start_node;

            while (it->next) {
                if (it->next->value == value) {
                    node *to_delete = it->next;
                    if (to_delete == end_node)
                        end_node = it;

                    it->next = it->next->next;
                    delete to_delete;
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
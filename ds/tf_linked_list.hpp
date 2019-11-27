#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

namespace tf {

template <class T>
class linked_list {
private:
    class linked_list_node {
    public:
        T value;
        linked_list_node *next;

        linked_list_node(const T &value):
            value(value),
            next(nullptr) {}
    };

    linked_list_node *start_node;
    linked_list_node *end_node;

public:
    linked_list():
        start_node(nullptr),
        end_node(nullptr) {}

    linked_list(const T &value):
        start_node(new linked_list_node(value)),
        end_node(start_node) {}

    ~linked_list() {
        close();
    }

    void add(const T &value) {
        if (empty()) {
            start_node = new linked_list_node(value);
            end_node = start_node;
        }
        else {
            end_node->next = new linked_list_node(value);
            end_node = end_node->next;
        }
    }

    void remove(const T &value) {
        if (!start_node)
            return;

        if (start_node->value == value) {
            linked_list_node *prev_start = start_node;
            start_node = start_node->next;
            delete prev_start;
        }
        else {
            linked_list_node *it = start_node;

            while (it->next) {
                if (it->next->value == value) {
                    linked_list_node *to_delete = it->next;
                    if (to_delete == end_node)
                        end_node = it;

                    it->next = it->next->next;
                    delete to_delete;
                }

                it = it->next;
            }
        }
    }

    bool empty() {
        return start_node == nullptr;
    }

    void close() {
        linked_list_node *it = start_node;

        while (it) {
            linked_list_node *to_delete = it;
            it = it->next;
            delete to_delete;
        }
    }
};

};

#endif
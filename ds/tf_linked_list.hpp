#ifndef TF_LINKED_LIST_H
#define TF_LINKED_LIST_H

namespace tf {

template <class T>
class linked_list_node {
private:
    T value;
    linked_list_node *next_node = nullptr;
};

template <class T>
class linked_list {
private:
    linked_list_node<T> start_node;

public:
    linked_list();
};

};

#endif
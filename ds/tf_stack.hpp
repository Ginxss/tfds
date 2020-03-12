//////////
// DONE //
//////////

#ifndef TF_STACK_H
#define TF_STACK_H

#include "tf_exception.hpp"

namespace tf {

/*
* Standard Stack structure.
* Put, Peek and Pop
*/
template <typename T>
class stack {
private:
    // ITEM

    struct item {
        T value;
        item *under;
    };

    item *alloc_item(const T &value, item *under) {
        item *i = (item *)malloc(sizeof(item));
        i->value = value;
        i->under = under;
        return i;
    }

    // VARIABLES

    item *top;

public:
    stack():
        top(nullptr) {}

    ~stack() {
        clear();
    }

    // O(1)
    void put(const T &value) {
        top = alloc_item(value, top);
    }

    // O(1)
    T &peek() {
        if (empty())
            throw tf::exception("stack: peek: stack is empty");
        
        return top->value;
    }

    // O(1)
    T pop() {
        if (empty())
            throw tf::exception("stack: pop: stack is empty");
        
        T value = top->value;

        item *to_delete = top;
        top = top->under;
        free(to_delete);

        return value;
    }

    // O(1)
    bool empty() const {
        return top == nullptr;
    }

    // O(n)
    void clear() {
        while (!empty()) {
            item *to_delete = top;
            top = top->under;
            free(to_delete);
        }
    }
};

}

#endif
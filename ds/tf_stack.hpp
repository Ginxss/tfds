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
template <class T>
class stack {
private:
    class stack_item {
    public:
        T value;
        stack_item *under;

        stack_item(T value, stack_item *under = nullptr):
            value(value),
            under(under) {}
    };

    stack_item *top;

public:
    stack():
        top(nullptr) {}

    stack(const T &value):
        top(new stack_item(value)) {}

    ~stack() {
        clear();
    }

    // O(1)
    void put(const T &value) {
        top = new stack_item(value, top);
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

        stack_item *prev_top = top;
        top = top->under;
        delete prev_top;

        return value;
    }

    // O(1)
    bool empty() const {
        return top == nullptr;
    }

    // O(n)
    void clear() {
        while (!empty()) {
            stack_item *prev_top = top;
            top = top->under;
            delete prev_top;
        }
        
        top = nullptr;
    }
};

}

#endif
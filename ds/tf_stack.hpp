namespace tf {

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

    int capacity;
    stack_item *top;

public:
    stack(int capacity = 10):
        capacity(capacity),
        top(nullptr) {}

    stack(const T &value, int capacity = 10):
        capacity(capacity),
        top(new stack_item(value)) {}

    ~stack() {
        close();
    }

    void put(const T &value) {
        stack_item *item = new stack_item(value, top);
        top = item;
    }

    T &peek() {
        return top->value;
    }

    T pop() {
        T value = top->value;

        stack_item *prev_top = top;
        top = top->under;
        delete prev_top;

        return value;
    }

    bool empty() {
        return top == nullptr;
    }

    void close() {
        while (!empty()) {
            stack_item *prev_top = top;
            top = top->under;
            delete prev_top;
        }
    }
};

};
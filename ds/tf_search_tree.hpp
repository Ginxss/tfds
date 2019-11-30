#ifndef TF_SEARCH_TREE_H
#define TF_SEARCH_TREE_H

namespace tf {

template <class T>
class search_tree {
private:
    class node {
    public:
        T value;
        node *left;
        node *right;

        node(const T &value):
            value(value),
            left(nullptr),
            right(nullptr) {}
    }

    node *root;

public:
    tree():
        root(nullptr) {}

    tree(const T &value):
        root(new node(value)) {}
    
    ~tree() {
        clear();
    }

    // TODO: always balance the tree, so that the search running time is always O(log(n)) and removing is O(1)
    // Make this an AVL tree
    // https://en.wikipedia.org/wiki/AVL_tree
    // O(log(n)) for a balanced tree - O(n) for a linear tree
    void insert(const T &value) {
        if (empty()) {
            root = new node(value);
        }
        else {
            node *it = root;

            while (true) {
                if (value == it->value)
                    break;
                else if (value < it->value)
                    it = it->left;
                else if (value > it->value)
                    it = it->right;

                if (!it) {
                    it = new node(value);
                    break;
                }
            }
        }
    }

    void remove(const T &value) {
        // TODO: how do i rebalance the tree?
    }

    // O(1)
    bool empty() const {
        return root == nullptr;
    }

    // O(n)
    void clear() {
        // TODO
    }
};

}

#endif
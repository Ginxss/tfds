//////////
// TODO //
//////////

#ifndef TF_SEARCH_TREE_H
#define TF_SEARCH_TREE_H

#include <iostream> // print            |
#include "tf_linked_list.hpp" // print  | DEBUG

namespace tf {

template <class T>
class search_tree {
private:
    class node {
    public:
        T value;
        int height;
        node *parent;
        node *left;
        node *right;

        node(const T &value, node *parent = nullptr):
            value(value),
            height(1),
            parent(parent),
            left(nullptr),
            right(nullptr) {}
    };

    node *root;

    int height(node *n) {
        return (n) ? n->height : 0;
    }

    void update_height(node *n) {
        if (n) {
            int left_height = height(n->left);
            int right_height = height(n->right);
            n->height = ((right_height > left_height) ? right_height : left_height) + 1;
        }
    }

    bool is_left_child(node *child) {
        if (child) {
            node *parent = child->parent;
            if (parent->left && parent->left->value == child->value) {
                return true;
            }
        }

        return false;
    }

    void set_left(node *parent, node *child) {
        if (parent) {
            parent->left = child;
            if (child)
                child->parent = parent;
        }
    }

    void set_right(node *parent, node *child) {
        if (parent) {
            parent->right = child;
            if (child)
                child->parent = parent;
        }
    }

    node *rotation(node *n, bool left) {
        // set the new n
        node *n_before = n;
        if (left)
            n = n->right;
        else
            n = n->left;

        if (n_before == root) {
            root = n;
        }

        // swap parents
        if (n_before->parent) {
            if (is_left_child(n_before))
                set_left(n_before->parent, n);
            else
                set_right(n_before->parent, n);
        }
        else {
            n->parent = nullptr;
        }
        n_before->parent = n;

        // swap trees
        if (left) {
            set_right(n_before, n->left);
            set_left(n, n_before);
        }
        else {
            set_left(n_before, n->right);
            set_right(n, n_before);
        }

        update_height(n->left);
        update_height(n->right);
        update_height(n);

        return n;
    }

    node *left_rotation(node *n) {
        return rotation(n, true);
    }

    node *right_rotation(node *n) {
        return rotation(n, false);
    }

public:
    search_tree():
        root(nullptr) {}

    search_tree(const T &value):
        root(new node(value)) {}
    
    ~search_tree() {
        clear();
    }

    // https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm
    // https://wkdtjsgur100.github.io/avl-tree/

    // O(log(n))
    void insert(const T &value) {
        if (empty()) {
            root = new node(value);
            return;
        }

        node *it = root;

        // insert
        while (true) {
            if (value < it->value) {
                if (it->left) {
                    it = it->left;
                }
                else {
                    it->left = new node(value, it);
                    break;
                }
            }
            else if (value > it->value) {
                if (it->right) {
                    it = it->right;
                }
                else {
                    it->right = new node(value, it);
                    break;
                }
            }
            else { // if (value == it->value)
                break;
            }
        }

        // rebalance
        while (it) {
            update_height(it);

            int balance = height(it->right) - height(it->left);
            if (balance > 1) {
                if (value > it->right->value) {
                    it = left_rotation(it);
                }
                else {
                    it->right = right_rotation(it->right);
                    it = left_rotation(it);
                }
            }
            else if (balance < -1) {
                if (value < it->left->value) {
                    it = right_rotation(it);
                }
                else {
                    it->left = left_rotation(it->left);
                    it = right_rotation(it);
                }
            }

            it = it->parent;
        }
    }

    // DEBUG
    void print() const {
        tf::linked_list<node *> list;
        list.add(root);

        tf::linked_list<node *> next_level;

        while (!list.empty()) {
            // print level
            for (auto it = list.begin(); it.condition(); ++it) {
                if ((*it)) {
                    std::cout << (*it)->value << "(" << (*it)->height << ") ";

                    // add next level
                    next_level.add((*it)->left);
                    next_level.add((*it)->right);
                }
                else {
                    std::cout << "_ ";
                }
            }
            std::cout << std::endl;

            // transfer next level
            list.clear();
            for (auto it = next_level.begin(); it.condition(); ++it) {
                list.add(*it);
            }
            next_level.clear();
        }
    }

    void remove(const T &value) {
        // TODO
    }

    // O(1)
    bool empty() const {
        return root == nullptr;
    }

    // O(n)
    void clear() {
        node *it = root;

        while (it) {
            if (it->left) {
                it = it->left;
                continue;
            }
            if (it->right) {
                it = it->right;
                continue;
            }

            node *to_delete = it;
            if (it->parent) {
                it = it->parent;
                if (is_left_child(to_delete))
                    it->left = nullptr;
                else 
                    it->right = nullptr;
            }
            else {
                it = nullptr;
            }

            delete to_delete;
        }
    }
};

}

#endif
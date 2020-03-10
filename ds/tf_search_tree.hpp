//////////
// TODO //
//////////

#ifndef TF_SEARCH_TREE_H
#define TF_SEARCH_TREE_H

#include <iostream> // print            |
#include "tf_linked_list.hpp" // print  | DEBUG

#include "tf_exception.hpp"

namespace tf {

template <class K, class V>
class search_tree {
private:
// CLASSES
    class node {
    public:
        K key;
        V value;
        int height;
        node *parent;
        node *left;
        node *right;

        node(const K &key, const V &value, node *parent = nullptr):
            key(key),
            value(value),
            height(1),
            parent(parent),
            left(nullptr),
            right(nullptr) {}
    };

// VARIABLES
    node *root;

// FUNCTIONS
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

    bool is_left_child(node *child) {
        if (child) {
            node *parent = child->parent;
            if (parent && parent->left && parent->left->key == child->key) {
                return true;
            }
        }

        return false;
    }

    node *left_rotation(node *n) {
        node *new_root = n->right;
        if (n == root)
            root = new_root;

        if (n->parent) {
            if (is_left_child(n))
                set_left(n->parent, new_root);
            else
                set_right(n->parent, new_root);
        }
        else {
            new_root->parent = nullptr;
        }
        n->parent = new_root;

        set_right(n, new_root->left);
        set_left(new_root, n);

        update_height(new_root->left);
        update_height(new_root->right);
        update_height(new_root);
        return new_root;
    }

    node *right_rotation(node *n) {
        node *new_root = n->left;
        if (n == root)
            root = new_root;

        if (n->parent) {
            if (is_left_child(n))
                set_left(n->parent, new_root);
            else
                set_right(n->parent, new_root);
        }
        else {
            new_root->parent = nullptr;
        }
        n->parent = new_root;

        set_left(n, new_root->right);
        set_right(new_root, n);

        update_height(new_root->left);
        update_height(new_root->right);
        update_height(new_root);
        return new_root;
    }

    void rebalance_upward(node *n) {
        node *it = n;
        while (it) {
            update_height(it);
            int balance = height(it->right) - height(it->left);
            if (balance > 1) {
                if (height(it->right->right) > height(it->right->left)) {
                    it = left_rotation(it);
                }
                else {
                    it->right = right_rotation(it->right);
                    it = left_rotation(it);
                }
            }
            else if (balance < -1) {
                if (height(it->left->left) > height(it->left->right)) {
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

    node *min_node(node *n) {
        node *it = n;
        while (true) {
            if (it->left) {
                it = it->left;
            }
            else {
                return it;
            }
        }
    }

    const V remove_leaf(node *to_delete) {
        const V result = to_delete->value;

        if (to_delete->parent) {
            if (is_left_child(to_delete)) {
                to_delete->parent->left = nullptr;
            }
            else {
                to_delete->parent->right = nullptr;
            }
        }
        else {
            root = nullptr;
        }

        delete to_delete;
        return result;
    }

    const V remove_single_parent(node *to_delete) {
        const V result = to_delete->value;

        node *new_root = (to_delete->left) ? to_delete->left : to_delete->right;
        if (to_delete->parent) {
            if (is_left_child(to_delete)) {
                set_left(to_delete->parent, new_root);
            }
            else {
                set_right(to_delete->parent, new_root);
            }
        }
        else {
            new_root->parent = nullptr;
            root = new_root;
        }

        delete to_delete;
        return result;
    }

    const V remove_double_parent(node *to_delete) {
        const V result = to_delete->value;

        node *succ = min_node(to_delete->right);
        to_delete->key = succ->key;
        to_delete->value = succ->value;

        if (succ->right) {
            remove_single_parent(succ);
        }
        else {
            remove_leaf(succ);
        }

        return result;
    }

public:
    search_tree():
        root(nullptr) {}

    search_tree(const K &key, const V &value):
        root(new node(key, value)) {}
    
    ~search_tree() {
        clear();
    }

    // https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm
    // https://wkdtjsgur100.github.io/avl-tree/

    // O(log(n))
    void insert(const K &key, const V &value) {
        if (empty()) {
            root = new node(key, value);
            return;
        }

        node *it = root;
        while (true) {
            if (key < it->key) {
                if (it->left) {
                    it = it->left;
                }
                else {
                    it->left = new node(key, value, it);
                    break;
                }
            }
            else if (key > it->key) {
                if (it->right) {
                    it = it->right;
                }
                else {
                    it->right = new node(key, value, it);
                    break;
                }
            }
            else { // if (value == it->value)
                throw tf::exception("search tree: insert: key already present");
            }
        }

        rebalance_upward(it);
    }

    const V remove(const K &key) {
        V result;

        node *it = root;
        while (it) {
            if (key < it->key) {
                it = it->left;
            }
            else if (key > it->key) {
                it = it->right;
            }
            else {
                node *to_delete = it;
                it = it->parent;

                if (to_delete->left && to_delete->right)
                    result = remove_double_parent(to_delete);
                else if (to_delete->left || to_delete->right)
                    result = remove_single_parent(to_delete);
                else
                    result = remove_leaf(to_delete);

                if (it) {
                    update_height(it->left);
                    update_height(it->right);
                }

                rebalance_upward(it);
                return result;
            }
        }

        throw tf::exception("search tree: remove: invalid key");
    }

    const V &get(const K &key) {
        node *it = root;
        while (it) {
            if (key < it->key) {
                it = it->left;
            }
            else if (key > it->key) {
                it = it->right;
            }
            else {
                return it->value;
            }
        }

        throw tf::exception("search tree: get: invalid key");
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
            }
            else if (it->right) {
                it = it->right;
            }
            else {
                node *to_delete = it;
                it = it->parent;

                if (it) {
                    if (is_left_child(to_delete))
                        it->left = nullptr;
                    else 
                        it->right = nullptr;
                }

                delete to_delete;
            }
        }

        root = nullptr;
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
                    std::cout << (*it)->key  << " ";

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
};

}

#endif
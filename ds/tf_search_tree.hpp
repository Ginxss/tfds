#ifndef TF_SEARCH_TREE_H
#define TF_SEARCH_TREE_H

// #include <iostream> // print            |
// #include "tf_linked_list.hpp" // print  | DEBUG

#include "tf_exception.hpp"

namespace tf {

/*
* An ordered map (iterative implementation of an AVL tree).
* Can be used as a priority queue (TODO: Make Wrapper for that)
*/
template <typename K, typename V>
class search_tree {
private:
    struct node {
        K key;
        V value;
        int height;
        node *parent;
        node *left;
        node *right;
    };

    node *alloc_node(const K &key, const V &value, int height, node *parent, node *left, node *right) {
        node *n = (node *)malloc(sizeof(node));
        n->key = key;
        n->value = value;
        n->height = height;
        n->parent = parent;
        n->left = left;
        n->right = right;
        return n;
    }

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

    node *min_node(node *n) {
        node *it = n;
        while (it->left) {
            it = it->left;
        }

        return it;
    }

    node *max_node(node *n) {
        node *it = n;
        while (it->right) {
            it = it->right;
        }

        return it;
    }

    bool is_left_child(node *child) {
        if (child) {
            node *parent = child->parent;
            if (parent && parent->left && parent->left->key == child->key)
                return true;
        }

        return false;
    }

    node *successor(node *n) {
        if (n->right)
            return min_node(n->right);

        node *it = n;
        while (it->parent) {
            if (is_left_child(it)) {
                return it->parent;
            }

            it = it->parent;
        }

        return nullptr;
    }

    node *predecessor(node *n) {
        if (n->left)
            return max_node(n->left);

        node *it = n;
        while (it->parent) {
            if (!is_left_child(it)) {
                return it->parent;
            }

            it = it->parent;
        }
        
        return nullptr;
    }

public:
    class iterator {
    public:
        search_tree *tree;
        node *nd;

        V &operator*() { return nd->value; }
        K &key() { return nd->key; }
        V &value() { return nd->value; }
        void operator++() { nd = tree->successor(nd); }
        void operator--() { nd = tree->predecessor(nd); }
        bool condition() { return nd != nullptr; }
    };

private:
    // VARIABLES

    node *root;
    int size_;

    iterator start_it;
    iterator end_it;

    // METHODS

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

    void remove_leaf(node *to_delete) {
        if (to_delete->parent) {
            if (is_left_child(to_delete))
                to_delete->parent->left = nullptr;
            else
                to_delete->parent->right = nullptr;
        }
        else {
            root = nullptr;
        }

        free(to_delete);
    }

    void remove_single_parent(node *to_delete) {
        node *new_root = (to_delete->left) ? to_delete->left : to_delete->right;
        if (to_delete->parent) {
            if (is_left_child(to_delete))
                set_left(to_delete->parent, new_root);
            else
                set_right(to_delete->parent, new_root);
        }
        else {
            new_root->parent = nullptr;
            root = new_root;
        }

        free(to_delete);
    }

    void remove_double_parent(node *to_delete) {
        node *succ = successor(to_delete);
        
        to_delete->key = succ->key;
        to_delete->value = succ->value;

        if (succ->right)
            remove_single_parent(succ);
        else
            remove_leaf(succ);
    }

    // uses one of the three methods above to remove a node and return its value.
    const V remove_node(node *n) {
        const V result = n->value;
        node *parent = n->parent;

        if (n->left && n->right)
            remove_double_parent(n);
        else if (n->left || n->right)
            remove_single_parent(n);
        else
            remove_leaf(n);

        if (!empty()) {
            node *replacing = (parent) ? parent : root;
            update_height(replacing->left);
            update_height(replacing->right);
            rebalance_upward(replacing);
        }

        size_--;
        return result;
    }

public:
    search_tree():
        root(nullptr),
        size_(0)
    {
        start_it.tree = this;
        end_it.tree = this;
    }
    
    ~search_tree() {
        clear();
    }

    // O(log(n))
    void insert(const K &key, const V &value) {
        if (empty()) {
            root = alloc_node(key, value, 1, nullptr, nullptr, nullptr);
            return;
        }
        else {
            node *it = root;
            while (true) {
                if (key < it->key) {
                    if (it->left) {
                        it = it->left;
                    }
                    else {
                        it->left = alloc_node(key, value, 1, it, nullptr, nullptr);
                        break;
                    }
                }
                else if (key > it->key) {
                    if (it->right) {
                        it = it->right;
                    }
                    else {
                        it->right = alloc_node(key, value, 1, it, nullptr, nullptr);
                        break;
                    }
                }
                else { // if (value == it->value)
                    throw tf::exception("search tree: insert: key already present");
                }
            }

            rebalance_upward(it);
        }

        size_++;
    }

    // O(log(n))
    const V remove(const K &key) {
        node *it = root;
        while (it) {
            if (key < it->key) {
                it = it->left;
            }
            else if (key > it->key) {
                it = it->right;
            }
            else {
                return remove_node(it);
            }
        }

        throw tf::exception("search tree: remove: key not found");
    }

    // O(log(n))
    const V &get(const K &key) const {
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

        throw tf::exception("search tree: get: key not found");
    }

    // O(log(n))
    V &operator[](const K &key) {
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

        throw tf::exception("search tree: []: key not found");
    }

    // O(log(n))
    iterator begin() {
        node *it = root;
        if (it) {
            while (it->left) {
                it = it->left;
            }
        }

        start_it.nd = it;
        return start_it;
    }

    // O(log(n))
    iterator end() {
        node *it = root;
        if (it) {
            while (it->right) {
                it = it->right;
            }
        }

        end_it.nd = it;
        return end_it;
    }

    // O(log(n))
    V pop_min() {
        if (empty())
            throw tf::exception("search tree: pop_min: tree is empty");

        node *it = root;
        while (it->left) {
            it = it->left;
        }
        
        return remove_node(it);
    }

    // O(log(n))
    V pop_max() {
        if (empty())
            throw tf::exception("search tree: pop_max: tree is empty");
        
        node *it = root;
        while (it->right) {
            it = it->right;
        }

        return remove_node(it);
    }

    // O(log(n))
    bool contains(const K &key) const {
        node *it = root;
        while (it) {
            if (key < it->key) {
                it = it->left;
            }
            else if (key > it->key) {
                it = it->right;
            }
            else {
                return true;
            }
        }

        return false;
    }

    // O(1)
    int height() const {
        return height(root);
    }

    // O(1)
    int size() const {
        return size_;
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
                else {
                    root = nullptr;
                }

                free(to_delete);
            }
        }
    }

    // DEBUG
    /*void print() const {
        tf::linked_list<node *> list;
        list.add_back(root);

        tf::linked_list<node *> next_level;

        while (!list.empty()) {
            // print level
            for (auto it = list.begin(); it.condition(); ++it) {
                if ((*it)) {
                    std::cout << (*it)->key  << "/" << (*it)->value << " ";

                    // add next level
                    next_level.add_back((*it)->left);
                    next_level.add_back((*it)->right);
                }
                else {
                    std::cout << "_/_ ";
                }
            }
            std::cout << std::endl;

            // transfer next level
            list.clear();
            for (auto it = next_level.begin(); it.condition(); ++it) {
                list.add_back(*it);
            }
            next_level.clear();
        }
    }*/
};

}

#endif
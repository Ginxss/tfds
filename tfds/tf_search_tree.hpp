#ifndef TF_SEARCH_TREE_H
#define TF_SEARCH_TREE_H

#include <algorithm> // std::swap
#include "tf_exception.hpp"

namespace tf {

/*
* Ordered map (iterative implementation of an AVL tree).
*/
template <typename K, typename V>
class search_tree {
private:
    struct node {
        K key;
        V value;
        size_t height;
        node *parent;
        node *left;
        node *right;

        node(const K &key, const V &value, const size_t height, node *parent, node *left, node *right):
            key(key), value(value), height(height), parent(parent), left(left), right(right) {}
    };

    node *new_node(const K &key, const V &value, node *parent) {
        node *n = new node(key, value, 1, parent, nullptr, nullptr);
        ++size_;
        return n;
    }

    void delete_node(node *n) {
        --size_;
        delete n;
    }

    int node_height(node *n) const {
        return (n) ? n->height : 0;
    }

    void update_height(node *n) {
        if (n) {
            size_t left_height = node_height(n->left);
            size_t right_height = node_height(n->right);
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

    node *min_node(node *n) const {
        node *it = n;
        while (it->left) {
            it = it->left;
        }

        return it;
    }

    node *max_node(node *n) const {
        node *it = n;
        while (it->right) {
            it = it->right;
        }

        return it;
    }

    bool is_left_child(node *child) const {
        if (child) {
            node *parent = child->parent;
            if (parent && parent->left && parent->left->key == child->key)
                return true;
        }

        return false;
    }

    node *successor(node *n) const {
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

    node *predecessor(node *n) const {
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

    size_t size_;
    node *root;

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
            int balance = node_height(it->right) - node_height(it->left);
            if (balance > 1) {
                if (node_height(it->right->right) > node_height(it->right->left)) {
                    it = left_rotation(it);
                    return;
                }
                else {
                    it->right = right_rotation(it->right);
                    it = left_rotation(it);
                    return;
                }
            }
            else if (balance < -1) {
                if (node_height(it->left->left) > node_height(it->left->right)) {
                    it = right_rotation(it);
                    return;
                }
                else {
                    it->left = left_rotation(it->left);
                    it = right_rotation(it);
                    return;
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

        delete_node(to_delete);
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

        delete_node(to_delete);
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

        return result;
    }

public:
    // constructor
    search_tree():
        size_(0),
        root(nullptr)
    {
        start_it.tree = this;
        end_it.tree = this;
    }

    // copy constructor
    search_tree(const search_tree &other):
        size_(0),
        root(nullptr)
    {
        start_it.tree = this;
        end_it.tree = this;

        // mimic iterator
        node *it = other.root;
        if (it) {
            while (it->left) {
                it = it->left;
            }
        }

        while (it != nullptr) {
            insert(it->key, it->value);
            it = successor(it);
        }
    }
    
    // destructor
    ~search_tree() {
        clear();
    }

    friend void swap(search_tree &first, search_tree &second) noexcept {
        using std::swap;
        swap(first.size_, second.size_);
        swap(first.root, second.root);
        swap(first.start_it, second.start_it);
        swap(first.end_it, second.end_it);
    }

    // copy assignment operator
    search_tree &operator=(search_tree other) {
        swap(*this, other);
        return *this;
    }

    // move constructor
    search_tree(search_tree &&other) noexcept : search_tree() {
        swap(*this, other);
    }

    // move assignment operator
    search_tree &operator=(search_tree &&other) {
        swap(*this, other);
        return *this;
    }

    // O(log(n))
    void insert(const K &key, const V &value) {
        if (empty()) {
            root = new_node(key, value, nullptr);
        }
        else {
            node *it = root;
            while (true) {
                if (key < it->key) {
                    if (it->left) {
                        it = it->left;
                    }
                    else {
                        it->left = new_node(key, value, it);
                        break;
                    }
                }
                else if (key > it->key) {
                    if (it->right) {
                        it = it->right;
                    }
                    else {
                        it->right = new_node(key, value, it);
                        break;
                    }
                }
                else { // if (key == it->key)
                    throw exception("search tree: insert: key already exists");
                }
            }

            rebalance_upward(it);
        }
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

        throw exception("search tree: remove: key not found");
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

        throw exception("search tree: get: key not found");
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

        throw exception("search tree: []: key not found");
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
            throw exception("search tree: pop_min: tree is empty");

        node *it = root;
        while (it->left) {
            it = it->left;
        }
        
        return remove_node(it);
    }

    // O(log(n))
    V pop_max() {
        if (empty())
            throw exception("search tree: pop_max: tree is empty");
        
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
    size_t height() const {
        return node_height(root);
    }

    // O(1)
    size_t size() const {
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

                delete_node(to_delete);
            }
        }
    }
};

}

#endif
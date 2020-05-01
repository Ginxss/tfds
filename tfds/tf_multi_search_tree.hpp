#ifndef TF_MULTI_SEARCH_TREE_H
#define TF_MULTI_SEARCH_TREE_H

#include "tf_exception.hpp"

namespace tf {

/*
* Ordered multimap (iterative implementation of an AVL tree, every node has a list of values).
*/
template <typename K, typename V>
class multi_search_tree {
private:
    // VALUE NODE

    struct value_node {
        V value;
        value_node *next;

        value_node(const V &value_) : value(value_) {}
    };

    value_node *alloc_value_node(const V &value, value_node *next) {
        value_node *vn = new value_node(value);
        vn->next = next;
        ++size_;
        return vn;
    }

    void free_value_node(value_node *vn) {
        --size_;
        delete vn;
    }

    void free_value_nodes(value_node *vn) {
        while (vn) {
            value_node *to_delete = vn;
            vn = vn->next;
            free_value_node(to_delete);
        }
    }

    // NODE

    struct node {
        K key;
        value_node *start_value;
        int height;
        node *parent;
        node *left;
        node *right;

        node(const K &key_) : key(key_) {}
    };

    node *alloc_node(const K &key, const V &value, int height, node *parent, node *left, node *right) {
        node *n = new node(key);
        n->start_value = alloc_value_node(value, nullptr);
        n->height = height;
        n->parent = parent;
        n->left = left;
        n->right = right;
        return n;
    }

    void free_node(node *n) {
        delete n;
    }

    int node_height(node *n) const {
        return (n) ? n->height : 0;
    }

    void update_height(node *n) {
        if (n) {
            int left_height = node_height(n->left);
            int right_height = node_height(n->right);
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

    value_node *next_value_node(value_node *vn, node **n) {
        if (vn->next) {
            return vn->next;
        }

        *n = successor(*n);
        if (*n)
            return (*n)->start_value;
        
        return nullptr;
    }

    value_node *prev_value_node(value_node *vn, node **n) {
        if (vn->next) {
            return vn->next;
        }

        *n = predecessor(*n);
        if (*n)
            return (*n)->start_value;
        
        return nullptr;
    }

public:
    class iterator {
    public:
        multi_search_tree *tree;
        value_node *vn;
        node *nd;

        V &operator*() { return vn->value; }
        K &key() { return nd->key; }
        V &value() { return vn->value; }
        void operator++() { vn = tree->next_value_node(vn, &nd); }
        void operator--() { vn = tree->prev_value_node(vn, &nd); }
        bool condition() { return nd != nullptr; }
    };

private:
    // VARIABLES

    int size_;
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

    // i dont think 2 cases can happen after 1 insert, and testing supports that. if it makes problems, just remove the return statements.
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

    void remove_leaf(node *to_delete, bool delete_values = true) {
        if (to_delete->parent) {
            if (is_left_child(to_delete))
                to_delete->parent->left = nullptr;
            else
                to_delete->parent->right = nullptr;
        }
        else {
            root = nullptr;
        }

        if (delete_values)
            free_value_nodes(to_delete->start_value);
        free_node(to_delete);
    }

    void remove_single_parent(node *to_delete, bool delete_values = true) {
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

        if (delete_values)
            free_value_nodes(to_delete->start_value);
        free_node(to_delete);
    }

    void remove_double_parent(node *to_delete) {
        node *succ = successor(to_delete);
        
        to_delete->key = succ->key;
        free_value_nodes(to_delete->start_value);
        to_delete->start_value = succ->start_value;

        if (succ->right)
            remove_single_parent(succ, false);
        else
            remove_leaf(succ, false);
    }

    // uses one of the three methods above to remove a node and return its value.
    void remove_node(node *n) {
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
    }

public:
    multi_search_tree():
        size_(0),
        root(nullptr)
    {
        start_it.tree = this;
        end_it.tree = this;
    }
    
    ~multi_search_tree() {
        clear();
    }

    // O(log(n))
    void insert(const K &key, const V &value) {
        if (empty()) {
            root = alloc_node(key, value, 1, nullptr, nullptr, nullptr);
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
                        rebalance_upward(it);
                        break;
                    }
                }
                else if (key > it->key) {
                    if (it->right) {
                        it = it->right;
                    }
                    else {
                        it->right = alloc_node(key, value, 1, it, nullptr, nullptr);
                        rebalance_upward(it);
                        break;
                    }
                }
                else { // if (key == it->key)
                    it->start_value = alloc_value_node(value, it->start_value);
                    break;
                }
            }
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
                const V result = it->start_value->value;
                remove_node(it);
                return result;
            }
        }

        throw tf::exception("search tree: remove: key not found");
    }

    // O(log(n) + #values with that key)
    const V remove_value(const K &key, const V &value) {
        node *it = root;
        while (it) {
            if (key < it->key) {
                it = it->left;
            }
            else if (key > it->key) {
                it = it->right;
            }
            else {
                value_node *value_it = it->start_value;
                value_node *prev = nullptr;
                while (value_it) {
                    if (value_it->value == value) {
                        const V result = value_it->value;

                        if (prev) {
                            prev->next = value_it->next;
                        }
                        else {
                            it->start_value = value_it->next;

                            if (it->start_value == nullptr) {
                                remove_node(it);
                            }
                        }

                        free_value_node(value_it);
                        return result;
                    }

                    prev = value_it;
                    value_it = value_it->next;
                }

                throw tf::exception("search tree: remove_value: value not found");
            }
        }

        throw tf::exception("search tree: remove_value: key not found");
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
                return it->start_value->value;
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
                return it->start_value->value;
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
        start_it.vn = it->start_value;
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
        end_it.vn = it->start_value;
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

        const V result = it->start_value->value;

        value_node *to_delete = it->start_value;
        it->start_value = to_delete->next;
        free_value_node(to_delete);

        if (it->start_value == nullptr) {
            remove_node(it);
        }

        return result;
    }

    // O(log(n))
    V pop_max() {
        if (empty())
            throw tf::exception("search tree: pop_max: tree is empty");
        
        node *it = root;
        while (it->right) {
            it = it->right;
        }

        const V result = it->start_value->value;

        value_node *to_delete = it->start_value;
        it->start_value = to_delete->next;
        free_value_node(to_delete);

        if (it->start_value == nullptr) {
            remove_node(it);
        }

        return result;
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
        return node_height(root);
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

                free_value_nodes(to_delete->start_value);
                free_node(to_delete);
            }
        }
    }
};

}

#endif
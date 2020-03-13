////////////
// USABLE //
////////////

#ifndef TF_SEARCH_TREE_H
#define TF_SEARCH_TREE_H

#include <iostream> // print            |
#include "tf_linked_list.hpp" // print  | DEBUG

#include "tf_exception.hpp"

namespace tf {

/*
* An ordered map (iterative implementation of an AVL tree).
*/
template <typename K, typename V>
class search_tree {
private:
    // NODE
    
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

    // VARIABLES

    node *root;

    // METHODS    

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
            if (parent && parent->left && parent->left->key == child->key)
                return true;
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

    const V remove_leaf(node *to_delete) {
        const V result = to_delete->value;

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
        return result;
    }

    const V remove_single_parent(node *to_delete) {
        const V result = to_delete->value;

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
        return result;
    }

    const V remove_double_parent(node *to_delete) {
        const V result = to_delete->value;

        node *succ = to_delete->right;
        while (succ->left) {
            succ = succ->left;
        }

        to_delete->key = succ->key;
        to_delete->value = succ->value;

        if (succ->right)
            remove_single_parent(succ);
        else
            remove_leaf(succ);

        return result;
    }

public:
    search_tree():
        root(nullptr) {}
    
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
    }

    // O(log(n))
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
                    rebalance_upward(it);
                }
                else {
                    root = nullptr;
                }

                return result;
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

    // average: O(1) - worst: O(n)
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
    void print() const {
        tf::linked_list<node *> list;
        list.add(root);

        tf::linked_list<node *> next_level;

        while (!list.empty()) {
            // print level
            for (auto it = list.begin(); it.condition(); ++it) {
                if ((*it)) {
                    std::cout << (*it)->key  << "/" << (*it)->value << " ";

                    // add next level
                    next_level.add((*it)->left);
                    next_level.add((*it)->right);
                }
                else {
                    std::cout << "_/_ ";
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
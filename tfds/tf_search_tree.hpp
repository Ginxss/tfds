#ifndef TF_SEARCH_TREE_H
#define TF_SEARCH_TREE_H

#include <algorithm> // std::swap
#include "utils/tf_exception.hpp"
#include "utils/tf_compare_functions.hpp"

namespace tf {

/*
* Ordered map (iterative implementation of an AVL tree).
*/
template <typename K, typename V>
class search_tree {
private:
    // VALUE BUCKET

    struct value_bucket {
        V value;
        value_bucket *next;

        value_bucket(const V &value, value_bucket *next):
            value(value), next(next) {}
    };

    // NODE

    struct node {
        K key;
        value_bucket *bucket;
        size_t height;
        node *parent;
        node *left;
        node *right;

        node(const K &key, value_bucket *bucket, const size_t height, node *parent, node *left, node *right):
            key(key), bucket(bucket), height(height), parent(parent), left(left), right(right) {}
    };

    // ITERATORS

    class iterator {
    private:
        search_tree *tree;
        value_bucket *current_bucket;
        node *current_node;

        void next_bucket() {
            if (current_bucket->next) {
                current_bucket = current_bucket->next;
                return;
            }

            current_node = tree->successor(current_node);
            if (current_node)
                current_bucket = current_node->bucket;
            else
                current_bucket = nullptr;
        }

        void prev_bucket() {
            if (current_bucket->next) {
                current_bucket = current_bucket->next;
                return;
            }

            current_node = tree->predecessor(current_node);
            if (current_node)
                current_bucket = current_node->bucket;
            else
                current_bucket = nullptr;
        }
    
    public:
        iterator(search_tree *tree, const bool forward):
            tree(tree)
        {
            if (forward)
                current_node = tree->min_node(tree->root);
            else
                current_node = tree->max_node(tree->root);
            
            if (current_node)
                current_bucket = current_node->bucket;
            else
                current_bucket = nullptr;
        }

        const K &key() const { return current_node->key; }
        V &operator*() { return current_bucket->value; }
        V &value() { return current_bucket->value; }
        void operator++() { next_bucket(); }
        void operator--() { prev_bucket(); }
        bool condition() const { return current_bucket != nullptr; }
    };

    class const_iterator {
    private:
        const search_tree *tree;
        value_bucket *current_bucket;
        node *current_node;

        void next_bucket() {
            if (current_bucket->next) {
                current_bucket = current_bucket->next;
                return;
            }

            current_node = tree->successor(current_node);
            if (current_node)
                current_bucket = current_node->bucket;
            else
                current_bucket = nullptr;
        }

        void prev_bucket() {
            if (current_bucket->next) {
                current_bucket = current_bucket->next;
                return;
            }

            current_node = tree->predecessor(current_node);
            if (current_node)
                current_bucket = current_node->bucket;
            else
                current_bucket = nullptr;
        }
    
    public:
        const_iterator(const search_tree *tree, const bool forward):
            tree(tree)
        {
            if (forward)
                current_node = tree->min_node(tree->root);
            else
                current_node = tree->max_node(tree->root);
            
            if (current_node)
                current_bucket = current_node->bucket;
            else
                current_bucket = nullptr;
        }

        const K &key() const { return current_node->key; }
        const V &operator*() const { return current_bucket->value; }
        const V &value() const { return current_bucket->value; }
        void operator++() { next_bucket(); }
        void operator--() { prev_bucket(); }
        bool condition() const { return current_bucket != nullptr; }
    };


    // VARIABLES

    size_t size_;
    bool allow_duplicate_keys;
    node *root;

    // METHODS

    value_bucket *create_value_bucket(const V &value, value_bucket *next) {
        value_bucket *b = new value_bucket(value, next);
        ++size_;
        return b;
    }

    void destroy_value_bucket(value_bucket *b) {
        --size_;
        delete b;
    }

    void destroy_all_value_buckets(value_bucket *b) {
        while (b) {
            value_bucket *to_delete = b;
            b = b->next;
            destroy_value_bucket(to_delete);
        }
    }

    node *create_node(const K &key, const V &value, node *parent) {
        node *n = new node(key, create_value_bucket(value, nullptr), 1, parent, nullptr, nullptr);
        return n;
    }

    void destroy_node(node *n, const bool delete_values = true) {
        if (delete_values) {
            destroy_all_value_buckets(n->bucket);
        }

        delete n;
    }

    size_t node_height(node *n) const {
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

    bool is_left_child(node *child) const {
        node *parent = child->parent;
        if (parent->left && equals<K>(parent->left->key, child->key))
            return true;

        return false;
    }

    node *left_rotation(node *n) {
        node *replacing = n->right;

        if (n->parent) {
            if (is_left_child(n))
                set_left(n->parent, replacing);
            else
                set_right(n->parent, replacing);
        }
        else {
            replacing->parent = nullptr;
            root = replacing;
        }

        set_right(n, replacing->left);
        set_left(replacing, n);

        update_height(replacing->left);
        update_height(replacing->right);
        update_height(replacing);
        return replacing;
    }

    node *right_rotation(node *n) {
        node *replacing = n->left;

        if (n->parent) {
            if (is_left_child(n))
                set_left(n->parent, replacing);
            else
                set_right(n->parent, replacing);
        }
        else {
            replacing->parent = nullptr;
            root = replacing;
        }

        set_left(n, replacing->right);
        set_right(replacing, n);

        update_height(replacing->left);
        update_height(replacing->right);
        update_height(replacing);
        return replacing;
    }

    void rebalance_upward(node *n) {
        node *it = n;
        while (it) {
            ssize_t balance = node_height(it->right) - node_height(it->left);
            if (balance > 1) {
                if (node_height(it->right->right) > node_height(it->right->left)) {
                    it = left_rotation(it);
                }
                else {
                    it->right = right_rotation(it->right);
                    it = left_rotation(it);
                }
            }
            else if (balance < -1) {
                if (node_height(it->left->left) > node_height(it->left->right)) {
                    it = right_rotation(it);
                }
                else {
                    it->left = left_rotation(it->left);
                    it = right_rotation(it);
                }
            }

            update_height(it);
            it = it->parent;
        }
    }

    node *min_node(node *n) const {
        node *it = n;
        if (it) {
            while (it->left) {
                it = it->left;
            }
        }

        return it;
    }

    node *max_node(node *n) const {
        node *it = n;
        if (it) {
            while (it->right) {
                it = it->right;
            }
        }

        return it;
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

    void remove_leaf(node *to_delete) {
        if (to_delete->parent) {
            if (is_left_child(to_delete))
                to_delete->parent->left = nullptr;
            else
                to_delete->parent->right = nullptr;
            
            rebalance_upward(to_delete->parent);
        }
        else {
            root = nullptr;
            rebalance_upward(root);
        }

        destroy_node(to_delete);
    }

    void remove_single_parent(node *to_delete) {
        node *replacing = (to_delete->left) ? to_delete->left : to_delete->right;
        if (to_delete->parent) {
            if (is_left_child(to_delete))
                set_left(to_delete->parent, replacing);
            else
                set_right(to_delete->parent, replacing);
        }
        else {
            replacing->parent = nullptr;
            root = replacing;
        }

        rebalance_upward(replacing);
        destroy_node(to_delete);
    }

    void remove_double_parent(node *to_delete) {
        node *succ = successor(to_delete);
        
        using std::swap;
        swap(to_delete->key, succ->key);
        swap(to_delete->bucket, succ->bucket);

        if (succ->right)
            remove_single_parent(succ);
        else
            remove_leaf(succ);
    }

    void remove_node(node *n) {
        if (n->left && n->right)
            remove_double_parent(n);
        else if (n->left || n->right)
            remove_single_parent(n);
        else
            remove_leaf(n);
    }

public:
    // constructor
    search_tree(const bool allow_duplicate_keys = false):
        size_(0),
        allow_duplicate_keys(allow_duplicate_keys),
        root(nullptr) {}

    // copy constructor
    search_tree(const search_tree &other):
        size_(0),
        allow_duplicate_keys(other.allow_duplicate_keys),
        root(nullptr)
    {
        for (auto it = other.begin(); it.condition(); ++it) {
            insert(it.key(), it.value());
        }
    }
    
    // destructor
    ~search_tree() {
        clear();
    }

    friend void swap(search_tree &first, search_tree &second) noexcept {
        using std::swap;
        swap(first.size_, second.size_);
        swap(first.allow_duplicate_keys, second.allow_duplicate_keys);
        swap(first.root, second.root);
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
            root = create_node(key, value, nullptr);
        }
        else {
            node *it = root;
            while (true) {
                if (less_than<K>(key, it->key)) {
                    if (it->left) {
                        it = it->left;
                    }
                    else {
                        it->left = create_node(key, value, it);
                        break;
                    }
                }
                else if (greater_than<K>(key, it->key)) {
                    if (it->right) {
                        it = it->right;
                    }
                    else {
                        it->right = create_node(key, value, it);
                        break;
                    }
                }
                else { // if (key == it->key)
                    if (!allow_duplicate_keys) {
                        throw exception("search tree: insert: key already exists");
                    }

                    it->bucket = create_value_bucket(value, it->bucket);
                    return;
                }
            }

            rebalance_upward(it);
        }
    }

    // O(log(n))
    V remove(const K &key) {
        node *it = root;
        while (it) {
            if (less_than<K>(key, it->key)) {
                it = it->left;
            }
            else if (greater_than<K>(key, it->key)) {
                it = it->right;
            }
            else {
                value_bucket *to_delete = it->bucket;
                V result = to_delete->value;

                if (to_delete->next) {
                    it->bucket = to_delete->next;
                    destroy_value_bucket(to_delete);
                }
                else {
                    remove_node(it);
                }

                return result;
            }
        }

        throw exception("search tree: remove: key not found");
    }

    // O(log(n))
    V remove_all(const K &key) {
        node *it = root;
        while (it) {
            if (less_than<K>(key, it->key)) {
                it = it->left;
            }
            else if (greater_than<K>(key, it->key)) {
                it = it->right;
            }
            else {
                V result = it->bucket->value;
                remove_node(it);
                return result;
            }
        }

        throw exception("search tree: remove: key not found");
    }

    // O(log(n) + #values with that key)
    V remove_value(const K &key, const V &value) {
        node *it = root;
        while (it) {
            if (less_than<K>(key, it->key)) {
                it = it->left;
            }
            else if (greater_than<K>(key, it->key)) {
                it = it->right;
            }
            else {
                value_bucket *bucket_it = it->bucket;
                value_bucket *prev = nullptr;
                while (bucket_it) {
                    if (equals<V>(value, bucket_it->value)) {
                        V result = bucket_it->value;

                        if (prev) {
                            prev->next = bucket_it->next;
                            destroy_value_bucket(bucket_it);
                        }
                        else {
                            if (bucket_it->next) {
                                it->bucket = bucket_it->next;
                                destroy_value_bucket(bucket_it);
                            }
                            else {
                                remove_node(it);
                            }
                        }

                        return result;
                    }

                    prev = bucket_it;
                    bucket_it = bucket_it->next;
                }

                throw exception("search tree: remove_value: value not found");
            }
        }

        throw exception("search tree: remove_value: key not found");
    }

    // O(log(n))
    const V &get(const K &key) const {
        node *it = root;
        while (it) {
            if (less_than<K>(key, it->key)) {
                it = it->left;
            }
            else if (greater_than<K>(key, it->key)) {
                it = it->right;
            }
            else {
                return it->bucket->value;
            }
        }

        throw exception("search tree: get: key not found");
    }

    // O(log(n))
    V &operator[](const K &key) {
        node *it = root;
        while (it) {
            if (less_than<K>(key, it->key)) {
                it = it->left;
            }
            else if (greater_than<K>(key, it->key)) {
                it = it->right;
            }
            else {
                return it->bucket->value;
            }
        }

        throw exception("search tree: []: key not found");
    }

    // O(log(n))
    const V &operator[](const K &key) const {
        node *it = root;
        while (it) {
            if (less_than<K>(key, it->key)) {
                it = it->left;
            }
            else if (greater_than<K>(key, it->key)) {
                it = it->right;
            }
            else {
                return it->bucket->value;
            }
        }

        throw exception("search tree: []: key not found");
    }

    // O(log(n))
    V &min() {
        if (empty())
            throw exception("search tree: min: tree is empty");

        return min_node(root)->bucket->value;
    }

    // O(log(n))
    const V &min() const {
        if (empty())
            throw exception("search tree: min: tree is empty");

        return min_node(root)->bucket->value;
    }

    // O(log(n))
    V &max() {
        if (empty())
            throw exception("search tree: max: tree is empty");

        return max_node(root)->bucket->value;
    }

    // O(log(n))
    const V &max() const {
        if (empty())
            throw exception("search tree: max: tree is empty");

        return max_node(root)->bucket->value;
    }

    // O(log(n))
    V pop_min() {
        if (empty())
            throw exception("search tree: pop_min: tree is empty");

        node *n = min_node(root);
        value_bucket *to_delete = n->bucket;
        V result = to_delete->value;

        if (to_delete->next) {
            n->bucket = to_delete->next;
            destroy_value_bucket(to_delete);
        }
        else {
            remove_node(n);
        }
        
        return result;
    }

    // O(log(n))
    V pop_max() {
        if (empty())
            throw exception("search tree: pop_max: tree is empty");
        
        node *n = max_node(root);
        value_bucket *to_delete = n->bucket;
        V result = to_delete->value;

        if (to_delete->next) {
            n->bucket = to_delete->next;
            destroy_value_bucket(to_delete);
        }
        else {
            remove_node(n);
        }
        
        return result;
    }

    // O(log(n))
    iterator begin() {
        return iterator(this, true);
    }

    // O(log(n))
    const_iterator begin() const {
        return const_iterator(this, true);
    }

    // O(log(n))
    iterator end() {
        return iterator(this, false);
    }

    // O(log(n))
    const_iterator end() const {
        return const_iterator(this, false);
    }

    // O(log(n))
    bool contains(const K &key) const {
        node *it = root;
        while (it) {
            if (less_than<K>(key, it->key)) {
                it = it->left;
            }
            else if (greater_than<K>(key, it->key)) {
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

    // O(1)
    bool allows_duplicate_keys() const {
        return allow_duplicate_keys;
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

                destroy_node(to_delete);
            }
        }
    }

    // DEBUG
    /* void print() const {
        tf::vector<node*> level;
        tf::vector<node*> next_level;

        level.add(root);

        while (true) {
            std::cout << "|";

            for (int i = 0; i < level.size(); ++i) {
                node *it = level[i];
                if (it) {
                    value_bucket *b = it->bucket;
                    std::cout << " (" << it->height << ") " << b->value;

                    while (b->next) {
                        std::cout << "," << b->next->value;
                        b = b->next;
                    }

                    next_level.add(it->left);
                    next_level.add(it->right);
                }
                else {
                    std::cout << " -";
                    next_level.add(nullptr);
                    next_level.add(nullptr);
                }

                std::cout << " |";
            }

            std::cout << std::endl;

            bool cancel = true;
            for (int i = 0; i < next_level.size(); ++i) {
                if (next_level[i] != nullptr) {
                    cancel = false;
                    break;
                }
            }
            if (cancel) {
                break;
            }

            level = next_level;
            next_level.clear();
        }

        std::cout << std::endl;
    } */
};

}

#endif
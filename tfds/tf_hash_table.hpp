#ifndef TF_HASH_TABLE_H
#define TF_HASH_TABLE_H

#include <algorithm> // std::swap
#include "utils/tf_exception.hpp"
#include "utils/tf_compare_functions.hpp"

namespace tf {

// src: http://www.cse.yorku.ca/~oz/hash.html
// the actual djb2 algorithm which iterates over a string until a '0' is found
inline unsigned long string_hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

// a modified version that iterates over a fixed number of bytes
inline unsigned long byte_hash(unsigned char *bytes, int length) {
    unsigned long hash = 5381;
    int c;

    int count = 0;
    while (count++ < length) {
        c = *bytes++;
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

template <typename K>
inline unsigned long hash(const K &key) {
    const size_t key_size = sizeof(K);
    unsigned char bytes[key_size];
    std::memcpy(bytes, &key, key_size);
    return byte_hash(bytes, key_size);
}

template <>
inline unsigned long hash<std::string>(const std::string &key) {
    return string_hash((unsigned char *)key.c_str());
}

template <>
inline unsigned long hash<const char *>(const char * const &key) {
    return string_hash((unsigned char *)key);
}

template <>
inline unsigned long hash<char *>(char * const &key) {
    return string_hash((unsigned char *)key);
}

/*
* Unordered map (separate chaining hash map).
*/
template <typename K, typename V>
class hash_table {
private:
    // BUCKET 

    struct bucket {
        K key;
        V value;
        bucket *next;

        bucket(const K &key, const V &value, bucket *next):
            key(key), value(value), next(next) {}
    };

    bucket *create_bucket(const K &key, const V &value, bucket *next) {
        bucket *b = new bucket(key, value, next);
        ++size_;
        return b;
    }

    void destroy_bucket(bucket *b) {
        --size_;
        delete b;
    }

    void destroy_all_buckets(bucket *b) {
        while (b) {
            bucket *to_delete = b;
            b = b->next;
            destroy_bucket(to_delete);
        }
    }

    // VARIABLES

    size_t table_size_;
    size_t size_;
    bool check_duplicate_keys;
    bucket **buckets;

public:
    // ITERATORS

    class iterator {
    private:
        hash_table *table;
        size_t current_index;
        bucket *current_bucket;

        void next_bucket() {
            if (current_bucket->next) {
                current_bucket = current_bucket->next;
                return;
            }

            while (++current_index < table->table_size_) {
                if (table->buckets[current_index]) {
                    current_bucket = table->buckets[current_index];
                    return;
                }
            }

            current_bucket = nullptr;
        }

    public:
        iterator(hash_table *table):
            table(table), current_index(0), current_bucket(table->buckets[0])
        {
            while (!current_bucket && ++current_index < table->table_size_) {
                current_bucket = table->buckets[current_index];
            }
        }

        const K &key() const { return current_bucket->key; }
        V &operator*() { return current_bucket->value; }
        V &value() { return current_bucket->value; }
        void operator++() { next_bucket(); }
        bool condition() const { return current_bucket != nullptr; }
    };

    class const_iterator {
    private:
        const hash_table *table;
        size_t current_index;
        bucket *current_bucket;

        void next_bucket() {
            if (current_bucket->next) {
                current_bucket = current_bucket->next;
                return;
            }

            while (++current_index < table->table_size_) {
                if (table->buckets[current_index]) {
                    current_bucket = table->buckets[current_index];
                    return;
                }
            }

            current_bucket = nullptr;
        }

    public:
        const_iterator(const hash_table *table):
            table(table), current_index(0), current_bucket(table->buckets[0])
        {
            while (!current_bucket && ++current_index < table->table_size_) {
                current_bucket = table->buckets[current_index];
            }
        }

        const K &key() const { return current_bucket->key; }
        const V &operator*() const { return current_bucket->value; }
        const V &value() const { return current_bucket->value; }
        void operator++() { next_bucket(); }
        bool condition() const { return current_bucket != nullptr; }
    };

    // CLASS

    // constructor
    hash_table(const size_t table_size = 100, const bool check_duplicate_keys = true):
        table_size_((table_size > 0) ? table_size : 1),
        size_(0),
        check_duplicate_keys(check_duplicate_keys),
        buckets(new bucket*[table_size_])
    {
        for (size_t i = 0; i < table_size_; ++i) {
            buckets[i] = nullptr;
        }
    }

    // copy constructor
    hash_table(const hash_table &other):
        table_size_(other.table_size_),
        size_(0),
        check_duplicate_keys(other.check_duplicate_keys),
        buckets(new bucket*[table_size_])
    {
        for (size_t i = 0; i < table_size_; ++i) {
            buckets[i] = nullptr;

            bucket *b = other.buckets[i];
            while (b) {
                buckets[i] = create_bucket(b->key, b->value, buckets[i]);
                b = b->next;
            }
        }
    }

    // destructor
    ~hash_table() {
        clear();
        delete[] buckets;
    }

    friend void swap(hash_table &first, hash_table &second) noexcept {
        using std::swap;
        swap(first.table_size_, second.table_size_);
        swap(first.size_, second.size_);
        swap(first.check_duplicate_keys, second.check_duplicate_keys);
        swap(first.buckets, second.buckets);
    }

    // move constructor
    hash_table(hash_table &&other) noexcept : hash_table(1) {
        swap(*this, other);
    }

    // copy assignment operator
    hash_table &operator=(hash_table other) {
        swap(*this, other);
        return *this;
    }

    // average: O(1) / worst: O(n)
    void insert(const K &key, const V &value) {
        unsigned long index = hash<K>(key) % table_size_;

        if (buckets[index]) {
            if (check_duplicate_keys) {
                bucket *it = buckets[index];
                while (it) {
                    if (equals<K>(key, it->key)) {
                        throw exception("hash table: insert: key already exists");
                    }

                    it = it->next;
                }
            }

            buckets[index] = create_bucket(key, value, buckets[index]);
        }
        else {
            buckets[index] = create_bucket(key, value, nullptr);
        }
    }

    // average: O(1) / worst: O(n)
    V remove(const K &key) {
        unsigned long index = hash<K>(key) % table_size_;

        if (buckets[index]) {
            bucket *to_delete = nullptr;

            if (equals<K>(key, buckets[index]->key)) {
                to_delete = buckets[index];
                buckets[index] = to_delete->next;
            }
            else {
                bucket *it = buckets[index]->next;
                bucket *prev = buckets[index];
                while (it) {
                    if (equals<K>(key, it->key)) {
                        to_delete = it;
                        prev->next = it->next;
                        break;
                    }

                    prev = it;
                    it = it->next;
                }
            }

            if (to_delete) {
                V result = to_delete->value;
                destroy_bucket(to_delete);
                return result;
            }
        }

        throw exception("hash table: remove: key not found");
    }

    // average: O(1) / worst: O(n)
    const V &get(const K &key) const {
        unsigned long index = hash<K>(key) % table_size_;

        bucket *it = buckets[index];
        while (it) {
            if (equals<K>(key, it->key)) {
                return it->value;
            }

            it = it->next;
        }

        throw exception("hash table: get: key not found");
    }

    // average: O(1) / worst: O(n)
    V &operator[](const K &key) {
        unsigned long index = hash<K>(key) % table_size_;

        bucket *it = buckets[index];
        while (it) {
            if (equals<K>(key, it->key)) {
                return it->value;
            }

            it = it->next;
        }

        throw exception("hash table: []: key not found");
    }

    // average: O(1) / worst: O(n)
    const V &operator[](const K &key) const {
        unsigned long index = hash<K>(key) % table_size_;

        bucket *it = buckets[index];
        while (it) {
            if (equals<K>(key, it->key)) {
                return it->value;
            }

            it = it->next;
        }

        throw exception("hash table: []: key not found");
    }

    // average: O(1) / worst: O(n)
    bool contains(const K &key) const {
        unsigned long index = hash<K>(key) % table_size_;

        bucket *it = buckets[index];
        while (it) {
            if (equals<K>(key, it->key)) {
                return true;
            }

            it = it->next;
        }

        return false;
    }

    // O(n)
    void clear() {
        for (size_t i = 0; i < table_size_; ++i) {
            destroy_all_buckets(buckets[i]);
            buckets[i] = nullptr;
        }
    }

    // O(1)
    size_t table_size() const {
        return table_size_;
    }

    // O(1)
    size_t size() const  {
        return size_;
    }

    // O(1)
    bool checks_duplicate_keys() const {
        return check_duplicate_keys;
    }

    // O(1)
    bool empty() const {
        return size_ == 0;
    }

    iterator begin() {
        return iterator(this);
    }

    const_iterator begin() const {
        return const_iterator(this);
    }
};

}

#endif
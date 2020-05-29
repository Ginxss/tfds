#ifndef TF_HASH_TABLE_H
#define TF_HASH_TABLE_H

#include <string> // std::string
#include <cstring> // memcpy, memset
#include "tf_exception.hpp"

namespace tf {

// HASH

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
    memcpy(bytes, &key, key_size);
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

// COMPARE

template <typename K>
inline bool compare(const K &key1, const K &key2) {
    return key1 == key2;
}

template <>
inline bool compare<std::string>(const std::string &key1, const std::string &key2) {
    return key1.compare(key2) == 0;
}

template <>
inline bool compare<const char *>(const char * const &key1, const char * const &key2) {
    return strcmp(key1, key2) == 0;
}

template <>
inline bool compare<char *>(char * const &key1, char * const &key2) {
    return strcmp(key1, key2) == 0;
}

/*
* Unordered map (separate chaining hash map).
*/
template <typename K, typename V>
class hash_table {
private:
    struct bucket {
        K key;
        V value;
        bucket *next;

        bucket(const K &key_, const V &value_) : key(key_), value(value_) {}
    };

    bucket *alloc_bucket(const K &key, const V &value, bucket *next) {
        bucket *b = new bucket(key, value);
        b->next = next;
        ++size_;
        return b;
    }

    void free_bucket(bucket *b) {
        --size_;
        delete b;
    }

    void free_buckets(bucket *b) {
        while (b) {
            bucket *to_delete = b;
            b = b->next;
            free_bucket(to_delete);
        }
    }

    bucket *next_bucket(bucket *curr_b, size_t *curr_index) {
        if (curr_b->next) {
            return curr_b->next;
        }

        ++(*curr_index);
        while ((*curr_index) < table_size_) {
            bucket *b = *(buckets + (*curr_index)++);
            if (b) {
                return b;
            }
        }
        return nullptr;
    }

public:
    class iterator {
    public:
        hash_table *table;
        size_t index;
        bucket *b;

        V &operator*() { return b->value; }
        K &key() { return b->key; }
        V &value() { return b->value; }
        void operator++() { b = table->next_bucket(b, &index); }
        bool condition() { return b != nullptr; }
    };

private:
    size_t table_size_;
    size_t size_;
    bool allow_duplicate_keys_;
    bucket **buckets;

    iterator start_it;

public:
    hash_table(const size_t table_size = 100, const bool allow_duplicate_keys = false):
        table_size_(table_size),
        size_(0),
        allow_duplicate_keys_(allow_duplicate_keys),
        buckets((bucket **)malloc(table_size_ * sizeof(bucket *)))
    {
        memset(buckets, 0, table_size_ * sizeof(bucket *));
        start_it.table = this;
    }

    // O(n)
    hash_table(const hash_table &other):
        table_size_(other.table_size_),
        size_(0),
        allow_duplicate_keys_(other.allow_duplicate_keys_),
        buckets((bucket **)malloc(table_size_ * sizeof(bucket *)))
    {
        memset(buckets, 0, table_size_ * sizeof(bucket *));
        start_it.table = this;

        for (size_t i = 0; i < table_size_; ++i) {
            bucket *b = *(other.buckets + i);

            while (b) {
                *(buckets + i) = alloc_bucket(b->key, b->value, *(buckets + i));
                b = b->next;
            }
        }
    }

    // O(n)
    hash_table &operator=(const hash_table &other) {
        if (this == &other)
            return *this;

        for (size_t i = 0; i < table_size_; ++i) {
            free_buckets(*(buckets + i));
        }
        free(buckets);

        table_size_ = other.table_size_;
        size_ = 0;
        allow_duplicate_keys_ = other.allow_duplicate_keys_;
        buckets = (bucket **)malloc(table_size_ * sizeof(bucket *));
        memset(buckets, 0, table_size_ * sizeof(bucket *));

        for (size_t i = 0; i < table_size_; ++i) {
            bucket *b = *(other.buckets + i);

            while (b) {
                *(buckets + i) = alloc_bucket(b->key, b->value, *(buckets + i));
                b = b->next;
            }
        }

        return *this;
    }

    ~hash_table() {
        for (size_t i = 0; i < table_size_; ++i) {
            free_buckets(*(buckets + i));
        }

        free(buckets);
    }

    // average: O(1) / worst: O(n)
    void insert(const K &key, const V &value) {
        unsigned long index = hash<K>(key) % table_size_;

        if (*(buckets + index)) {
            bucket *it = *(buckets + index);

            if (allow_duplicate_keys_) {
                *(buckets + index) = alloc_bucket(key, value, *(buckets + index));
            }
            else {
                if (compare<K>(key, it->key))
                    throw tf::exception("hash table: insert: key already exists");
                
                while (it->next) {
                    it = it->next;
                    if (compare<K>(key, it->key))
                        throw tf::exception("hash table: insert: key already exists");
                }

                it->next = alloc_bucket(key, value, nullptr);
            }
        }
        else {
            *(buckets + index) = alloc_bucket(key, value, nullptr);
        }
    }

    // average: O(1) / worst: O(n)
    const V remove(const K &key) {
        unsigned long index = hash<K>(key) % table_size_;

        if (compare<K>(key, (*(buckets + index))->key)) {
            bucket *to_delete = *(buckets + index);
            *(buckets + index) = to_delete->next;

            V result = to_delete->value;
            free_bucket(to_delete);
            return result;
        }
        else {
            bucket *it = (*(buckets + index))->next;
            bucket *prev = *(buckets + index);
            while (it) {
                if (compare<K>(key, it->key)) {
                    prev->next = it->next;
                    
                    V result = it->value;
                    free_bucket(it);
                    return result;
                }

                prev = it;
                it = it->next;
            }
        }

        throw tf::exception("hash table: remove: key not found");
    }

    // average: O(1) / worst: O(n)
    const V &get(const K &key) const {
        unsigned long index = hash<K>(key) % table_size_;
        bucket *it = *(buckets + index);

        while (it) {
            if (compare<K>(key, it->key)) {
                return it->value;
            }

            it = it->next;
        }

        throw tf::exception("hash table: get: key not found");
    }

    // average: O(1) / worst: O(n)
    V &operator[](const K &key) {
        unsigned long index = hash<K>(key) % table_size_;
        bucket *it = *(buckets + index);
        
        while (it) {
            if (compare<K>(key, it->key)) {
                return it->value;
            }

            it = it->next;
        }

        throw tf::exception("hash table: []: key not found");
    }

    // average: O(1) / worst: O(n)
    bool contains(const K &key) const {
        unsigned long index = hash<K>(key) % table_size_;
        bucket *it = *(buckets + index);

        while (it) {
            if (compare<K>(key, it->key)) {
                return true;
            }

            it = it->next;
        }

        return false;
    }

    iterator begin() {
        start_it.index = 0;
        start_it.b = *buckets;

        while (!start_it.b) {
            ++start_it.index;
            start_it.b = *(buckets + start_it.index);
        }

        return start_it;
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
    bool allows_duplicate_keys() const {
        return allow_duplicate_keys_;
    }

    // O(1)
    bool empty() const {
        return size_ == 0;
    }

    // O(n)
    void clear() {
        for (size_t i = 0; i < table_size_; ++i) {
            free_buckets(*(buckets + i));
        }

        memset(buckets, 0, table_size_ * sizeof(bucket *));
    }
};

}

#endif
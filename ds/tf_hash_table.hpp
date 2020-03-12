//////////
// TODO //
//////////

#ifndef TF_HASH_TABLE_H
#define TF_HASH_TABLE_H

#include <cstring> // memcpy
#include <iostream> // DEBUG
#include "tf_exception.hpp"

namespace tf {

// src: http://www.cse.yorku.ca/~oz/hash.html
// the actual djb2 algorithm which iterates over a string until a '0' is found
unsigned long string_hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

// a modified version that iterates over a fixed number of bytes
unsigned long byte_hash(unsigned char *bytes, int length) {
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
unsigned long hash(const K &key) {
    int key_size = sizeof(K);
    unsigned char bytes[key_size];
    memcpy(bytes, &key, key_size);
    return byte_hash(bytes, key_size);
}

template <>
unsigned long hash<std::string>(const std::string &key) {
    return string_hash((unsigned char *)key.c_str());
}

template <>
unsigned long hash<const char *>(const char * const &key) {
    return string_hash((unsigned char *)key);
}

template <typename K, typename V>
class hash_table {
private:
// CLASSES

    struct bucket {
        K key;
        V value;
        bucket *next;
    };

// VARIABLES

    const int table_size;
    bucket **buckets;

// FUNCTIONS

    void free_bucket(bucket *b) {
        if (b) {
            free_bucket(b->next);
            free(b);
        }
    }

public:
    hash_table(const int table_size = 100):
        table_size(table_size),
        buckets((bucket **)malloc(table_size * sizeof(bucket *)))
    {
        for (int i = 0; i < table_size; ++i) {
            *(buckets + i) = nullptr;
        }
    }

    ~hash_table() {
        clear();
    }

    // average: O(1) - worst case: O(n)
    void insert(const K &key, const V &value) {
        unsigned long index = hash<K>(key) % table_size;
        bucket *new_bucket;

        if (*(buckets + index)) {
            bucket *it = *(buckets + index);
            while (it->next) {
                it = it->next;
            }

            it->next = (bucket *)malloc(sizeof(bucket));
            new_bucket = it->next;
        }
        else {
            *(buckets + index) = (bucket *)malloc(sizeof(bucket));
            new_bucket = *(buckets + index);
        }

        new_bucket->key = key;
        new_bucket->value = value;
        new_bucket->next = nullptr;
    }

    // average: O(1) - worst case: O(n)
    const V &get(const K &key) const {
        unsigned long index = hash<K>(key) % table_size;
        bucket *it = *(buckets + index);

        while (it) {
            if (key == it->key) {
                return it->value;
            }

            it = it->next;
        }

        throw tf::exception("hash table: get: invalid key");
    }

    // average: O(1) - worst case: O(n)
    V &operator[](const K &key) {
        unsigned long index = hash<K>(key) % table_size;
        bucket *it = *(buckets + index);
        
        while (it) {
            if (key == it->key) {
                return it->value;
            }

            it = it->next;
        }

        throw tf::exception("hash table: get: invalid key");
    }

    // O(n)
    void clear() {
        for (int i = 0; i < table_size; ++i) {
            free_bucket(*(buckets + i));
        }

        free(buckets);
        buckets = nullptr;
    }

    // DEBUG
    void print() {
        for (int i = 0; i < table_size; ++i) {
            std::cout << "bucket " << i << ": ";

            bucket *it = *(buckets + i);
            while (it) {
                std::cout << it->key->a << "/" << it->value << " ";
                it = it->next;
            }

            std::cout << std::endl;
        }
    }
};

}

#endif
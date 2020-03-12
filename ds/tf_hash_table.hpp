//////////
// TODO //
//////////

#ifndef TF_HASH_TABLE_H
#define TF_HASH_TABLE_H

#include <cstdlib> // malloc
#include <cstring> // strlen
#include <iostream>
#include "tf_exception.hpp"

namespace tf {

unsigned long byte_hash(const char *bytes, int length) {
    unsigned long result = 0;

    for (int i = 0; i < length; ++i) {
        result += (i+1) * bytes[i];
    }

    return result;
}

template <typename K>
unsigned long hash(const K &key) {
    // return byte_hash( (char *)( (void *)&key ) );
    return byte_hash(static_cast<const char*>(static_cast<const void *>(&key)), sizeof(K));
}

template <>
unsigned long hash<std::string>(const std::string &key) {
    return byte_hash(key.c_str(), key.length());
}

template <>
unsigned long hash<const char *>(const char * const &key) {
    return byte_hash(key, strlen(key));
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

    void free_bucket(bucket *b) {
        if (b) {
            free_bucket(b->next);
            free(b);
        }
    }

    // DEBUG
    void print() {
        for (int i = 0; i < table_size; ++i) {
            std::cout << "bucket " << i << ": ";

            bucket *it = *(buckets + i);
            while (it) {
                std::cout << it->key << "/" << it->value << " ";
                it = it->next;
            }

            std::cout << std::endl;
        }
    }
};

}

#endif
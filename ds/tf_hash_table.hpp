////////////
// USABLE //
////////////

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
    // BUCKET

    struct bucket {
        K key;
        V value;
        bucket *next;
    };

    bucket *alloc_bucket(const K &key, const V &value, bucket *next) {
        bucket *b = (bucket *)malloc(sizeof(bucket));
        b->key = key;
        b->value = value;
        b->next = next;
        return b;
    }

    void free_bucket(bucket *b) {
        if (b) {
            free_bucket(b->next);
            free(b);
        }
    }

    // VARIABLES

    const int table_size;
    bucket **buckets;

public:
    hash_table(const int table_size = 100):
        table_size(table_size),
        buckets((bucket **)malloc(table_size * sizeof(bucket *)))
    {
        memset(buckets, 0, table_size);
    }

    ~hash_table() {
        for (int i = 0; i < table_size; ++i) {
            free_bucket(*(buckets + i));
        }

        free(buckets);
    }

    // average case: O(1) - worst case: O(n)
    void insert(const K &key, const V &value) {
        unsigned long index = hash<K>(key) % table_size;

        if (*(buckets + index)) {
            bucket *it = *(buckets + index);
            while (it->next) {
                it = it->next;
            }

            it->next = alloc_bucket(key, value, nullptr);
        }
        else {
            *(buckets + index) = alloc_bucket(key, value, nullptr);
        }
    }

    // average case: O(1) - worst case: O(n)
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

    // average case: O(1) - worst case: O(n)
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

    // DEBUG
    void print() {
        for (int i = 0; i < table_size; ++i) {
            std::cout << "bucket " << i << ": ";

            bucket *it = *(buckets + i);
            if (!it) {
                std::cout << "- ";
            }
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
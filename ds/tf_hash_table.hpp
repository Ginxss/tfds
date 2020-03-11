//////////
// TODO //
//////////

// Nothing works!

#ifndef TF_HASH_TABLE_H
#define TF_HASH_TABLE_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "tf_linked_list.hpp"
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
        bool empty;
    };

// VARIABLES

    int array_size;
    bucket *array;

public:
    hash_table(const int array_size = 100):
        array_size(array_size),
        array((bucket *)malloc(array_size * sizeof(bucket)))
    {
        for (int i = 0; i < array_size; ++i) {
            array[i].empty = true;
        }
    }

    ~hash_table() {
        clear();
    }

    void insert(const K &key, const V &value) {
        unsigned long index = hash<K>(key) % array_size;
        bucket *b;
        int offset = 0;

        do {
            b = array + index + offset++;
            if (offset >= array_size)
                throw tf::exception("hash table: insert: hash table is full");
        } while (!b->empty);

        b->key = key;
        b->value = value;
        b->empty = false;
    }

    const V &get(const K &key) const {
        unsigned long index = hash<K>(key) % array_size;
        bucket *b;
        int offset = 0;
        
        do {
            b = array + index + offset++;
            if (b->empty || offset >= array_size)
                throw tf::exception("hash table: get: invalid key");
        } while (key != b->key);
        
        return b->value;
    }

    // do this
    V &operator[](const K &key) {
        unsigned long index = hash<K>(key) % array_size;
        bucket *b;
        int offset = 0;
        
        do {
            b = array + index + offset++;
            if (b->empty || offset >= array_size)
                throw tf::exception("hash table: get: invalid key");
        } while (key != b->key);
        
        return b->value;
    }

    // O(1)
    void clear() {
        if (array)
            free(array);
        array = nullptr;
    }

    // DEBUG
    void print() {
        for (int i = 0; i < array_size; ++i) {
            std::cout << "bucket " << i << ": ";

            bucket &b = array[i];
            if (b.empty) {
                std::cout << "-" << std::endl;
            }
            else {
                std::cout << b.key << "/" << b.value << std::endl;
            }
        }
    }
};

}

#endif
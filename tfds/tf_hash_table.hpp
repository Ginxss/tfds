#ifndef TF_HASH_TABLE_H
#define TF_HASH_TABLE_H

#include <string>
#include <cstring> // memcpy, memset
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

    bucket *next_bucket(bucket *curr_b, int *curr_index) {
        if (curr_b->next) {
            return curr_b->next;
        }

        (*curr_index)++;
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
        int index;
        bucket *b;

        V &operator*() { return b->value; }
        K &key() { return b->key; }
        V &value() { return b->value; }
        void operator++() { b = table->next_bucket(b, &index); }
        bool condition() { return b != nullptr; }
    };

private:
    const int table_size_;
    int size_;
    bucket **buckets;

    iterator start_it;

public:
    hash_table(const int table_size = 100):
        table_size_(table_size),
        size_(0),
        buckets((bucket **)malloc(table_size_ * sizeof(bucket *)))
    {
        memset(buckets, 0, table_size_ * sizeof(bucket *));
        start_it.table = this;
    }

    ~hash_table() {
        for (int i = 0; i < table_size_; ++i) {
            free_bucket(*(buckets + i));
        }

        free(buckets);
    }

    // average: O(1) / worst: O(n)
    void insert(const K &key, const V &value) {
        unsigned long index = hash<K>(key) % table_size_;

        if (*(buckets + index)) {
            bucket *it = *(buckets + index);
            if (key == it->key)
                throw tf::exception("hash table: insert: key already exists");
            
            while (it->next) {
                it = it->next;
                if (key == it->key)
                    throw tf::exception("hash table: insert: key already exists");
            }

            it->next = alloc_bucket(key, value, nullptr);
        }
        else {
            *(buckets + index) = alloc_bucket(key, value, nullptr);
        }

        size_++;
    }

    // average: O(1) / worst: O(n)
    const V remove(const K &key) {
        unsigned long index = hash<K>(key) % table_size_;

        if (key == (*(buckets + index))->key) {
            bucket *to_delete = *(buckets + index);
            *(buckets + index) = to_delete->next;

            V result = to_delete->value;
            free(to_delete);
            size_--;
            return result;
        }
        else {
            bucket *it = (*(buckets + index))->next;
            bucket *prev = *(buckets + index);
            while (it) {
                if (key == it->key) {
                    prev->next = it->next;
                    
                    V result = it->value;
                    free(it);
                    size_--;
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
            if (key == it->key) {
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
            if (key == it->key) {
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
            if (key == it->key) {
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
            start_it.index++;
            start_it.b = *(buckets + start_it.index);
        }

        return start_it;
    }

    // O(1)
    int table_size() const {
        return table_size_;
    }

    // O(1)
    int size() const  {
        return size_;
    }

    // O(1)
    bool empty() const {
        return size_ == 0;
    }

    // O(n)
    void clear() {
        for (int i = 0; i < table_size_; ++i) {
            free_bucket(*(buckets + i));
        }

        memset(buckets, 0, table_size_ * sizeof(bucket *));
        size_ = 0;
    }
};

}

#endif
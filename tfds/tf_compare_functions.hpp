#ifndef TF_COMPARE_FUNCTIONS_H
#define TF_COMPARE_FUNCTIONS_H

#include <string> // std::string
#include <cstring> // std::strcmp, std::memcpy, std::memset

namespace tf {

template <typename T>
inline bool compare(const T &key1, const T &key2) {
    return key1 == key2;
}

template <>
inline bool compare<std::string>(const std::string &key1, const std::string &key2) {
    return key1.compare(key2) == 0;
}

template <>
inline bool compare<const char *>(const char * const &key1, const char * const &key2) {
    return std::strcmp(key1, key2) == 0;
}

template <>
inline bool compare<char *>(char * const &key1, char * const &key2) {
    return std::strcmp(key1, key2) == 0;
}

}

#endif
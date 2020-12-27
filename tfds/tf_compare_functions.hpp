#ifndef TF_COMPARE_FUNCTIONS_H
#define TF_COMPARE_FUNCTIONS_H

#include <string> // std::string
#include <cstring> // std::strcmp, std::memcpy, std::memset

namespace tf {

template <typename T>
inline bool compare(const T &t1, const T &t2) {
    return t1 == t2;
}

template <>
inline bool compare<std::string>(const std::string &s1, const std::string &s2) {
    return s1.compare(s2) == 0;
}

template <>
inline bool compare<const char *>(const char * const &s1, const char * const &s2) {
    return std::strcmp(s1, s2) == 0;
}

template <>
inline bool compare<char *>(char * const &s1, char * const &s2) {
    return std::strcmp(s1, s2) == 0;
}

}

#endif
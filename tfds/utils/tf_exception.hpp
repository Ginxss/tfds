#ifndef TF_EXCEPTION_H
#define TF_EXCEPTION_H

#include <exception> // std::exception
#include <string> // std::string

namespace tf {

/*
* Exception class used in all tfds classes.
*/
class exception : public std::exception {
protected:
    std::string msg;

public:
    exception():
        msg("tf exception") {}

    exception(const std::string &msg):
        msg(msg) {}

    virtual ~exception() noexcept {}

    virtual const char *what() const throw() {
        return msg.c_str();
    }
};

}

#endif
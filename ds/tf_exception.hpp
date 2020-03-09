#ifndef TF_EXCEPTION_H
#define TF_EXCEPTION_H

#include <exception>

namespace tf {

class exception : public std::exception {
protected:
    const char *msg;

public:
    exception():
        msg("tf exception") {}

    exception(const char *msg):
        msg(msg) {}

    virtual ~exception() throw() {}

    virtual const char *what() const throw() {
        return msg;
    }
};

}

#endif
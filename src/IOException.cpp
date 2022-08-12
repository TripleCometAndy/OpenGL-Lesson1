#include "IOException.h"

namespace gl {
IOException::IOException(std::string message) {
    this->message = message;
}

const char *IOException::what() const throw() {
    return message.c_str();
}
} // namespace gl

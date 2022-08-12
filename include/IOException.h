#pragma once

#include <exception>
#include <iostream>

namespace gl {
struct IOException : public std::exception {
  private:
    std::string message;

  public:
    IOException(std::string message);
    const char *what() const throw();
};
}
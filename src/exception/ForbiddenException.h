#include <iostream>
#include <exception>

class ForbiddenException : public std::exception {
 public: 
  const void print_error() const throw ();

 private: 
  const std::string error = "Error 403 : Forbidden Exception";
};
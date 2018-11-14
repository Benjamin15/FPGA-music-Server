#include <iostream>
#include <exception>

class UnauthorizedException : public std::exception {
 public: 
  const void print_error() const throw ();

 private: 
  const std::string error = "Error 401 : Unauthorized Exception";
};
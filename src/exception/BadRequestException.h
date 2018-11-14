#include <iostream>
#include <exception>

class BadRequestException : public std::exception {
 public: 
  const void print_error () const throw ();

 private: 
  const std::string error = "Error 400 : Bad Request Exception";
};
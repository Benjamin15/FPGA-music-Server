#include <iostream>
#include <exception>

class RequestLargeException : public std::exception {
 public: 
  const void print_error() const throw ();

 private: 
  const std::string error = "Error 413 : request too large Exception";
};
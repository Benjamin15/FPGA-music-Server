#pragma once
#include <iostream>
#include <exception>

class NotAllowedException : public std::exception {
 public: 
  const void print_error() const throw ();

 private: 
  const std::string error = "Error 405 : Forbidden Exception";
};
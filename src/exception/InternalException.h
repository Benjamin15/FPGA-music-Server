#pragma once
#include <iostream>
#include <exception>

class InternalException : public std::exception {
 public: 
  const void print_error() const throw ();

 private: 
  const std::string error = "Error 500 : Internal Exception";
};
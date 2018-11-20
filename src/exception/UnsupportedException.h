#pragma once
#include <iostream>
#include <exception>

class UnsupportedException : public std::exception {
 public: 
  const void print_error() const throw ();

 private: 
  const std::string error = "Error 415 : Unsupported Exception";
};
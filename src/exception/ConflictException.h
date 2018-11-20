#pragma once

#include <iostream>
#include <exception>

class ConflictException : public std::exception {
 public: 
  const void print_error () const throw ();

 private: 
  const std::string error = "Error 409 : Conflict Exception";
};
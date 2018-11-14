#include "ConflictException.h"

const void ConflictException::print_error () const throw () {
  std::cout << error << std::endl;
}
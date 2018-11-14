#include "InternalException.h"

const void InternalException::print_error () const throw () {
  std::cout << error << std::endl;
}
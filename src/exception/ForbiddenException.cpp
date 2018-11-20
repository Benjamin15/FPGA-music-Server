#include "ForbiddenException.h"

const void ForbiddenException::print_error () const throw () {
  std::cout << error << std::endl;
}
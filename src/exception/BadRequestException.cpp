#include "BadRequestException.h"

const void BadRequestException::print_error () const throw () {
  std::cout << error << std::endl;
}
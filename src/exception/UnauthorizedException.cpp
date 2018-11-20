#include "UnauthorizedException.h"

const void UnauthorizedException::print_error() const throw () {
  std::cout << error << std::endl;
}
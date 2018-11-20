#include "RequestLargeException.h"

const void RequestLargeException::print_error() const throw () {
  std::cout << error << std::endl;
}
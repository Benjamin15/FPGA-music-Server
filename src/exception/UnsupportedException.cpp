#include "UnsupportedException.h"

const void UnsupportedException::print_error() const throw () {
  std::cout << error << std::endl;
}
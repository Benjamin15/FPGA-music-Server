#include "NotAllowedException.h"

const void NotAllowedException::print_error () const throw () {
  std::cout << error << std::endl;
}
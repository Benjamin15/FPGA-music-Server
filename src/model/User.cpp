#include "User.h"

std::string User::to_string() {
  std::stringstream result;
  result << "{ "
    << "\"id\": \"" << id_ << "\", "
    << "\"name\": \"" << name_ << "\", "
    << "\"ip\": \"" << ip_ << "\", "
    << "\"mac\": \"" << mac_ << "\", "
    << "\"is_block\":" << is_block_ << " }";
  return result.str();
}
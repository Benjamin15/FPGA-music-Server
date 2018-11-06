#include "Music.h"

std::ostream& Music::operator<<(std::ostream &strm) {
  return strm << toString();
}

std::string Music::toStringForUser() {
  std::stringstream result;
  result << "{ "
    << "\"titre\": \"" << title_ << "\", "
    << "\"artiste\": \"" << artist_ << "\", "
    << "\"duree\": \"" << duration_ << "\", "
    << "\"proposeePar\": \"" << user_.name_ << "\", "
    << "\"proprietaire\": \"" << owner_ << "\", "
    << "\"no\": " << id_ << " }";
  return result.str();
}

std::string Music::toString() {
  std::stringstream result;
  result << "{ "
    << "\"titre\": \"" << title_ << "\", "
    << "\"artiste\": \"" << artist_ << "\", "
    << "\"duree\": \"" << duration_ << "\", "
    << "\"ip\": \"" << user_.ip_ << "\", "
    << "\"MAC\": \"" << user_.mac_ << "\", "
    << "\"id\":" << user_.id_ << ", "
    << "\"proposeePar\": \"" << user_.name_ << "\", "
    << "\"proprietaire\": \"" << owner_ << "\", "
    << "\"no\": " << id_ << " }";
  return result.str();
}
#include "Music.h"

std::ostream& Music::operator<<(std::ostream &strm) {
  return strm << toString();
}

std::string Music::toString() {
  std::stringstream result;
  result << "{ "
    << "\"titre\": \"" << title_ << "\", "
    << "\"artiste\": \"" << artist_ << "\", "
    << "\"duree\": \"" << duration_ << "\", "
    << "\"proposeePar\": \"" << suggestBy_ << "\", "
    << "\"proprietaire\": \"" << owner_ << "\", "
    << "\"no \": \"" << id_ << "\" }";
  return result.str();
}
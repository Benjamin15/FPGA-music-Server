#ifndef MUSIC_H
#define MUSIC_H
#include <memory>
#include <iostream>
#include <sstream>
#include "User.h"
/**
 * Class model of music
 */  
class Music
{  
 public:
  Music(unsigned int id, std::string title, std::string artist, std::string duration, User user)
      : id_(id), title_(title), artist_(artist), duration_(duration), user_(user), owner_(false) {}
  
  std::ostream& operator<<(std::ostream &strm);
  std::string toStringForUser();
  std::string toString();
  
  unsigned int id_;
  std::string title_;
  std::string artist_;
  std::string duration_;
	User user_;
  bool owner_;

 private:

};

#endif
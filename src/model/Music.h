#ifndef MUSIC_H
#define MUSIC_H
#include <memory>
#include <iostream>
#include <sstream>

/**
 * Class model of music
 */  
class Music
{  
 public:
  Music(unsigned int id, std::string title, std::string artist, std::string duration, std::string suggestBy)
      : id_(id), title_(title), artist_(artist), duration_(duration), suggestBy_(suggestBy), owner_(false){}
  
  std::ostream& operator<<(std::ostream &strm);
  std::string toString();
  
  unsigned int id_;
  std::string title_;
  std::string artist_;
  std::string duration_;
  std::string suggestBy_;
  bool owner_;

 private:

};

#endif
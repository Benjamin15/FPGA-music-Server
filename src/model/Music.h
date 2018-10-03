#ifndef MUSIC_H
#define MUSIC_H
#include <memory>
#include <iostream>

/**
 * Class model of music
 */  
class Music
{  
 public:
  Music(unsigned int id, std::string title, std::string artist, std::string duration, std::string suggestBy)
      : id_(id), title_(title), artist_(artist), duration_(duration), suggestBy_(suggestBy){}

  unsigned int id_;
  std::string title_;
  std::string artist_;
  std::string duration_;
  std::string suggestBy_;

 private:
 
};

#endif
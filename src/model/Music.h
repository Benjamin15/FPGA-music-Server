#ifndef MUSIC_H
#define MUSIC_H
#include <memory>
#include <iostream>
#include <sstream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include "rapidjson/document.h"
#include "User.h"
/**
 * Class model of music
 */  
class Music
{  
 public:
  Music(unsigned int no, std::string title, std::string artist, std::string duration, User user)
      : no_(no), title_(title), artist_(artist), duration_(duration), user_(user), owner_(false) {}
  Music(std::string title, std::string artist, std::string duration)
      : no_(0), title_(title), artist_(artist), duration_(duration), owner_(false) {}
  
  std::ostream& operator<<(std::ostream &strm);
  friend bool operator==(const Music &musicFirst, const Music &musicSecond){
      return (musicFirst.no_==musicSecond.no_ && musicFirst.title_==musicSecond.title_ 
          && musicFirst.artist_==musicSecond.artist_ && musicFirst.duration_==musicSecond.duration_
          && musicFirst.user_==musicSecond.user_);
  }

  bool operator==(const unsigned int no) const { return this->no_ == no;}

  friend bool operator<(const Music &musicFirst, const Music &musicSecond) {
  return (musicFirst.no_<musicSecond.no_ && musicFirst.title_<musicSecond.title_ 
          && musicFirst.artist_<musicSecond.artist_ && musicFirst.duration_<musicSecond.duration_
          && musicFirst.user_<musicSecond.user_);
  }
  std::string toStringForUser();
  std::string toString();
  std::string toRegisterString();
  void setMusicUser(User user);
  void setMusicTitle(std::string fileName);
  void setMusicNumber(std::string path);
  static int getNextMusicId(std::string path);

  
  unsigned int no_;
  std::string title_;
  std::string artist_;
  std::string duration_;
  User user_;
  bool owner_;

 private:

};

#endif
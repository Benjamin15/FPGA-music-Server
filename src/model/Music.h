#pragma once

#include <memory>
#include <iostream>
#include <sstream>
#include "../header/rapidjson.h"
#include "User.h"


#define Object rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>>
#define ObjectMetadata rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>>


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
  
  Music(Object music);
  Music(ObjectMetadata music);


  std::ostream& operator<<(std::ostream &strm);

  bool operator==(const unsigned int no) const { return this->no_ == no;}

  std::string toStringForUser();
  std::string toString();
  std::string toRegisterString();
  void setMusicUser(User user);
  void setMusicTitle(std::string fileName);
  void setMusicNumber();
  static int getNextMusicId(std::string path);
  rapidjson::Document to_json();

  unsigned int no_;
  std::string title_;
  std::string artist_;
  std::string duration_;
  User user_;
  bool owner_;

 private:

};
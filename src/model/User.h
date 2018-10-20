#ifndef USER_H
#define USER_H
#include <memory>
#include <iostream>
#include <sstream>

/**
 * Class model of user
 */  
class User
{  
 public:
  User(unsigned int id, std::string name, std::string ip, std::string mac)
      : id_(id), name_(name), ip_(ip), mac_(mac){}
  User() : id_(0), name_(""), ip_(""), mac_("") {}
  friend bool operator==(const User& userFirst, const User& userSecond){
          return (userFirst.id_== userSecond.id_ && userFirst.name_==userSecond.name_
    && userFirst.ip_==userSecond.ip_ && userFirst.mac_==userSecond.mac_);
  }
  friend bool operator<(const User& userFirst, const User& userSecond){
          return (userFirst.id_< userSecond.id_ && userFirst.name_<userSecond.name_
    && userFirst.ip_<userSecond.ip_ && userFirst.mac_<userSecond.mac_);
  }
  //std::ostream& operator<<(std::ostream &strm);
  //std::string toString();
  
  unsigned int id_;
  std::string name_;
  std::string ip_;
  std::string mac_;
 private:

};

#endif
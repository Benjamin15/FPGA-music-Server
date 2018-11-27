#pragma once

#include <memory>
#include <iostream>
#include <sstream>
#include <random>
#include <chrono>
#include "../metadata/metadata.h"
#include "../header/rapidjson.h"

#define Object rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>>
#define ObjectMetadata rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>>

/**
 * Class model of user
 */  
class User
{  
 public:
  User(unsigned int token, std::string name, std::string ip, std::string mac)
      : token_(token), name_(name), ip_(ip), mac_(mac), is_blocked_(false) {}
  User() : token_(), name_(""), ip_(""), mac_(""), is_blocked_(false) {}
  User(Object value);
  User(ObjectMetadata value);
  //std::ostream& operator<<(std::ostream &strm);

  bool operator==(const unsigned int token) const { return this->token_ == token;}
  bool operator==(const User user) const { return this->token_ == user.token_;}


  unsigned int token_;
  std::string name_;
  std::string ip_;
  std::string mac_; 
  bool is_blocked_;
  int64_t create_at_;

  std::string to_string();
  rapidjson::Document to_json();
 private:

};
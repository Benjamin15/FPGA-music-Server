#include "User.h"

/**
 * Constructor used when we read parameter
 * @param object_json
 */ 
User::User(Object value) {
  if (value.HasMember(id_log.c_str()) && value[id_log.c_str()].IsUint())
    token_ = value[id_log.c_str()].GetUint();
  else
    token_ = rand();
  if (value.HasMember(name_log.c_str()) && value[name_log.c_str()].IsString())
    name_ = value[name_log.c_str()].GetString();
  else
    std::cout << "name mal formaté" << std::endl;
  if (value.HasMember(ip_log.c_str()) && value[ip_log.c_str()].IsString())
    ip_ = value[ip_log.c_str()].GetString();
  else 
    std::cout << "ip mal formaté" << std::endl;
  if (value.HasMember(mac_log.c_str()) && value[mac_log.c_str()].IsString())
    mac_ = value[mac_log.c_str()].GetString();
  else 
    std::cout << "mac mal formaté " << std::endl;
  if (value.HasMember(is_block_log.c_str()) && value[is_block_log.c_str()].IsBool())
    is_blocked_ = value[is_block_log.c_str()].GetBool();
  else
    is_blocked_ = false;
  if (value.HasMember(create_at_log.c_str()) && value[create_at_log.c_str()].IsInt64())
    create_at_ = value[create_at_log.c_str()].GetInt64();
  else {
    std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) ;
    create_at_ = timestamp.count();
  }
}

/**
 * 
 * Constructor used when we used metadata file
 * @param objectMetadata
 */ 

User::User(ObjectMetadata value) {
  if (value.HasMember(id_log.c_str()) && value[id_log.c_str()].IsUint())
    token_ = value[id_log.c_str()].GetUint();
  else
    token_ = rand();
  if (value.HasMember(name_log.c_str()) && value[name_log.c_str()].IsString())
    name_ = value[name_log.c_str()].GetString();
  else
    std::cout << "name mal formaté" << std::endl;
  if (value.HasMember(ip_log.c_str()) && value[ip_log.c_str()].IsString())
    ip_ = value[ip_log.c_str()].GetString();
  else 
    std::cout << "ip mal formaté" << std::endl;
  if (value.HasMember(mac_log.c_str()) && value[mac_log.c_str()].IsString())
    mac_ = value[mac_log.c_str()].GetString();
  else 
    std::cout << "mac mal formaté " << std::endl;
  if (value.HasMember(is_block_log.c_str()) && value[is_block_log.c_str()].IsBool())
    is_blocked_ = value[is_block_log.c_str()].GetBool();
  else
    is_blocked_ = false;
  if (value.HasMember(create_at_log.c_str()) && value[create_at_log.c_str()].IsInt64())
    create_at_ = value[create_at_log.c_str()].GetInt64();
  else {
    std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) ;
    create_at_ = timestamp.count();
  }
}

/**
 * Method to_string
 * @return string of object
 */ 
std::string User::to_string() {
  std::stringstream result;
  result << "{ "
    << "\"token\": " << token_ << ", "
    << "\"name\": \"" << name_ << "\", "
    << "\"ip\": \"" << ip_ << "\", "
    << "\"mac\": \"" << mac_ << "\", "
    << "\"create_at\": \"" << create_at_ << "\", "
    << "\"is_blocked\":" << is_blocked_ << " }";
  return result.str();
}


/**
 * convert this object to json
 * @return json
 */ 
rapidjson::Document User::to_json() {
  rapidjson::Document document;
 	document.SetObject();
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
  document.AddMember("token", rapidjson::Value().SetUint(token_), allocator);
  document.AddMember("name", rapidjson::Value().SetString(name_.c_str(), name_.size()), allocator);
  document.AddMember("ip", rapidjson::Value().SetString(ip_.c_str(), ip_.size()), allocator);
  document.AddMember("mac", rapidjson::Value().SetString(mac_.c_str(), mac_.size()), allocator);
  document.AddMember("is_blocked", rapidjson::Value().SetBool(is_blocked_), allocator);
  document.AddMember("create_at", rapidjson::Value().SetUint64(create_at_), allocator);
  return document;
}
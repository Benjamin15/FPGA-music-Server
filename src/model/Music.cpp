#include "Music.h"
#include "../utils/json.h"

std::ostream& Music::operator<<(std::ostream &strm) {
  return strm << toString();
}

Music::Music(Object music) {
  unsigned int token = 0;
  std::string ip = "";
  std::string mac = "";
  std::string suggest_by = "";
  if (music.HasMember(no_log.c_str()) && music[no_log.c_str()].IsUint())
    no_ = music[id_log.c_str()].GetUint();
  else
    setMusicNumber();
  if (music.HasMember(title_log.c_str()) && music[title_log.c_str()].IsString())
    title_ = music[title_log.c_str()].GetString();
  if (music.HasMember(artist_log.c_str()) && music[artist_log.c_str()].IsString())
    artist_ = music[artist_log.c_str()].GetString(); 
  if (music.HasMember(duration_log.c_str()) && music[duration_log.c_str()].IsString())
    duration_ = music[duration_log.c_str()].GetString(); 
  if (music.HasMember(id_log.c_str()) && music[id_log.c_str()].IsUint())
    token = music[id_log.c_str()].GetUint();
  if (music.HasMember(ip_log.c_str()) && music[ip_log.c_str()].IsString())
    ip = music[ip_log.c_str()].GetString();
  if (music.HasMember(mac_log.c_str()) && music[mac_log.c_str()].IsString())
    mac = music[mac_log.c_str()].GetString();
  if (music.HasMember(suggest_by_log.c_str()) && music[suggest_by_log.c_str()].IsString())
    suggest_by = music[mac_log.c_str()].GetString();
  user_ = User(token, suggest_by, ip, mac);
}

Music::Music(ObjectMetadata music) {
  unsigned int token = 0;
  std::string ip = "";
  std::string mac = "";
  if (music.HasMember(no_log.c_str()) && music[no_log.c_str()].IsUint())
    no_ = music[id_log.c_str()].GetUint();
  else
    setMusicNumber();
  if (music.HasMember(title_log.c_str()) && music[title_log.c_str()].IsString())
    title_ = music[title_log.c_str()].GetString();
  if (music.HasMember(artist_log.c_str()) && music[artist_log.c_str()].IsString())
    artist_ = music[artist_log.c_str()].GetString(); 
  if (music.HasMember(duration_log.c_str()) && music[duration_log.c_str()].IsString())
    duration_ = music[duration_log.c_str()].GetString(); 
  if (music.HasMember(id_log.c_str()) && music[id_log.c_str()].IsUint())
    token = music[id_log.c_str()].GetUint();
  if (music.HasMember(ip_log.c_str()) && music[ip_log.c_str()].IsString())
    ip = music[ip_log.c_str()].GetString();
  if (music.HasMember(mac_log.c_str()) && music[mac_log.c_str()].IsString())
    mac = music[mac_log.c_str()].GetString();
  user_ = User(token, "", ip, mac);
}

void Music::setMusicNumber() {
  rapidjson::Document document = getJsonFile(music_json_path.c_str());
  rapidjson::Value& value = document[musics_log.c_str()];
  unsigned int higherMusicNo = 0;
  if(value.GetArray().Size() != 0) {
    for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
      if (value[i][no_log.c_str()].GetUint() > higherMusicNo) {
        higherMusicNo = value[i][no_log.c_str()].GetUint();
      }
    }
    no_ = higherMusicNo + 1;
  }else {
    no_ = 0;
  }
}

int Music::getNextMusicId(std::string path) {
  rapidjson::Document document = getJsonFile(path.c_str());
  rapidjson::Value& value = document[musics_log.c_str()];
  unsigned int higherMusicNo = 0;
  if (value.GetArray().Size() != 0) {
    for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
      if (value[i][no_log.c_str()].GetUint() > higherMusicNo) {
        higherMusicNo = value[i][no_log.c_str()].GetUint();
      }
    }
    higherMusicNo ++;
    return higherMusicNo;
  } else
    return 0;
}

std::string Music::toStringForUser() {
  std::stringstream result;
  result << "{ "
    << "\"titre\": \"" << title_ << "\", "
    << "\"artiste\": \"" << artist_ << "\", "
    << "\"duree\": \"" << duration_ << "\", "
    << "\"proposeePar\": \"" << user_.name_ << "\", "
    << "\"proprietaire\": \"" << owner_ << "\", "
    << "\"IdProprietaire\": \"" << user_.token_ << "\", "
    << "\"no\": " << no_ << " }";
  return result.str();
}

std::string Music::toString() {
  std::stringstream result;
  result << "{ "
    << "\"title\": \"" << title_ << "\", "
    << "\"artist\": \"" << artist_ << "\", "
    << "\"duration\": \"" << duration_ << "\", "
    << "\"ip\": \"" << user_.ip_ << "\", "
    << "\"mac\": \"" << user_.mac_ << "\", "
    << "\"id\":" << user_.token_ << ", "
    << "\"suggested_by\": \"" << user_.name_ << "\", "
    << "\"proprietaire\": \"" << owner_ << "\", "
    << "\"no\":" << no_ << " }";
  return result.str();
}

std::string Music::toRegisterString() {
  std::stringstream result;
  result << "{ "
    << "\"title\": \"" << title_ << "\", "
    << "\"artist\": \"" << artist_ << "\", "
    << "\"duration\": \"" << duration_ << "\", "
    << "\"ip\": \"" << user_.ip_ << "\", "
    << "\"mac\": \"" << user_.mac_ << "\", "
    << "\"id\":" << user_.token_ << ", "
    << "\"suggested_by\": \"" << user_.name_ << "\", "
    << "\"no\":" << no_ << " }";
  return result.str();
}

/**
 * convert this object to json
 * @return json
 */ 
rapidjson::Document Music::to_json() {
  rapidjson::Document document;
 	document.SetObject();
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
  document.AddMember("no", rapidjson::Value().SetUint(no_), allocator);
  document.AddMember("title", rapidjson::Value().SetString(title_.c_str(), title_.size()), allocator);
  document.AddMember("artist", rapidjson::Value().SetString(artist_.c_str(), artist_.size()), allocator);
  document.AddMember("duration", rapidjson::Value().SetString(duration_.c_str(), duration_.size()), allocator);
  document.AddMember("ip", rapidjson::Value().SetString(user_.ip_.c_str(), user_.ip_.size()), allocator);
  document.AddMember("mac", rapidjson::Value().SetString(user_.mac_.c_str(), user_.mac_.size()), allocator);
  document.AddMember("id", rapidjson::Value().SetUint(user_.token_), allocator);
  document.AddMember("suggest_by", rapidjson::Value().SetString(user_.name_.c_str(), user_.name_.size()), allocator);
  return document;
}
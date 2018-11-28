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
    no_ = music[no_log.c_str()].GetUint();
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
  std::cout << "no : " << no_ << std::endl;
}

Music::Music(ObjectMetadata music) {
  unsigned int token = 0;
  std::string ip = "";
  std::string mac = "";
  if (music.HasMember(no_log.c_str()) && music[no_log.c_str()].IsUint())
    no_ = music[no_log.c_str()].GetUint();
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
    std::cout << "no : " << no_ << std::endl;

}

void Music::setMusicNumber() {
  std::cout << "calcul numero musique" << std::endl;
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
    << "\"titre\": \"" << title_ << "\", "
    << "\"artiste\": \"" << artist_ << "\", "
    << "\"duree\": \"" << duration_ << "\", "
    << "\"ip\": \"" << user_.ip_ << "\", "
    << "\"mac\": \"" << user_.mac_ << "\", "
    << "\"IdProprietaire\":" << user_.token_ << ", "
    << "\"proposeePar\": \"" << user_.name_ << "\", "
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
  rapidjson::Value no_key(no_log.c_str(), document.GetAllocator());
  rapidjson::Value no_value(no_);
  document.AddMember(no_key, no_, document.GetAllocator());
  rapidjson::Value title_key(title_log.c_str(), document.GetAllocator());
  rapidjson::Value title_value(title_.c_str(), document.GetAllocator());
  document.AddMember(title_key, title_value, document.GetAllocator());
  rapidjson::Value artist_key(artist_log.c_str(), document.GetAllocator());
  rapidjson::Value artist_value(artist_.c_str(), document.GetAllocator());
  document.AddMember(artist_key, artist_value, document.GetAllocator());
  rapidjson::Value duration_key(duration_log.c_str(), document.GetAllocator());
  rapidjson::Value duration_value(duration_.c_str(), document.GetAllocator());
  document.AddMember(duration_key, duration_value, document.GetAllocator());
  rapidjson::Value ip_key(ip_log.c_str(), document.GetAllocator());
  rapidjson::Value ip_value(user_.ip_.c_str(), document.GetAllocator());
  document.AddMember(ip_key, ip_value, document.GetAllocator());
  rapidjson::Value mac_key(mac_log.c_str(), document.GetAllocator());
  rapidjson::Value mac_value(user_.mac_.c_str(), document.GetAllocator());
  document.AddMember(mac_key, mac_value, document.GetAllocator());
  rapidjson::Value id_key(id_log.c_str(), document.GetAllocator());
  rapidjson::Value id_value(user_.token_);
  document.AddMember(id_key, id_value, document.GetAllocator());
  rapidjson::Value suggest_by_key(no_log.c_str(), document.GetAllocator());
  rapidjson::Value suggest_by_value(user_.name_.c_str(), document.GetAllocator());
  document.AddMember(suggest_by_key, suggest_by_value, document.GetAllocator());
  return document;
}
#include "Music.h"
#include "../utils/json.h"

std::ostream& Music::operator<<(std::ostream &strm) {
  return strm << toString();
}

void Music::setMusicUser(User user){
  user_= user;
}

void Music::setMusicTitle(std::string fileName){
  title_= fileName;
  }

void Music::setMusicNumber(std::string path){
  rapidjson::Document document = getJsonFile(path.c_str());
  rapidjson::Value& value = document["musics"];
  unsigned int higherMusicNo = 0;
  if(value.GetArray().Size() != 0) {
    for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
      if (value[i]["no"].GetUint() > higherMusicNo) {
        higherMusicNo = value[i]["no"].GetUint();
      }
    }
    no_ = higherMusicNo + 1;
  }else {
    no_ = 0;
  }
}

int Music::getNextMusicId(std::string path) {
  rapidjson::Document document = getJsonFile(path.c_str());
  rapidjson::Value& value = document["musics"];
  unsigned int higherMusicNo = 0;
  if (value.GetArray().Size() != 0) {
    for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
      if (value[i]["no"].GetUint() > higherMusicNo) {
        higherMusicNo = value[i]["no"].GetUint();
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
    << "\"MAC\": \"" << user_.mac_ << "\", "
    << "\"id\":" << user_.id_ << ", "
    << "\"proposeePar\": \"" << user_.name_ << "\", "
    << "\"proprietaire\": \"" << owner_ << "\", "
    << "\"no\":" << no_ << " }";
  return result.str();
}

std::string Music::toRegisterString() {
  std::stringstream result;
  result << "{ "
    << "\"titre\": \"" << title_ << "\", "
    << "\"artiste\": \"" << artist_ << "\", "
    << "\"duree\": \"" << duration_ << "\", "
    << "\"ip\": \"" << user_.ip_ << "\", "
    << "\"MAC\": \"" << user_.mac_ << "\", "
    << "\"id\":" << user_.id_ << ", "
    << "\"proposeePar\": \"" << user_.name_ << "\", "
    << "\"no\":" << no_ << " }";
  return result.str();
}
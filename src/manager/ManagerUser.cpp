#include "ManagerUser.h"

bool checkUserToken(unsigned int token) {
   rapidjson::Document idLogs = getJsonFile("metadata/idLogs.json");
   rapidjson::Value& value = idLogs["UsersLogs"];
   for (rapidjson::SizeType i = 0; i < value.GetArray().Size(); i++) {
      if (value[i]["Token"].GetUint() == token) {
        return true;
      }
   }
  return false;
}

bool isValidToken(unsigned int token) {
  rapidjson::Document document = getJsonFile("metadata/idLogs.json");
  rapidjson::Value& usersLogs = document["UsersLogs"];
  std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  int time = timestamp.count();
  for (rapidjson::SizeType i = 0; i < usersLogs.Size(); i++) { 
    if (usersLogs[i]["Token"].GetUint() == token) {
      if (time - usersLogs[i]["Time"].GetUint() >= 24*60*60*1000) {
        std::cout << "Token invalide" << std::endl; 
        return false;
      }
      return true;
    }
  }
  return false;
}

bool identify(unsigned int token) {
  return checkUserToken(token) && isValidToken(token);
}

User get_user_for_sent_music(unsigned int token){ 
  rapidjson::Document document = getJsonFile("metadata/idLogs.json");
  rapidjson::Value& value = document["UsersLogs"];
  std::string userMac, userIp, userName;
  for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
    unsigned int temp = value[i]["Token"].GetUint();
    if(temp == token) {
      userMac = value[i]["MAC"].GetString();
      userIp = value[i]["ip"].GetString();
      if (value[i]["nom"].IsString())
        userName = value[i]["nom"].GetString();
    }
  }
  User user(token, userName, userIp, userMac);
  return user;
}

void update_password(std::string old_password, std::string new_password) {
  rapidjson::Document d = getJsonFile("metadata/admin.json");
  rapidjson::SizeType i = 0;
  rapidjson::Value& admin = d["admin"];
  if (old_password != (admin[i]["mot_de_passe"].GetString()))
    throw UnauthorizedException();
  else if (new_password == "")
    throw BadRequestException();
  else {
    // TODO : Useless je pense, à verifier
    admin[i]["mot_de_passe"].SetObject();
    admin[i]["mot_de_passe"].SetString(new_password.c_str(), new_password.length());
    writeJsonFile("metadata/admin.json", d);
  }
}

std::string sign_in(std::string body) {
  /*srand(time(NULL));
  std::string token = std::to_string(rand());
  std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) ;
  int64_t time = timestamp.count();
  rapidjson::Document document;
  document.Parse<0>(body.c_str());
  document.AddMember("Token", token, document.GetAllocator());
  document.AddMember("Time", time, document.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);
  std::string response_token = registerIds(buffer.GetString());
  if (response_token == "0") {
    SysLoggerSingleton::GetInstance().WriteLine("Emission d'un nouvel identificateur d'usager ordinaire: " + token); 
    return createIdentificationResponseJson(token, "Bienvenue sur l'application Café-Bistro Elevation !");
  } else {
    SysLoggerSingleton::GetInstance().WriteLine("Emission de l'identificateur " + response_token); 
    return createIdentificationResponseJson(token, "Bienvenue sur l'application Café-Bistro Elevation !");
  }*/ return "";
}


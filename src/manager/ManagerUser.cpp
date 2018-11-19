#include "ManagerUser.h"

bool checkUserToken(unsigned int token) {
  std::cout << "checkUserToken :  " <<  token << std::endl;
  rapidjson::Document document = getJsonFile("metadata/idLogs.json");
  rapidjson::Value& users = document["UsersLogs"];
  for (rapidjson::SizeType i = 0; i < users.GetArray().Size(); i++) {
    if (users[i]["Token"].GetUint() == token) {
      return true;
    }
  }
  std::cout << "check fail" << std::endl;
  return false;
}

bool isValidToken(unsigned int token) {
  std::cout << "is Valid Token " << std::endl;
  rapidjson::Document document = getJsonFile("metadata/idLogs.json");
  rapidjson::Value& usersLogs = document["UsersLogs"];
  const unsigned int day = 24 * 60 * 60 * 1000;
  std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t time = timestamp.count();
  for (rapidjson::SizeType i = 0; i < usersLogs.Size(); i++) { 
    if (usersLogs[i]["Token"].GetUint() == token) {
      if (usersLogs[i]["Time"].IsUint64()) {
        if (time - usersLogs[i]["Time"].GetUint64() >= day) {
          std::cout << "Token invalide" << std::endl; 
          return false;
        }
        std::cout << "token valide" << std::endl;
        return true;
      }
      std::cout << "error parse uint" << std::endl;
    }
  }
  std::cout << "aucun token trouve" << std::endl;
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
  srand(time(NULL));
  unsigned int token = rand();

  std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) ;
  int64_t time = timestamp.count();
  rapidjson::Document document;
  std::cout << "body : " << std::endl;
  std::cout << body.c_str() << std::endl;
  document.Parse<0>(body.c_str());
  document.AddMember("Token", token, document.GetAllocator());
  document.AddMember("Time", time, document.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);
  std::string response_token = registerIds(buffer.GetString());
  if (response_token == "0") {
    std::cout << "response = 0" << std::endl;
    write_log("Emission d'un nouvel identificateur d'usager ordinaire: " + std::to_string(token)); 
    std::cout <<" log done" << std::endl;
    return createIdentificationResponseJson(std::to_string(token), "Bienvenue sur l'application Café-Bistro Elevation !");
  } else {
    std::cout << "response token " << std::endl;
    write_log("Emission de l'identificateur " + response_token); 
    std::cout << "log done" << std::endl;
    return createIdentificationResponseJson(response_token, "Bienvenue sur l'application Café-Bistro Elevation !");
  }
}


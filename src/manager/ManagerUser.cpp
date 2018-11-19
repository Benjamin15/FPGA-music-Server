#include "ManagerUser.h"

/**
 * Check if the token exist
 * @param token 
 * @return exist
 */ 
bool checkUserToken(unsigned int token) {
  rapidjson::Document document = getJsonFile(user_log_path.c_str());
  rapidjson::Value& users = document[users_log.c_str()];
  for (rapidjson::SizeType i = 0; i < users.GetArray().Size(); i++) {
    if (users[i][token_log.c_str()].GetUint() == token) {
      return true;
    }
  }
  return false;
}

/**
 *  Check if the token is create in the last 24 hours
 * @param token 
 * @return isValid
 * 
 */ 
bool isValidToken(unsigned int token) {
  rapidjson::Document document = getJsonFile(user_log_path.c_str());
  rapidjson::Value& users = document[users_log.c_str()];
  const unsigned int day = 24 * 60 * 60 * 1000;
  std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
  uint64_t time = timestamp.count();
  for (rapidjson::SizeType i = 0; i < users.Size(); i++) { 
    if (users[i][token_log.c_str()].GetUint() == token) {
      if (users[i][time_log.c_str()].IsUint64()) {
        if (time - users[i][time_log.c_str()].GetUint64() >= day) {
          return false;
        }
        return true;
      }
    }
  }
  return false;
}

/**
 * try to identify the user
 * @param token 
 * @return isIdentify
 */ 
bool identify(unsigned int token) {
  return checkUserToken(token) && isValidToken(token);
}

/**
 * get the user with this token
 * @param token
 * @return user
 */ 
User get_user(unsigned int token){ 
  rapidjson::Document document = getJsonFile(user_log_path.c_str());
  rapidjson::Value& value = document[users_log.c_str()];
  std::string userMac, userIp, userName;
  for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
    unsigned int temp = value[i][token_log.c_str()].GetUint();
    if(temp == token) {
      userMac = value[i][mac_log.c_str()].GetString();
      userIp = value[i][ip_log.c_str()].GetString();
      if (value[i][name_log.c_str()].IsString())
        userName = value[i][name_log.c_str()].GetString();
    }
  }
  User user(token, userName, userIp, userMac);
  return user;
}

/**
 * change the password
 * @param old_password
 * @param new_password
 */ 
void update_password(std::string old_password, std::string new_password) {
  rapidjson::Document d = getJsonFile(admin_log_path.c_str());
  rapidjson::SizeType i = 0;
  rapidjson::Value& admin = d[admin_log.c_str()];
  if (old_password != (admin[i][password_log.c_str()].GetString()))
    throw UnauthorizedException();
  else if (new_password == "")
    throw BadRequestException();
  else {
    admin[i][password_log.c_str()].SetObject();
    admin[i][password_log.c_str()].SetString(new_password.c_str(), new_password.length());
    writeJsonFile(admin_log_path.c_str(), d);
  }
}

/**
 * Sign the user 
 * @param body body of the request, with ip, mac and name
 * @return token
 */ 
std::string sign_in(std::string body) {
  srand(time(NULL));
  unsigned int token = rand();
  std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) ;
  int64_t time = timestamp.count();
  rapidjson::Document document;
  document.Parse<0>(body.c_str());
  document.AddMember("token", token, document.GetAllocator());
  document.AddMember("time", time, document.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);
  std::string response_token = registerIds(buffer.GetString());
  if (response_token == "0") {
    write_log("Emission d'un nouvel identificateur d'usager ordinaire: " + std::to_string(token)); 
    return createIdentificationResponseJson(std::to_string(token), "Bienvenue sur l'application Café-Bistro Elevation !");
  } else {
    write_log("Emission de l'identificateur " + response_token); 
    return createIdentificationResponseJson(response_token, "Bienvenue sur l'application Café-Bistro Elevation !");
  }
}

/**
 * search if the token exist in the log for the same mac
 * @param users list
 * @param mac
 * @return token
 */ 
std::string find_token(rapidjson::Value& users, rapidjson::Value& mac) {
  std::string token = "0";
  for (rapidjson::SizeType i = 0; i < users.Size(); i++) {
    std::string temp = users[i][mac_log.c_str()].GetString();
    if(!temp.compare(mac.GetString())) {
      token = std::to_string(users[i][token_log.c_str()].GetUint());
    }
  }
  return token;
}


/**
 * register token in the metadata file
 * @param body_json
 * @return token
 * 
 */ 
std::string registerIds(std::string body_json) {
  FILE* fp = fopen(user_log_path.c_str(), "rb");
  char buffer_reader[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document readDoc;
  readDoc.ParseStream(is);
  fclose(fp);
  rapidjson::Document writeDoc;
  if(writeDoc.Parse<0>(body_json.c_str()).HasParseError())
    throw BadRequestException();
  rapidjson::Value& users = readDoc[users_log.c_str()];
  rapidjson::Value& mac = writeDoc[mac_log.c_str()];
  std::string token = find_token(users, mac);
  if (token == "0"){ 
    users.PushBack(writeDoc.GetObject(), readDoc.GetAllocator());
    fp = fopen(user_log_path.c_str(), "w+");
    char buffer_writer[65536];
    rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    readDoc.Accept(writer);
    fclose(fp);    
  }
  return token;
}
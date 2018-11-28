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
      if (users[i][create_at_log.c_str()].IsUint64()) {
        if (time - users[i][create_at_log.c_str()].GetUint64() >= day) {
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
  rapidjson::Value& admin = d[admin_log.c_str()];
  if (old_password != (admin[password_log.c_str()].GetString()))
    throw UnauthorizedException();
  else if (new_password == "")
    throw BadRequestException();
  else {
    admin[password_log.c_str()].SetObject();
    admin[password_log.c_str()].SetString(new_password.c_str(), new_password.length());
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
  rapidjson::Document document;
  document.Parse<0>(body.c_str());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);
  std::string response_token = registerIds(buffer.GetString());
  return createIdentificationResponseJson(response_token, "Bienvenue sur l'application Café-Bistro Elevation !");
}

/**
 * search if the token exist in the log for the same mac
 * @param users list
 * @param mac
 * @return token
 */ 
unsigned int find_token(rapidjson::Value& users, std::string mac) {
  unsigned int token = 0;
  for (rapidjson::SizeType i = 0; i < users.Size(); i++) {
    std::string temp = users[i][mac_log.c_str()].GetString();
    if(!temp.compare(mac)) {
      token = users[i][token_log.c_str()].GetUint();
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
  mutex_user.lock();
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
  unsigned int token = find_token(users, mac.GetString());
  if (token == 0){ 
    User user(writeDoc.GetObject());
    token = user.token_;
    users_sign.push_back(user);
    add_user(user);
    users.PushBack(user.to_json().GetObject(), readDoc.GetAllocator());
    fp = fopen(user_log_path.c_str(), "w+");
    char buffer_writer[65536];
    rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    readDoc.Accept(writer);
    fclose(fp);    
  }
  mutex_user.unlock();
  return std::to_string(token);
}


/**
 * create the list of music with metadata file 
 * 
 */ 
void create_list_user() {
  std::cout << "Initialisation user" << std::endl;
  FILE* fp = fopen(user_log_path.c_str(), "rb"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);
  const rapidjson::Value& users = d[users_log.c_str()];
  for (rapidjson::SizeType i = 0; i < users.Size(); i++) {
    User user(users[i].GetObject());
    users_sign.push_back(user);
  }
  std::cout << "Liste de user bien initialisé" << std::endl;
}

std::vector<User> get_list_users() {
  return users_sign;
}

/**
 * let the admin login
 * @param username
 * @param password
 * @throw ForbiddenException (Error 403, requete non autorisé)
 */ 
void loginSupervisor(std::string username, std::string password) {
  rapidjson::Document d = getJsonFile(admin_log_path.c_str());
  rapidjson::Value& admin = d[admin_log.c_str()];
  if (username == admin[username_log.c_str()].GetString() && password == (admin[password_log.c_str()].GetString()))
    write_log("L'administrateur vient de se log"); 
  else {
    throw new ForbiddenException(); // Error 403, requete non autorisé
  }
}

/**
 * Save the fact than the admin is log
 * @param username
 */ 
void saveLogin(std::string username) {
  rapidjson::Document d = getJsonFile(admin_log_path.c_str());
  rapidjson::Value& admin = d[admin_log.c_str()];
  admin[isLog_log.c_str()].SetBool(true);
  writeJsonFile(admin_log_path.c_str(), d);
}

/**
 * logout the admin
 * @param username
 */
void logoutSupervisor(std::string username) {
  rapidjson::Document d = getJsonFile(admin_log_path.c_str());
  rapidjson::Value& admin = d[admin_log.c_str()];
  admin[isLog_log.c_str()].SetBool(false);
  writeJsonFile(admin_log_path.c_str(), d);
}

/**
 * check if the admin is login
 * @param username
 * @throw UnauthorizedException (Error 401 : Utilisateur non confirmé)
 */ 

void checkIfLogin(std::string username) {
  rapidjson::Document d = getJsonFile(admin_log_path.c_str());
  rapidjson::Value& admin = d[admin_log.c_str()];
  if (!admin[isLog_log.c_str()].GetBool())
    throw UnauthorizedException();
}


void lock_user(std::string mac, bool is_blocked) {
  std::cout << "lock " << std::endl;
  for (User user : users_sign)
    std::cout << user.to_string();
  mutex_user.lock();
  FILE* fp = fopen(user_log_path.c_str(), "rb");
  char buffer_reader[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document readDoc;
  readDoc.ParseStream(is);
  fclose(fp);
  rapidjson::Document writeDoc;
  rapidjson::Value& users = readDoc[users_log.c_str()];
  std::cout << "mac : " << mac << std::endl;
  unsigned int token = find_token(users, mac);
  std::cout << "token : " << token << std::endl;
  auto it_user = find(users_sign.begin(), users_sign.end(), token);
  if (it_user != users_sign.end()) {
    std::cout << "user : " << it_user->to_string() << std::endl;
    it_user->is_blocked_ = is_blocked;
    std::cout << "user block : " << it_user->is_blocked_ << std::endl;
    std::cout << "is_block value : " << is_blocked << std::endl;
    write_users(users_sign);
  }
  mutex_user.unlock();
  for (User user : users_sign)
    std::cout << user.is_blocked_ << std::endl;
}

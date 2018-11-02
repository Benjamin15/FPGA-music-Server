#include "ManagerUser.h"

void ManagerUser::identify(const std::shared_ptr< restbed::Session > session){
  std::cout << "identification" << std::endl;
}

void ManagerUser::lock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "bloquer" << std::endl;
}

void ManagerUser::unlock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "debloquer" << std::endl;
}

void ManagerUser::get_black_list(const std::shared_ptr< restbed::Session > session) {
  std::cout << "liste noire" << std::endl;
}

void ManagerUser::login(const std::shared_ptr< restbed::Session > session) {
  std::cout << "login" << std::endl;
}

void ManagerUser::logout(const std::shared_ptr< restbed::Session > session) {
  std::cout << "logout" << std::endl;
}

void ManagerUser::set_password(const std::shared_ptr< restbed::Session > session) {
  std::cout << "changement mot de passe" << std::endl;
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    std::string contentJson = "";
    for(auto byte : body){
      contentJson += byte;
    }
    rapidjson::Document document;
    bool isLogged=true;
    std::string codeMessage ="";
    int statusCode=0;
    document.SetObject();
    document.Parse<0>(contentJson.c_str(), contentJson.length());
    if(isLogged) {
      if (!document.HasParseError()) {
        std::string oldPwdFromRequest= document["ancien"].GetString();
        std::string newPwdFromRequest= document["nouveau"].GetString();
        rapidjson::Document d= getJsonFile("metadata/admin.json");
        rapidjson::SizeType i = 0;
        rapidjson::Value& admin = d["admin"];
        if (oldPwdFromRequest!= (admin[i]["mot_de_passe"].GetString())) {
          codeMessage= CodeMessage::UNAUTHORIZED;
        }
        else if (newPwdFromRequest == "") {
           codeMessage= CodeMessage::BAD_REQUEST;
        }
        else {
          admin[i]["mot_de_passe"].SetObject();
          admin[i]["mot_de_passe"].SetString(newPwdFromRequest.c_str(), newPwdFromRequest.length());
          writeJsonFile("metadata/admin.json", d);
        }
      }
      else {
        codeMessage= CodeMessage::BAD_REQUEST;
      }
    }
    else {
     codeMessage= CodeMessage::UNAUTHORIZED;
    }
    statusCode=ManagerUser::getStatusCode(codeMessage);
    session->close(statusCode, codeMessage, {{"Content-Length", std::to_string(codeMessage.length())},
      {"Connection","close"}});
  });
}

int ManagerUser::getStatusCode(std::string codeMessage) {
  if (codeMessage==CodeMessage::BAD_REQUEST){
    return restbed::BAD_REQUEST;
  }
  else if (codeMessage==CodeMessage::UNAUTHORIZED) {
    return restbed::UNAUTHORIZED;
  }
  else {
    return restbed::OK;
  }
}


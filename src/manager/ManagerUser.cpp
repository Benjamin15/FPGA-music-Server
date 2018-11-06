#include "ManagerUser.h"

void ManagerUser::identify(const std::shared_ptr< restbed::Session > session){
  std::cout << "identification en cours" << std::endl;
  const auto request = session->get_request();
  size_t content_length = request->get_body().size();
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
     const std::string parameter = session->get_request()->get_query_parameter("name","Error getting Query parameter");
     rapidjson::Document document;
     document.Parse(parameter.c_str());
     srand(time(NULL));
     int token = rand();
     std::time_t time = std::time(0);
     struct tm *aTime = localtime(&time);
     int hour=aTime->tm_hour;
     document.AddMember("Token", token, document.GetAllocator());
     document.AddMember("Time", hour, document.GetAllocator());
     rapidjson::StringBuffer buffer;
     rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
     document.Accept(writer);
     std::string compare = registerIds(buffer.GetString());
     std::string response;
     if(!compare.compare("")){
       response = createIdentificationResponseJson(std::to_string(token), "Bienvenue sur l'application Café-Bistro Elevation !");
       session->close(restbed::OK,response,{{"Content-Length",std::to_string(response.size())},
       {"Connection","close"}});
     }else {
      response = createIdentificationResponseJson(compare, "Bienvenue sur l'application Café-Bistro Elevation !");
      session->close(restbed::OK,response,{{"Content-Length",std::to_string(response.size())},
      {"Connection","close"}});
     }
  });
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
}
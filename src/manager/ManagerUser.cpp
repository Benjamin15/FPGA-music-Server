#include "ManagerUser.h"
#include "ManagerIdentification.h"



void ManagerUser::identify(const std::shared_ptr< restbed::Session > session){
  const auto request = session->get_request();
  std::string content_length = request->get_header("Content-Length","");
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    ManagerIdentification::registerId(std::string(body.begin(),body.end()));
    std::string result = "Ok";
    session->close(restbed::OK,"",{{"Content-Length",std::to_string(result.size())},
    {"Connection","close"}});
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
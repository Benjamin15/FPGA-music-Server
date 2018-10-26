#include "ManagerUser.h"

void ManagerUser::identify(const std::shared_ptr< restbed::Session > session){
  std::cout << "identification" << std::endl;
  std::cout << "header" << std::endl;
 // std::cout << session->get_headers(). << std::endl;
  std::cout << "request" << std::endl;
  const auto request = session->get_request( );
  const auto body = request->get_body( );
  std::string param1 = request->get_path_parameter("param1");
  std::string param2 = request->get_path_parameter("param2");
  std::cout << "param1 : " << param1 << " ; param2 : " << param2 << std::endl; 
  fprintf( stdout, "Complete body content: %.*s\n", static_cast< int >( body.size( ) ), body.data( ) );

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
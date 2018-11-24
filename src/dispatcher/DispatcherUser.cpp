#include "DispatcherUser.h"

/**
 * endpoint to identify user
 * 
 */ 

void connect(const std::shared_ptr< restbed::Session > session){
  std::cout << "Tentative d'identification" << std::endl;
  const auto request = session->get_request();
  size_t content_length = request->get_body().size();
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    const std::string body_parameter = session->get_request()->get_query_parameter("body","Error getting Query parameter");
    try {
      std::string response = sign_in(body_parameter);
      std::cout << "return response : " << response << std::endl;
      sendResponse(session, createOkResponse(response));
    } catch (BadRequestException& error) {
      std::cout << "error" << std::endl;
      error.print_error();
      sendResponse(session, createBadRequestResponse());
    }
  });
}

/**
 * endpoint to lock user 
 * 
 */

void lock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "bloquer" << std::endl;
}

/**
 * endpoint to unlock user 
 * 
 */ 

void unlock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "debloquer" << std::endl;
}

/**
 * endpoint to get the black list of user
 * 
 */

void get_black_list(const std::shared_ptr< restbed::Session > session) {
  std::cout << "liste noire" << std::endl;
}


/**
 * endpoint to login the admin
 * 
 */ 

void login(const std::shared_ptr< restbed::Session > session) {
  std::cout << "login" << std::endl;
}


/**
 * endpoint to logout the admin
 * 
 */

void logout(const std::shared_ptr< restbed::Session > session) {
  std::cout << "logout" << std::endl;
}


/**
 * 
 * endpoint to change the password of the admin user
 * 
 */ 
void set_password(const std::shared_ptr< restbed::Session > session) {
  std::cout << "Tentative changement mot de passe" << std::endl;
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    const std::string old_password = "ancien";
    const std::string new_password = "nouveau";
    std::string contentJson = std::string(body.begin(), body.end());
    rapidjson::Document document;
    document.SetObject();
    document.Parse<0>(contentJson.c_str(), contentJson.length());
    try {
      if (!document.HasParseError())
        throw BadRequestException();
      update_password(document[old_password.c_str()].GetString(), document[new_password.c_str()].GetString());
      sendResponse(session, createOkResponse(responseBody::OK));
    } catch (BadRequestException& error) {
      error.print_error();
      sendResponse(session, createBadRequestResponse(responseBody::BAD_REQUEST_V2));
    } catch (UnauthorizedException& error) {
      error.print_error();
      sendResponse(session, createUnauthorizedResponse());
    }
  });
}

/**
 * endpoint for get all users (admin function)
 * 
 */  
void get_users(const std::shared_ptr< restbed::Session > session) {
  std::cout << "get users " << std::endl;
}
#include "DispatcherUser.h"

void connect(const std::shared_ptr< restbed::Session > session){
  std::cout << "Tentative d'identification" << std::endl;
  const auto request = session->get_request();
  size_t content_length = request->get_body().size();
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    const std::string body_parameter = session->get_request()->get_query_parameter("body","Error getting Query parameter");
    try {
      std::string response = sign_in(body_parameter);
      std::cout << "return response" << std::endl;
      sendResponse(session, createOkResponse(response));
    } catch (BadRequestException& error) {
      std::cout << "error" << std::endl;
      error.print_error();
      sendResponse(session, createBadRequestResponse());
    }
  });
}

void lock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "bloquer" << std::endl;
}

void unlock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "debloquer" << std::endl;
}

void get_black_list(const std::shared_ptr< restbed::Session > session) {
  std::cout << "liste noire" << std::endl;
}

void login(const std::shared_ptr< restbed::Session > session) {
  std::cout << "login" << std::endl;
}

void logout(const std::shared_ptr< restbed::Session > session) {
  std::cout << "logout" << std::endl;
}

void set_password(const std::shared_ptr< restbed::Session > session) {
  std::cout << "Tentative changement mot de passe" << std::endl;
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    std::string contentJson = std::string(body.begin(), body.end());
    rapidjson::Document document;
    document.SetObject();
    document.Parse<0>(contentJson.c_str(), contentJson.length());
    try {
      if (!document.HasParseError())
        throw BadRequestException();
      update_password(document["ancien"].GetString(), document["nouveau"].GetString());
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


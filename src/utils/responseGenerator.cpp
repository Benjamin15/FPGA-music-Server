#include "responseGenerator.h"


  std::shared_ptr<restbed::Response> createResponse(int statusCode, const std::string& body){
  std::shared_ptr<restbed::Response> response = std::make_shared<restbed::Response>();
  response->set_status_code(statusCode);
  response->set_body(body);
  response->set_headers(createResponseHeaders(body.size()));
  return response;
}

std::shared_ptr<restbed::Response> createOkResponse(const std::string& body){
  return createResponse(restbed::OK, body);
} 

std::shared_ptr<restbed::Response> createBadRequestResponse(const std::string& body){
  std::cout << "Requete json non valide" << std::endl;
  return createResponse(restbed::BAD_REQUEST, body);
}

std::shared_ptr<restbed::Response> createUnauthorizedResponse(const std::string& body){
  std::cout << "L'utilisateur n'a pas l'autorisation necessaire" << std::endl;
  return createResponse(restbed::UNAUTHORIZED, body);
} 

std::shared_ptr<restbed::Response> createForbiddenResponse(const std::string& body){
  std::cout << "L'acces a été refusé" << std::endl;
  return createResponse(restbed::FORBIDDEN, body);
}

std::shared_ptr<restbed::Response> createMethodNotAllowedResponse(const std::string& body){
  std::cout << "La chanson ne peut pas être supprimer" << std::endl;
  return createResponse(restbed::METHOD_NOT_ALLOWED, body);
}

std::shared_ptr<restbed::Response> createConflictResponse(const std::string& body){
  std::cout << "L'une des chansons ne se trouve plus dans la file d'attente" << std::endl;
  return createResponse(restbed::CONFLICT, body);
}

std::shared_ptr<restbed::Response> createRequestEntityTooLargeResponse(const std::string& body){
  std::cout << "La chanson est trop longue ou la file est pleine" << std::endl;
  return createResponse(restbed::REQUEST_ENTITY_TOO_LARGE, body);
}

std::shared_ptr<restbed::Response> createUnsupportedMediaTypeResponse(const std::string& body){
  std::cout << "Veuillez choisir un fichier mp3 valide" << std::endl;
  return createResponse(restbed::UNSUPPORTED_MEDIA_TYPE, body);
} 

std::shared_ptr<restbed::Response> createInternalServerErrorResponse(const std::string& body){
  std::cout << "Le server ne repond pas" << std::endl;
  return createResponse(restbed::INTERNAL_SERVER_ERROR, body);
}


std::multimap< std::string, std::string > createResponseHeaders(size_t contentLength){
  return {{"Content-Length",std::to_string(contentLength)}, {"Connection","close"}};
}

void sendResponse(const std::shared_ptr< restbed::Session >& session, const std::shared_ptr<restbed::Response>& response){
  session->close(*response);
}


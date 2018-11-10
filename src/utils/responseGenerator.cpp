#include "responseGenerator.h"


  std::shared_ptr<restbed::Response> ResponseGenerator::createResponse(int statusCode, const std::string& body){
  std::shared_ptr<restbed::Response> response = std::make_shared<restbed::Response>();
  response->set_status_code(statusCode);
  response->set_body(body);
  response->set_headers(createResponseHeaders(body.size()));
  return response;
}

std::shared_ptr<restbed::Response> ResponseGenerator::createOkResponse(const std::string& body){
  return createResponse(restbed::OK, body);
} 

std::shared_ptr<restbed::Response> ResponseGenerator::createBadRequestResponse(const std::string& body){
  return createResponse(restbed::BAD_REQUEST, body);
}

std::shared_ptr<restbed::Response> ResponseGenerator::createUnauthorizedResponse(const std::string& body){
  return createResponse(restbed::UNAUTHORIZED, body);

} 

std::shared_ptr<restbed::Response> ResponseGenerator::createForbiddenResponse(const std::string& body){
  return createResponse(restbed::FORBIDDEN, body);
}

std::shared_ptr<restbed::Response> ResponseGenerator::createMethodNotAllowedResponse(const std::string& body){
  return createResponse(restbed::METHOD_NOT_ALLOWED, body);
}

std::shared_ptr<restbed::Response> ResponseGenerator::createConflictResponse(const std::string& body){
  return createResponse(restbed::CONFLICT, body);
}

std::shared_ptr<restbed::Response> ResponseGenerator::createRequestEntityTooLargeResponse(const std::string& body){
  return createResponse(restbed::REQUEST_ENTITY_TOO_LARGE, body);
}

std::shared_ptr<restbed::Response> ResponseGenerator::createUnsupportedMediaTypeResponse(const std::string& body){
  return createResponse(restbed::UNSUPPORTED_MEDIA_TYPE, body);
} 

std::shared_ptr<restbed::Response> ResponseGenerator::createInternalServerErrorResponse(const std::string& body){
  return createResponse(restbed::INTERNAL_SERVER_ERROR, body);
}


std::multimap< std::string, std::string > ResponseGenerator::createResponseHeaders(size_t contentLength){
  return {{"Content-Length",std::to_string(contentLength)}, {"Connection","close"}};
}

void ResponseGenerator::sendResponse(const std::shared_ptr< restbed::Session >& session, const std::shared_ptr<restbed::Response>& response){
  session->close(*response);
}


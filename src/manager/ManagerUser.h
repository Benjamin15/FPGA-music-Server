#ifndef MANAGE_USER_H
#define MANAGE_USER_H
#include <memory>
#include <chrono>
#include <restbed>
#include <iostream>
#include "../utils/json.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "../utils/logSingleton.h"
#include "../utils/responseGenerator.h"


/**
 * Class which manage all users
 */  
class ManagerUser
{  
  private:
   
  public:
    static void identify(const std::shared_ptr< restbed::Session > session);
    static void lock(const std::shared_ptr< restbed::Session > session);
    static void unlock(const std::shared_ptr< restbed::Session > session);
    static void get_black_list(const std::shared_ptr< restbed::Session > session);
    static void login(const std::shared_ptr< restbed::Session > session);
    static void logout(const std::shared_ptr< restbed::Session > session);
    static void set_password(const std::shared_ptr< restbed::Session > session);
    static int getStatusCode(std::string codeMessage);
};
#endif

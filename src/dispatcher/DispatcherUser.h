#pragma once
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
#include "../exception/Exception.h"
#include "../manager/ManagerUser.h"

void connect(const std::shared_ptr< restbed::Session > session);
void lock(const std::shared_ptr< restbed::Session > session);
void unlock(const std::shared_ptr< restbed::Session > session);
void get_black_list(const std::shared_ptr< restbed::Session > session);
void login(const std::shared_ptr< restbed::Session > session);
void logout(const std::shared_ptr< restbed::Session > session);
void set_password(const std::shared_ptr< restbed::Session > session);
int getStatusCode(std::string codeMessage);
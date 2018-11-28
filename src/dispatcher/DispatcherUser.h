#pragma once

#include <memory>
#include <chrono>
#include <restbed>
#include <iostream>
#include "../header/rapidjson.h"
#include "../utils/utils.h"
#include "../exception/exception.h"
#include "../manager/ManagerUser.h"

void connect(const std::shared_ptr< restbed::Session > session);
void lock(const std::shared_ptr< restbed::Session > session);
void unlock(const std::shared_ptr< restbed::Session > session);
void get_black_list(const std::shared_ptr< restbed::Session > session);
void login(const std::shared_ptr< restbed::Session > session);
void logout(const std::shared_ptr< restbed::Session > session);
void set_password(const std::shared_ptr< restbed::Session > session);
void get_users(const std::shared_ptr< restbed::Session > session);

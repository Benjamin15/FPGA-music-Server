#pragma once

#include <memory>
#include <chrono>
#include <restbed>
#include <iostream>
#include <mutex>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "../utils/utils.h"
#include "../exception/exception.h"
#include "../model/User.h"
#include "../metadata/admin_log.h"
#include "../metadata/users_log.h"
#include "ManagerStatistics.h"


bool checkUserToken(unsigned int token);
bool isValidToken(unsigned int token);
bool identify(unsigned int token);
User get_user(unsigned int token);
std::vector<User> get_list_users();
void update_password(std::string old_password, std::string new_password);
std::string sign_in(std::string body);
std::string registerIds(std::string body_json);
void create_list_user();
void loginSupervisor(std::string username, std::string password);
void saveLogin(std::string username);
void logoutSupervisor(std::string username);
void checkIfLogin(std::string username);
void lock_user(std::string body_json, bool is_blocked);

static std::vector<User> users_sign;
static std::mutex mutex_user;


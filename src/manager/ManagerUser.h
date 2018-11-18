#pragma once

#include <memory>
#include <chrono>
#include <restbed>
#include <iostream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "../utils/utils.h"
#include "../exception/exception.h"

bool checkUserToken(unsigned int token);
bool isValidToken(unsigned int token);
bool identify(unsigned int token);
User get_user_for_sent_music(unsigned int token);
void update_password(std::string old_password, std::string new_password);
std::string sign_in(std::string body);

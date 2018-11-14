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
#include "../exception/UnauthorizedException.h"
#include "../exception/BadRequestException.h"

std::string sign_in(std::string body);
User get_user_for_sent_music(int token);
void update_password(std::string old_password, std::string new_password);
bool identify(int token);
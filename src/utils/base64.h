#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>  

#include "../header/rapidjson.h"
#include "../header/boost.h"

std::string base64_decode(std::string encoded_string);
inline bool is_base64(unsigned char c);
void base64_toBinary(std::string input,std::string title);
const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
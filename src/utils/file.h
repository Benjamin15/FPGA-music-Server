
#pragma once

#include <string>
#include <iostream>
#include "../header/taglib.h"
#include "../model/Music.h"
#include "../exception/UnsupportedException.h"

void removeMP3Selected(const std::string no);
Music get_info(std::string path);
bool checkIfMp3(std::string path);


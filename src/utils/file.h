
#ifndef JSON_H
#define JSON_H
#include <string>
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include "../model/Music.h"

void removeMP3Selected(const std::string no);
Music get_info(std::string path);
bool checkIfMp3(std::string path);

#endif

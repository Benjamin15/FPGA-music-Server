#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "../model/Music.h"

rapidjson::Document getJsonFile(const char* path);
std::string getListForUser(std::vector<Music> musics);

#endif
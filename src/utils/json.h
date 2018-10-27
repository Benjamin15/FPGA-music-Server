#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include "rapidjson/document.h"
#include "../model/Music.h"

rapidjson::Document getJsonFile(const char* path);
std::string getListForUser(std::vector<Music> musics);
std::string getListForAdmin(std::vector<Music> musics);
void remove_last_music();
void registerIds(std::string parameter);

#endif
#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include "rapidjson/document.h"
#include "../model/Music.h"

rapidjson::Document getJsonFile(const char* path);
void writeJsonFile(const char* path, const rapidjson::Document& d);
std::string getListForUser(std::vector<Music> musics);
std::string getListForAdmin(std::vector<Music> musics);
void remove_last_music();
std::string removeMusicSelected(const unsigned int idMusic, const unsigned int noMusic);
void removeMP3Selected(const std::string titre);
#endif

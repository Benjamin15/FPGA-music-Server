#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <chrono>
#include "rapidjson/document.h"
#include "../model/Music.h"

rapidjson::Document getJsonFile(const char* path);
void writeJsonFile(const char* path, const rapidjson::Document& d);
std::string getListForUser(std::vector<Music> musics);
std::string getListForAdmin(std::vector<Music> musics);
void remove_last_music();
std::string registerIds(std::string parameter);
void registerMusic(Music music);
std::string createIdentificationResponseJson(std::string token, std::string message);
void removeMusicSelected(const unsigned int idMusic, const unsigned int noMusic);
void removeMP3Selected(const std::string titre);
void write_music(const std::vector<Music> musics);
bool isValidToken(int token);
#endif

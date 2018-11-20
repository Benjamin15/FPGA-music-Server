#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../header/rapidjson.h"
#include "../model/Music.h"
#include "../exception/exception.h"
#include "../metadata/metadata.h"

rapidjson::Document getJsonFile(const char* path);
void writeJsonFile(const char* path, const rapidjson::Document& d);
std::string getListForUser(std::vector<Music> musics);
std::string getListForAdmin(std::vector<Music> musics);
void remove_last_music();
std::string registerIds(std::string body_json);
void registerMusic(Music music);
std::string createIdentificationResponseJson(std::string token, std::string message);
void removeMusicSelected(const unsigned int noMusic);
void write_music(std::vector<Music> musics);
bool isValidToken(int token);
std::string getStats(int n_music, int n_user, int n_music_remove, std::string avg_duration);
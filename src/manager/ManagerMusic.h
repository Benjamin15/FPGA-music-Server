#pragma once

#include <memory>
#include <restbed>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <mutex>
#include "../header/rapidjson.h"
#include "../model/Music.h"
#include "../utils/json.h"

void insert(Music music);
void remove(unsigned int no);
void create_list_music();
void updateMusicsOwner(unsigned int token);
void reverse(unsigned int noFirst, unsigned int noSecond);
bool checkUserMusics(unsigned int token);
bool supressionPermission(unsigned int noMusic, unsigned int token);
bool checkListSize();
std::vector<Music> getMusics();

static std::vector<Music> musics;
static std::mutex mutex;

const int MAX_LIST_SIZE = 10;
const int MAX_USER_MUSICS = 5;

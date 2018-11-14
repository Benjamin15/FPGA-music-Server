#pragma once
#include <memory>
#include <restbed>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "../model/Music.h"
#include "../utils/json.h"

void insert(Music music);
void remove(int no);
void create_list_music();
void updateMusicsOwner(int token);
void reverse(int noFirst, int noSecond);
bool checkUserMusics(int token);
bool supressionPermission(int noMusic, int token);

std::vector<Music> musics;
std::mutex mutex;

const int MAX_LIST_SIZE = 10;
const int MAX_USER_MUSICS = 5;
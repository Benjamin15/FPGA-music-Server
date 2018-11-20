#pragma once

#include <memory>
#include <restbed>
#include <iostream>
#include <vector>
#include <mutex>

#include "../model/User.h"
#include "../model/Music.h"

void reset();
void add_user(User user);
void add_music(Music music);
void new_remove_music();
int get_n_users();
int get_n_musics();
int get_n_music_remove_admin();
int get_avg_time();

static std::vector<User> users;
static std::vector<Music> musics;
static int n_music_remove_admin = 0;
static std::mutex mutex;

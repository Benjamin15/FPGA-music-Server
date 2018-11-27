#pragma once

#include <memory>
#include <restbed>
#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>

#include "../model/User.h"
#include "../model/Music.h"

/**
 * Class which manage all users
 */  

void reset_stats();
void add_user(User user);
void add_music(Music music);
void add_remove_music();
int get_n_users();
int get_n_musics();
int get_n_music_remove_admin();
std::string get_avg_time();

static std::vector<User> users_stats;
static std::vector<Music> musics_stat;
static int n_music_remove_admin = 0;
static std::mutex mutex_stat;

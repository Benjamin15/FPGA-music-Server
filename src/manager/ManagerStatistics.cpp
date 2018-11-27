#include "ManagerStatistics.h"

/**
 * reset stats each 24 hours
 * 
 */ 
void reset_stats() {
  mutex.lock();
  users.clear();
  musics.clear();
  n_music_remove_admin = 0;
  mutex.unlock();
}

/**
 * add new user when they add a music, for each day
 * 
 */ 
void add_user(User user) {
  if(std::find(users.begin(), users.end(), user) == users.end()) {
     mutex.lock();
    users.push_back(user);
    mutex.unlock();
  } 
}

/**
 * add a new music for each day
 * 
 */ 
void add_music(Music music) {
  mutex.lock();
  musics.push_back(music);
  mutex.unlock();
}

/**
 * we need to increase the number of music remove by the admin. Reset at each day
 * 
 */ 
void add_remove_music() {
  n_music_remove_admin++;
}


/**
 * get count user
 * 
 */ 
int get_n_users() {
  return users.size();
}

/**
 * get count music
 * 
 */ 
int get_n_musics() {
  return musics.size();
}

/**
 * get number of music remove by the admin
 * 
 */ 
int get_n_music_remove_admin() {
  return n_music_remove_admin;
}

/**
 * get the average time of music. Reset at each day
 * 
 */ 
std::string get_avg_time() {
  time_t sum_time;
  for (Music music : musics) {
    struct tm duration;
    strptime(music.duration_.c_str(), "%M:%S", &duration);
    sum_time += mktime(&duration);  
  }
  time_t avg_time = (sum_time / musics.size());
  struct tm* avg_time_tm = localtime(&avg_time);
  return avg_time_tm->tm_min + ":"+avg_time_tm->tm_sec;
}
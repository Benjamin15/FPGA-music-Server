#include "ManagerStatistics.h"

/**
 * reset stats each 24 hours
 * 
 */ 
void reset_stats() {
  mutex_stat.lock();
  musics_stat.clear();
  n_music_remove_admin = 0;
  mutex_stat.unlock();
}

int get_n_users() {
  return users_stats.size();
}

void add_user(User user) {
  if (std::find(users_stats.begin(), users_stats.end(), user) == users_stats.end()) {
    mutex_stat.lock();
    users_stats.push_back(user);
    mutex_stat.unlock();
  } 
}

/**
 * add a new music for each day
 * 
 */ 
void add_music(Music music) {
  mutex_stat.lock();
  musics_stat.push_back(music);
  mutex_stat.unlock();
}

/**
 * we need to increase the number of music remove by the admin. Reset at each day
 * 
 */ 
void add_remove_music() {
  n_music_remove_admin++;
}

/**
 * get count music
 * 
 */ 
int get_n_musics() {
  return musics_stat.size();
}

/**
 * get number of music remove by the admin
 * 
 */ 
int get_n_music_remove_admin() {
  return n_music_remove_admin;
}

std::string calcul_avg() {
  int seconds = 0;
  int minutes = 0;
  for (Music music : musics_stat) {
    struct tm duration;
    std::cout << "duration : " << music.duration_.c_str() << std::endl;
    strptime(music.duration_.c_str(), "%M:%S", &duration);
    seconds += duration.tm_sec;
    minutes += duration.tm_min;
  }
  minutes += (seconds / 60);
  seconds %= 60;
  seconds /= musics_stat.size();
  minutes /= musics_stat.size();
  std::string seconds_string = std::to_string(seconds);
  std::string minutes_string = std::to_string(minutes);
  if (seconds_string.size() == 1)
    seconds_string = std::string(1, '0') + seconds_string;
  if (minutes_string.size() == 1)
    minutes_string = std::string(1, '0') + minutes_string;
  return  minutes_string + ":" + seconds_string;
}

/**
 * get the average time of music. Reset at each day
 * 
 */ 
std::string get_avg_time() {
  std::string avg_time = calcul_avg();
  std::cout << "avg time : " << avg_time << std::endl;
  return avg_time;
}
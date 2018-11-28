#include "ManagerMusic.h"

/**
 * insert a new music in the list
 * @param music
 * 
 */ 
void insert(Music music) {
  mutex.lock();
  registerMusic(music);
  musics.push_back(music);
  add_music(music);
  mutex.unlock();
}

/**
 * remove a music in the list
 * @param no of music
 * 
 */ 
void remove(unsigned int no) {
  std::size_t i = 0;
  mutex.lock();
  while (i < musics.size()) {
    if (musics[i].no_ == no) {
      musics.erase(musics.begin() + i);
      removeMP3Selected(std::to_string(no));
    }
    i++;
  }
  mutex.unlock();
}

/**
 * reverse music
 * @param first numero of music
 * @param second numero of music
 * 
 */ 
void reverse(unsigned int noFirst, unsigned int noSecond) {
  std::cout << "debut : " << std::endl;
  for (Music music : musics) 
    std::cout << music.no_ << std::endl;
  mutex.lock();
  std::rotate(find(musics.begin(), musics.end(), noFirst), find(musics.begin(), musics.end(), noFirst) + 1, find(musics.begin(), musics.end(), noSecond) + 1);
  write_music(musics);
  mutex.unlock();
  std::cout << "fini ! " << std::endl;
  for (Music music : musics) 
    std::cout << music.no_ << std::endl;
}

/**
 * update the owner music
 * @param token 
 */ 
void updateMusicsOwner(unsigned int token) {
  for (std::size_t i = 0; i < musics.size() ; i++ ) {
    if (musics[i].user_.token_ == token ) {
      musics[i].owner_ = true;
    }
  }
}


/**
 *  check the count of music than have an user
 *  @parma token
 *  @return nb music
 */ 

bool checkUserMusics(unsigned int token) {
  int userMusics = 0;
  for (std::size_t i = 0 ; i < musics.size() ; i++) {
    if (musics[i].user_.token_ == token ){
      userMusics ++;
    }
  }
  return userMusics < MAX_USER_MUSICS;
}

/**
 * create the list of music with metadata file 
 * 
 */ 
void create_list_music() {
  FILE* fp = fopen(music_json_path.c_str(), "rb"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);
  const rapidjson::Value& musiques = d[musics_log.c_str()];
  for (rapidjson::SizeType i = 0; i < musiques.Size(); i++) {
    Music music(musiques[i].GetObject());
    musics.push_back(music);
    add_music(music);
    add_user(music.user_);
  }
  std::cout << "Liste de musique bien initialisé" << std::endl;
}

/**
 * check if we can remove an music
 * @param noMusic
 * @param token
 * @return canRemove
 */ 
bool canRemove(unsigned int noMusic, unsigned int token) {
  for(size_t i = 0 ; i < musics.size() ; i++) {
    if (musics[i].no_ == noMusic) {
      if (musics[i].user_.token_ == token) {
        return true;
      }
      return false;
    }
  }
  return false;
}

/**
 * check if the list of the music is correct
 * @return isCorrect
 */ 
bool checkListSize() {
  return musics.size() < MAX_LIST_SIZE;
}


/**
 * get the list of music
 * @return list of music
 */ 
std::vector<Music> getMusics() {
  return musics;
}
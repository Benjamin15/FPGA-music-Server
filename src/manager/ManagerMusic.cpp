#include "ManagerMusic.h"

void insert(Music music) {
  mutex.lock();
  musics.push_back(music);
  mutex.unlock();
}

void remove(int no) {
  int i = 0;
  mutex.lock();
  while (i < musics.size()) {
    if (musics[i].no_ == no) {
      musics.erase(musics.begin() + i);
    }
    i++;
  }
  mutex.unlock();
}

void reverse(int noFirst, int noSecond) {
  mutex.lock();
  std::iter_swap(find(musics.begin(), musics.end(), noFirst), find(musics.begin(), musics.end(), noSecond));
  write_music(musics);
  mutex.unlock();
}

void updateMusicsOwner(int token) {
  for(int i = 0; i< musics.size();i++ ){
    if(musics[i].user_.id_ == token ){
      musics[i].owner_ = true;
    }
  }
}


bool checkUserMusics(int token) {
  int userMusics = 0;
  for(int i=0;i< musics.size();i++){
    if(musics[i].user_.id_== token ){
      userMusics ++;
    }
  }
  return userMusics < MAX_USER_MUSICS;
}


void create_list_music() {
  FILE* fp = fopen("metadata/musiques.json", "rb"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);
  const rapidjson::Value& musiques = d["musiques"];
  for (rapidjson::SizeType i = 0; i < musiques.Size(); i++) {
    std::string mac = musiques[i]["MAC"].GetString();
    int idUser = musiques[i]["id"].GetUint();
    std::string suggestBy = musiques[i]["proposeePar"].GetString();
    std::string ip = musiques[i]["ip"].GetString();
    int idMusic = musiques[i]["no"].GetUint();
    std::string duration = musiques[i]["duree"].GetString();
    std::string artist = musiques[i]["artiste"].GetString();
    std::string title = musiques[i]["titre"].GetString();
    User user(idUser, suggestBy, ip, mac);
    Music music(idMusic, title, artist, duration, user);
    musics.push_back(music);
    }
  std::cout << "Liste de musique bien initialisé" << std::endl;
}


bool supressionPermission(int noMusic, int token){
  for(int i=0;i< musics.size();i++) {
    if(musics[i].no_== noMusic){
      if(musics[i].user_.id_ == token){
        return true;
      }
      return false;
    }
  }
  return false;
}
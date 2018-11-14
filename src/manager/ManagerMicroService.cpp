#include "ManagerMicroService.h"
#include <mutex>

std::thread ManagerMicroService::thread_manager_player_;
std::thread ManagerMicroService::thread_insert_music_;
std::mutex ManagerMicroService::mutex;
void ManagerMicroService::manage_player() {
  while(1) {
    if (musics.size() > 0) {
      pid_t pid = fork();
      if (pid == 0) {
        const char* player_path = "./player";
        const std::string id = std::to_string(musics[0].no_);
        std::string path_music = "metadata/musique/" + id + ".mp3";
        std::cout << "lancement de la chanson : " << id << std::endl;
        const char* parmList[] = {"./player", path_music.c_str(), NULL};
        SysLoggerSingleton::GetInstance().WriteLine("Debut du decodage de la chanson: " + id);
        execv(player_path, (char **) parmList);
      }
      wait(NULL);
      std::cout << "fin musique, suppression" << std::endl;
      musics.erase(musics.begin());
      remove_last_music();
    
    }
  }
}

void ManagerMicroService::manage_insertion_music(const std::shared_ptr< restbed::Session > session,const restbed::Bytes& body) {
  const std::string id = session->get_request()->get_path_parameter( "id" );
  const std::string musicTitle = session->get_request()->get_query_parameter( "title","Error getting Query parameter" );
  std::string bodyString(body.begin(), body.end());
  ManagerMicroService::mutex.lock();
  if(!checkUserToken(std::stoi(id)) && !isValidToken(std::stoi(id))){
    sendResponse(session, createForbiddenResponse());
  }
  else if (checkListSize() && checkUserMusics(std::stoi(id))
      && checkUserToken(std::stoi(id))){
    std::string mp3EncodedMusic = bodyString;
    std::string mp3DecodedMusic = base64_decode(mp3EncodedMusic);
    std::string fileName = std::to_string(Music::getNextMusicId("metadata/musiques.json"))+".mp3";
    base64_toBinary(mp3DecodedMusic,fileName);
    std::string path = "metadata/musique/" + fileName;
    if(!checkIfMp3(path)){
      sendResponse(session,createUnsupportedMediaTypeResponse());
    }
    Music music = get_info(path);
    User user = get_user_for_sent_music(std::stoi(id));
    music.setMusicUser(user);
    if(music.title_ == ""){
      music.setMusicTitle(musicTitle);
    }
    music.setMusicNumber("metadata/musiques.json");
    registerMusic(music);
    musics.push_back(music);
    ManagerMicroService::mutex.unlock();
    std::cout << "Soumission d'une nouvelle chanson: " + musicTitle << std::endl;
    SysLoggerSingleton::GetInstance().WriteLine("Soumission d'une nouvelle chanson: " + musicTitle);
    sendResponse(session, createOkResponse());
  }else if(!checkListSize() || !checkUserMusics(std::stoi(id)) ){
    sendResponse(session, createRequestEntityTooLargeResponse());
  }else{
    sendResponse(session, createInternalServerErrorResponse());
  }
}

int ManagerMicroService::run_player() {
  thread_manager_player_ = std::thread(manage_player);
}

int ManagerMicroService::insert_music(const std::shared_ptr< restbed::Session > session,const restbed::Bytes& body) {
  thread_insert_music_ = std::thread(manage_insertion_music,session,body);
  thread_insert_music_.detach();
}
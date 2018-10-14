#include "ManagerMicroService.h"

std::thread ManagerMicroService::thread_manager_player_;

void ManagerMicroService::manage_player() {
  while(1) {
    if (ManagerMusic::musics.size() > 0) {
      pid_t pid = fork();
      if (pid == 0) {
        const char* player_path = "./player";
        const std::string title = ManagerMusic::musics[ManagerMusic::musics.size() - 1].title_;
        std::string path_music = "metadata/musique/" + title  + ".mp3";
        std::cout << "lancement de la chanson : " << title << std::endl;
        const char* parmList[] = {"./player", path_music.c_str(), NULL};
        execv(player_path, (char **) parmList);
      }
      wait(NULL);
      ManagerMusic::musics.pop_back();
      remove_last_music();
    }
  }
}
int ManagerMicroService::run_player() {
  thread_manager_player_ = std::thread(manage_player);
}
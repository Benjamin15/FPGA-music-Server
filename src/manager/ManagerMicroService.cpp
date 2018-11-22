#include "ManagerMicroService.h"

/**
 * It's the thread which run each musics.
 * 
 */ 
void manage_player() {
  while(1) {
    if (getMusics().size() > 0) {
      pid_t pid = fork();
      if (pid == 0) {
        const std::string id = std::to_string(getMusics()[0].no_);
        std::string path_music = music_path + id + mp3_ext;
        const char* parmList[] = {player_path.c_str(), path_music.c_str(), NULL};
        write_log("Debut du decodage de la chanson: " + id);
        execv(player_path.c_str(), (char **) parmList);
      }
      wait(NULL);
      std::cout << "fin musique, suppression" << std::endl;
      getMusics().erase(getMusics().begin());
      remove_last_music();
    }
  }
}

/**
 * run the thread which run the player
 *
 */  
void run_player() {
  std::thread manage_player;
}

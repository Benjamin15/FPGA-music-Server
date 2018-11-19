#include "ManagerMicroService.h"

void manage_player() {
  while(1) {
    if (getMusics().size() > 0) {
      pid_t pid = fork();
      if (pid == 0) {
        const char* player_path = "./player";
        const std::string id = std::to_string(getMusics()[0].no_);
        std::string path_music = "metadata/musique/" + id + ".mp3";
        std::cout << "lancement de la chanson : " << id << std::endl;
        const char* parmList[] = {"./player", path_music.c_str(), NULL};
        write_log("Debut du decodage de la chanson: " + id);
        execv(player_path, (char **) parmList);
      }
      wait(NULL);
      std::cout << "fin musique, suppression" << std::endl;
      getMusics().erase(getMusics().begin());
      remove_last_music();
    }
  }
}

void run_player() {
  std::thread(manage_player);
}

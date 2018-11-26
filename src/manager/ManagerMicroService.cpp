#include "ManagerMicroService.h"

/**
 * It's the thread which run each musics.
 * 
 */ 
void manage_player() {
  std::cout << "manage player" << std::endl;
  while(1) {
    if (getMusics().size() > 0) {
      std::cout << "devrait lancer une musique" << std::endl;
      pid_t pid = fork();
      if (pid == 0) {
        const std::string no = std::to_string(getMusics()[0].no_);
        std::string path_music = music_path + no + mp3_ext;
        Music music = get_info_for_player(path_music);
        std::string sample_rate = std::to_string(music.sample_rate_);
        std::string channel = std::to_string(music.channels_);
        std::string bitrate = std::to_string(music.bitrate_);
        std::cout << "path : " << path_music << std::endl;
        std::cout << "sample :"  << sample_rate << std::endl;
        std::cout << "channel :"  << channel << std::endl;
        std::cout << "bitrate :"  << bitrate << std::endl;
        const char* parmList[] = {
          player_path.c_str(),
          path_music.c_str(),
          sample_rate.c_str(),
          bitrate.c_str(),
          channel.c_str(),
          NULL};
        write_log("Debut du decodage de la chanson: " + no);
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
  std::cout << "run player " << std::endl;
  player = std::thread(manage_player);
}

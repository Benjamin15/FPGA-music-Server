#include "DispatcherStatistics.h"

/**
 * 
 * endpoint to get the statistics of the application
 * 
 */ 
void get_statistics(const std::shared_ptr< restbed::Session > session){
  //if ()      Si on n'est pas authentifié
  //  sendResponse() On retourne l'erreur 401
  
  std::cout << "statistiques" << std::endl;
  std::string avg_duration =  get_avg_time();
  int n_music = get_n_musics();
  int n_music_remove_by_admin = get_n_music_remove_admin();
  int n_user = get_n_users();
  std::string response = getStats(n_music, n_user, n_music_remove_by_admin, avg_duration);
  std::cout << "response : " << response << std::endl;
  sendResponse(session, createOkResponse(response));
}
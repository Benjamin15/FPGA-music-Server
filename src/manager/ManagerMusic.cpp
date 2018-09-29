#include "ManagerMusic.h"

void ManagerMusic::get_usager_files(const std::shared_ptr< restbed::Session > session) {
  std::cout << "liste musique de l'usager" << std::endl;
}

void ManagerMusic::insert_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "Insert musique" << std::endl;
}

void ManagerMusic::delete_usager__song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer musique utilisateur" << std::endl;
}

void ManagerMusic::get_superviser_files(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir musique superviseur" << std::endl;

}

void ManagerMusic::delete_superviser_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer song avec le superviseur" << std::endl;
}

void ManagerMusic::reverse_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "inverser musique" << std::endl;
}

void ManagerMusic::get_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir le volume" << std::endl;
}

void ManagerMusic::set_up_volume(const std::shared_ptr< restbed::Session > session) {
    std::cout << "augmenter le volume" << std::endl;
}

void ManagerMusic::set_down_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "diminuer le volume" << std::endl;
}

void ManagerMusic::enabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "activer mute" << std::endl;
}

void ManagerMusic::disabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "désactiver mute" << std::endl;
}


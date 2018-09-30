#ifndef MANAGE_MUSIC_H
#define MANGE_MUSIC_H
#include <memory>
#include <restbed>
#include <iostream>

/**
 * Class which manage all users
 */  
class ManagerMusic
{  
  private: 
  public:
    static void get_usager_files(const std::shared_ptr< restbed::Session > session);
    static void insert_song(const std::shared_ptr< restbed::Session > session);
    static void delete_usager__song(const std::shared_ptr< restbed::Session > session);

    static void get_superviser_files(const std::shared_ptr< restbed::Session > session);
    static void delete_superviser_song(const std::shared_ptr< restbed::Session > session);
    static void reverse_song(const std::shared_ptr< restbed::Session > session);
    static void get_volume(const std::shared_ptr< restbed::Session > session);
    static void set_up_volume(const std::shared_ptr< restbed::Session > session);
    static void set_down_volume(const std::shared_ptr< restbed::Session > session);
    static void enabledMute(const std::shared_ptr< restbed::Session > session);
    static void disabledMute(const std::shared_ptr< restbed::Session > session);
};
#endif
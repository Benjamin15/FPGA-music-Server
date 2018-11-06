#ifndef MANAGE_MUSIC_H
#define MANAGE_MUSIC_H
#include <memory>
#include <restbed>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include  <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <system_error>
#include "../model/Music.h"
#include "../utils/json.h"
#include "./ManagerMicroService.h"

#define gettid() syscall(SYS_gettid)

  /**
   * Class which manage all users
   */  
  class ManagerMusic
  {  
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

    static void create_list_music();
    static void launch_music();
    static Music get_info(std::string path);

    static std::vector<Music> musics;
  };
#endif
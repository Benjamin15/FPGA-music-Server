#pragma once
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
#include "../utils/base64.h"
#include "./ManagerMicroService.h"
#include "../utils/logSingleton.h"
#include "../utils/defaultResponseBody.h"
#include "../utils/responseGenerator.h"
#include "../manager/ManagerUser.h"
//#include "../utils/file.h"


#define gettid() syscall(SYS_gettid)

void get_usager_files(const std::shared_ptr< restbed::Session > session);
void insert_song(const std::shared_ptr< restbed::Session > session);
void delete_usager__song(const std::shared_ptr< restbed::Session > session);

void get_superviser_files(const std::shared_ptr< restbed::Session > session);
void delete_superviser_song(const std::shared_ptr< restbed::Session > session);
void reverse_song(const std::shared_ptr< restbed::Session > session);
void get_volume(const std::shared_ptr< restbed::Session > session);
void set_up_volume(const std::shared_ptr< restbed::Session > session);
void set_down_volume(const std::shared_ptr< restbed::Session > session);
void enabledMute(const std::shared_ptr< restbed::Session > session);
void disabledMute(const std::shared_ptr< restbed::Session > session);
void updateMusicsOwner(int userId);
bool checkListSize();
bool checkUserMusics(int userId);
bool checkUserToken(int token);
bool supressionPermission(int noMusic,int userId);
User get_user_for_sent_music(int userId);

void create_list_music();
void launch_music();
Music get_info(std::string path);
bool checkIfMp3(std::string path);


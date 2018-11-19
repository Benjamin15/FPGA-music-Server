#pragma once

#include <memory>
#include <restbed>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "../header/rapidjson.h"
#include "../header/taglib.h"
#include <system_error>
#include "../model/Music.h"
#include "../utils/utils.h"
#include "../manager/manager.h"

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

void launch_music();


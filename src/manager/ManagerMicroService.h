#pragma once

#include <memory>
#include <iostream>
#include <thread>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../utils/utils.h"
#include "ManagerMusic.h"

#define gettid() syscall(SYS_gettid)

void manage_player();
void run_player();

const std::string player_path = "./player";
const std::string music_path = "metadata/musics/";
const std::string mp3_ext = ".mp3";

static std::thread player;
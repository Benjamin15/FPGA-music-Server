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

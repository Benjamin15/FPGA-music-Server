#ifndef MANAGE_MICRO_SERVICE_H
#define MANAGE_MICRO_SERVICE_H
#include <memory>
#include <iostream>
#include <thread>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ManagerMusic.h"
#include "../utils/json.h"
#define gettid() syscall(SYS_gettid)


/**
 * Class which manage all microService
 */  
class ManagerMicroService
{  
 private:
  static void manage_player(); // boucle sur musics, waitpid() à la fin de la boucle
  static std::thread thread_manager_player_;
 public:
  static int run_player(); // il est appelé :) 
};
#endif
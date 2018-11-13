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
#include "../utils/logSingleton.h"

#define gettid() syscall(SYS_gettid)


/**
 * Class which manage all microService
 */  
class ManagerMicroService
{  
 private:
  static void manage_player();
  static void manage_insertion_music(const std::shared_ptr< restbed::Session > session,const restbed::Bytes& body);
  static std::thread thread_manager_player_;
  static std::thread thread_insert_music_;
 public:
  static int run_player();
  static int insert_music(const std::shared_ptr< restbed::Session > session,const restbed::Bytes& body);
};
#endif
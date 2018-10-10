#ifndef MANAGE_MICRO_SERVICE_H
#define MANAGE_MICRO_SERVICE_H
#include <memory>
#include <iostream>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

#define gettid() syscall(SYS_gettid)


/**
 * Class which manage all microService
 */  
class ManagerMicroService
{  
 private:
  static int call_player(std::string path); // fork et exec || thread exec
  static void manage_player(); // boucle sur musics, waitpid() à la fin de la boucle
 public:
  static int run_player(); // il est appelé :) 
};
#endif
#ifndef REST_H
#define REST_H
#include <memory>
#include <cstdlib>
#include <restbed>
#include <csignal>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

/**
 * Class which create all page we need
 */  
class Rest
{  
  private: 
    std::shared_ptr<restbed::Resource> resource;
    std::shared_ptr<restbed::Service> service;
    std::shared_ptr<restbed::Settings> settings;
  public:
    Rest();
    ~Rest(){}
    void initService();
    void initSettings();
    void createRoute();
    void run();
};
#endif
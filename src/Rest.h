#ifndef REST_H
#define REST_H

#include <memory>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <cstdio>
#include "manager/ManagerMusic.h"
#include "manager/ManagerStatistiques.h"
#include "manager/ManagerUser.h"

/**
 * Class which create all page we need
 */  
class Rest
{  
  private: 
    std::shared_ptr<restbed::Service> service;
    std::shared_ptr<restbed::Settings> settings;
    
    void createRouteUsager();
    rapidjson::Document getJsonFile();
    std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>> mapFunction();

  public:
    Rest();
    ~Rest(){}
    void initService();
    void initSettings();
    void createRoute();
    void run();
};
#endif
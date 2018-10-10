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
#include "utils/json.h"

/**
 * Class which create all page we need
 */  
class Rest
{  
  public:
    Rest();
    ~Rest(){}
    void initService();
    void initSettings(std::string address, uint16_t port);
    void createRoute();
    void run();

  private: 
    void createRouteUsager();
    std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>> mapFunction();

    std::shared_ptr<restbed::Service> service_;
    std::shared_ptr<restbed::Settings> settings_;
};
#endif
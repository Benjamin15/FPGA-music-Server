#pragma once

#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include "header/rapidjson.h"
#include "dispatcher/dispatcher.h"

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
  void initSSLSettings(std::string address);
  std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>> mapFunction();

  std::shared_ptr<restbed::Service> service_;
  std::shared_ptr<restbed::Settings> settings_;
  std::shared_ptr<restbed::SSLSettings> ssl_settings_;
};

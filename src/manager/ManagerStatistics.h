#pragma once

#include <memory>
#include <restbed>
#include <iostream>

/**
 * Class which manage all users
 */  

class ManagerStatistics
{  
  private: 
  public:
    static void get_statistics(const std::shared_ptr< restbed::Session > session);
};
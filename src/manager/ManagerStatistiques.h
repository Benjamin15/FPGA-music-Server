#pragma once
#include <memory>
#include <restbed>
#include <iostream>

/**
 * Class which manage all users
 */  

class ManagerStatistiques
{  
  private: 
  public:
    static void get_statistiques(const std::shared_ptr< restbed::Session > session);
};
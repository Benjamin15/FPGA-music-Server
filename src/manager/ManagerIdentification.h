#ifndef MANAGE_IDENTIFICATION_H
#define MANAGE_IDENTIFICATION_H
#include <memory>
#include <restbed>
#include <iostream>

/**
 * Class which manage all users
 */  
class ManagerIdentification
{  
  private:
   /** je genere l'id, je verifie qu'il ne figure pas déjà dans la liste
    * j'inscris l'id avec ip et macaddress et tmp
    * je renvois l'id au client avec la response adéquate
    * 
   */
  public:
    static int registerId();
    static bool checkId(int id);
    static bool checkMacAddress(std::string userMacQddress);
};
#endif
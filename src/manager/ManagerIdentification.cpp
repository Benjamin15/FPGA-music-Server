#include "ManagerIdentification.h"

int registerId(){
  const int minimum = 1, maximum = 999999999999;
  int id = minimum+(rand()% static_cast<int>(maximum-minimum+1));
  std::cout << "id : " << id << std::endl;
  FILE* fp = fopen("metadata/idLogs.json", "rb"); // non-Windows use "r"
  return id;
}

bool checkId(int id);
bool checkMacAddress(std::string userMacAddress);

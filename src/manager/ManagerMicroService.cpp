#include "ManagerMicroService.h"

int ManagerMicroService::call_player(std::string path) {

}
void ManagerMicroService::manage_player() {
  std::cout << "je suis lance" << std::endl;
  int id = gettid();
  std::cout << "id manage player : " << id << std::endl;
}
int ManagerMicroService::run_player() {
  std::cout << " run palyer" << std::endl;
  std::thread thread_manager_player(manage_player);
  std::cout << "thread lance ? " << std::endl;
}
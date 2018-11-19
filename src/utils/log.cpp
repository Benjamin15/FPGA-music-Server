#include "log.h"

void write_log(std::string content)
{
  std::cout << "j'komprin po" << std::endl;
  std::cout << "try write" << std::endl;
  init();
  std::lock_guard<std::mutex> lock(mMutex);
    
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  char timestamp[26];
  ctime_r(&now, timestamp);

  std::string timestampWithoutEndl(timestamp);    
  timestampWithoutEndl = timestampWithoutEndl.substr(0, 24);
  std::cout << content << std::endl;
  mStream << timestampWithoutEndl << ": " << content << std::endl;
  mStream.close();
}

void init() {
  std::cout << "constructor" << std::endl;
  mStream.open(file_path, std::ios::trunc);
  std::cout << "open trunc" << std::endl;
  mStream.close();
  std::cout << "close" << std::endl;
  mStream.open(file_path, std::ios::app);
  std::cout << "open app" << std::endl;
  if (mStream.fail())
  {
    std::cout << "fail"<<std::endl;
    throw std::iostream::failure("Cannot open file: " + file_path);
  }
}
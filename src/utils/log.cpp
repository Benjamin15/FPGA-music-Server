#include "log.h"


  SysLogger::SysLogger(std::string fileName)
  {
    mStream.open(fileName, std::ios::trunc);
    mStream.close();
    mStream.open(fileName, std::ios::app);
    if (mStream.fail())
    {
      std::cout << "fail"<<std::endl;
      throw std::iostream::failure("Cannot open file: " + fileName);

    }
  }  
 
  void SysLogger::WriteLine(std::string content)
  {
    std::lock_guard<std::mutex> lock(mMutex);
     
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestamp[26];
    ctime_r(&now, timestamp);
 
    std::string timestampWithoutEndl(timestamp);    
    timestampWithoutEndl = timestampWithoutEndl.substr(0, 24);
    std::cout << content << std::endl;
    mStream << timestampWithoutEndl << ": " << content << std::endl;
  }

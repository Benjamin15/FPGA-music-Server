#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>
#include <time.h>


  class SysLogger
  {
  public:
  
    SysLogger(std::string fileName);
    SysLogger(const SysLogger&) = delete;    
    SysLogger& operator= (const SysLogger&) = delete;

    SysLogger(SysLogger&& other){
      mStream.close();
      mStream = move(other.mStream);
    }

    SysLogger& operator=(SysLogger&& other){
      mStream.close();
      mStream = move(other.mStream);
      return *this;
    }
 
    ~SysLogger()     
    {
      mStream.close();
    }
 
    void WriteLine(std::string content);
 
  private:
    std::ofstream mStream;
    std::mutex mMutex;
  };


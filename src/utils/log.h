#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>
#include <time.h>


  class MyLogger
  {
  public:
    // parametrized ctor
    MyLogger(std::string fileName);
    MyLogger(const MyLogger&) = delete;    
    MyLogger& operator= (const MyLogger&) = delete;
 
    // dtor
    ~MyLogger()     
    {
      mStream.close();
    }
 
    // write to log file
    void WriteLine(std::string content);
 
  private:
    std::ofstream mStream;
    std::mutex mMutex;
  };

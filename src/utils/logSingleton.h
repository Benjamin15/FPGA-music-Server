#pragma once
#include "log.h"

  class MyLoggerSingleton
  {
  public:
    MyLoggerSingleton(MyLoggerSingleton const&) = delete;             // Copy construct
    MyLoggerSingleton(MyLoggerSingleton&&) = delete;                  // Move construct
    MyLoggerSingleton& operator=(MyLoggerSingleton const&) = delete;  // Copy assign
    MyLoggerSingleton& operator=(MyLoggerSingleton &&) = delete;      // Move assign
 
    static MyLogger& Instance()
    {
      static MyLogger myInstance("metadata/log.txt");
      return myInstance;
    }  
 
  protected:
    MyLoggerSingleton() {}
    ~MyLoggerSingleton() {}
  };

#pragma once
#include "log.h"

  class SysLoggerSingleton
  {
  public:
    SysLoggerSingleton(SysLoggerSingleton const&) = delete;             
    SysLoggerSingleton(SysLoggerSingleton&&) = delete;                  
    SysLoggerSingleton& operator=(SysLoggerSingleton const&) = delete;  
    SysLoggerSingleton& operator=(SysLoggerSingleton &&) = delete;      
 
    static SysLogger& GetInstance()
    {
      static SysLogger myInstance("metadata/log.txt");
      return myInstance;
    }  
 
  protected:
    SysLoggerSingleton() {}
    ~SysLoggerSingleton() {}
  };

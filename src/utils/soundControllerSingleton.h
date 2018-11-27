#pragma once
#include "soundController.h"

  class SoundControllerSingleton
  {
  public:
    SoundControllerSingleton(SoundControllerSingleton const&) = delete;             
    SoundControllerSingleton(SoundControllerSingleton&&) = delete;                  
    SoundControllerSingleton& operator=(SoundControllerSingleton const&) = delete;  
    SoundControllerSingleton& operator=(SoundControllerSingleton &&) = delete;      
 
    static SoundController& GetInstance()
    {
      static SoundController soundControllerInstance;
      return soundControllerInstance;
    }  
  
 
  protected:
    SoundControllerSingleton() {}
    ~SoundControllerSingleton() {}


  };

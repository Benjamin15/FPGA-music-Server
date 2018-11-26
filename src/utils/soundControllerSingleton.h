#pragma once
#include "soundController.h"

  class SoundControllerSingleton
  {
  public:
    SoundControllerSingleton(SoundControllerSingleton const&) = delete;             
    SoundControllerSingleton(SoundControllerSingleton&&) = delete;                  
    SoundControllerSingleton& operator=(SoundControllerSingleton const&) = delete;  
    SoundControllerSingleton& operator=(SoundControllerSingleton &&) = delete;      
 
    static std::shared_ptr<SoundController> GetInstance()
    {
      if(!instance_){
          instance_ = std::make_shared<SoundController>();
      }
      return instance_;
    }  
  
  private:
    static std::shared_ptr<SoundController> instance_;
 
  protected:
    SoundControllerSingleton() {}
    ~SoundControllerSingleton() {}


  };

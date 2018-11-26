#include "soundController.h"

SoundController::SoundController(){
    initSoundController();
}

SoundController::~SoundController(){
    snd_mixer_close(handle_);
}

void SoundController::initSoundController(){
    std::cout << "Initializing sound control..." << std::endl;

    snd_mixer_open(&handle_, 0);
    snd_mixer_attach(handle_, card);
    snd_mixer_selem_register(handle_, NULL, NULL);
    snd_mixer_load(handle_);
    
    snd_mixer_selem_id_alloca(&sid_);
    snd_mixer_selem_id_set_index(sid_, 0);
    snd_mixer_selem_id_set_name(sid_, selement_name);
    element_ = snd_mixer_find_selem(handle_, sid_);

    snd_mixer_selem_get_playback_volume_range(element_, &minVolumeRange_, &maxVolumeRange_);
    volumeRange_ = maxVolumeRange_ - minVolumeRange_;

    std::cout << "min: " << minVolumeRange_ << std::endl;
    std::cout << "max: " << maxVolumeRange_ << std::endl;
    std::cout << "Volume range: " << volumeRange_ << std::endl;

    getPlaybackVolume();
    volumePercentage_ = MAX_VOLUME_PERCENTAGE * (volume_ - minVolumeRange_) / volumeRange_;
    printVolumeStatus();
}

void SoundController::setVolume(int volumePercentage){
    long volume = ((volumePercentage * volumeRange_)/ MAX_VOLUME_PERCENTAGE) + minVolumeRange_;
    volumePercentage_ = volumePercentage;
    std::cout << "Setting volume to: " << volume << std::endl;
    snd_mixer_selem_set_playback_volume_all(element_, volume);
    getPlaybackVolume();
    printVolumeStatus();
}

void SoundController::getPlaybackVolume(){
    snd_mixer_selem_get_playback_volume(element_, SND_MIXER_SCHN_FRONT_LEFT, &volume_);
}
void SoundController::printVolumeStatus(){
    std::cout << "Updated volume: " << volume_ << std::endl;
    std::cout << "Updated volume percentage: " << volumePercentage_ << std::endl;
}

int SoundController::getVolume(){
    std::cout << "Current volume percentage: " << volumePercentage_ << std::endl;
    return volumePercentage_;
}

void SoundController::increaseVolume(int percent){
    if(isMuted()){
        unmute();
    }
    if(volumePercentage_ != MAX_VOLUME_PERCENTAGE){

        std::cout << "Volume increased by: " << percent << " percent(s)" << std::endl;
        int newVolumePercentage = volumePercentage_ + percent;

        if(newVolumePercentage >= MAX_VOLUME_PERCENTAGE){
            setVolume(MAX_VOLUME_PERCENTAGE);
        }
        else{
            setVolume(newVolumePercentage);
        }
    }
    else{
        std::cout << "Volume is maxed out" << std::endl;
    }
}

void SoundController::decreaseVolume(int percent){
    if(volumePercentage_ != MIN_VOLUME_PERCENTAGE){

        std::cout << "Volume decreased by: " << percent << " percents" << std::endl;
        int newVolumePercentage = volumePercentage_ - percent;
        
        if(newVolumePercentage <= MIN_VOLUME_PERCENTAGE){
            setVolume(MIN_VOLUME_PERCENTAGE);
            mute();
        }
        else{
            setVolume(newVolumePercentage);
        }
    }
    else{
        std::cout << "Volume is at the lowest" << std::endl;
    }

    
}

void SoundController::mute(){
    snd_mixer_selem_set_playback_switch_all(element_, 0);
    isMuted_ = true;
    std::cout << "Mute status: " << isMuted_ << std::endl;
    
}

void SoundController::unmute(){
    snd_mixer_selem_set_playback_switch_all(element_, 1);
    setVolume(volumePercentage_);
    isMuted_ = false;
    std::cout << "Mute status: " << isMuted_ << std::endl;
}

bool SoundController::isMuted(){
    return isMuted_;
}

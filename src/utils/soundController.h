#pragma once

#include <alsa/asoundlib.h>
#include <iostream>

class SoundController
{
public:
  SoundController();
  SoundController(const SoundController &) = delete;
  SoundController &operator=(const SoundController &) = delete;
  ~SoundController();

  int getVolume();
  void increaseVolume(int percent);
  void decreaseVolume(int percent);
  void mute();
  void unmute();
  bool isMuted();

private:
  void initSoundController();
  void setVolume(int volumePercentage);
  void printVolumeStatus();
  void getPlaybackVolume();

  bool isMuted_;
  snd_mixer_t *handle_;
  snd_mixer_elem_t *element_;
  snd_mixer_selem_id_t *sid_;
  long maxVolumeRange_;
  long minVolumeRange_;
  long volumeRange_;
  long volume_;
  int volumePercentage_;

  const char *card = "default";
  const char *selement_name = "Headphone";
  const int MAX_VOLUME_PERCENTAGE = 100;
  const int MIN_VOLUME_PERCENTAGE = 0;
};

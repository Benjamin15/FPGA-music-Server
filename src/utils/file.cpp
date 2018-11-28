#include "file.h"

/**
 * This function remove a mp3 file
 * @param no : the music's no
 */ 
void removeMP3Selected(const std::string no) {
  std::string path= ("metadata/musique/" + no + ".mp3");
  if (remove(path.c_str()) == 0)
    std::cout<<"Le fichier "<< no << ".mp3 "<< "est supprimé."<<std::endl;
  else
    std::cout<<"Fichier introuvable"<<std::endl;
}


/**
 * decode info of music
 * @param path : link to the file
 * @return write info in model music
 */ 
Music get_info(std::string path) {
  TagLib::FileRef f(path.c_str());
  std::string title;
  TagLib::Tag *tag;
  unsigned int seconds;
  unsigned int minutes;
  if (!f.isNull() && f.tag() && f.audioProperties()) {
    TagLib::AudioProperties *properties = f.audioProperties();
    seconds = properties->length() % 60;
    minutes = (properties->length() - seconds) / 60;
    tag = f.tag();
  } else 
    throw UnsupportedException();
  return Music( tag->title().to8Bit() == "" ? "Unknown" : tag->title().to8Bit(),
                tag->artist().to8Bit() == "" ? "Unknown" : tag->artist().to8Bit(),
                std::to_string(minutes) + ":" +std::to_string(seconds));
}


/**
 * get nbsample in the music
 * @param path : link to the file
 * @return nsamples
 */ 
Music get_info_for_player(std::string path) {
  TagLib::FileRef f(path.c_str());
  std::string title;
  int channels = 2;
  int bitrate = 80000;
  int sample_rate = 44100;
  if (!f.isNull() && f.tag() && f.audioProperties()) {
    TagLib::AudioProperties *properties = f.audioProperties();
    channels = properties->channels();
    bitrate = properties->bitrate();
    sample_rate = properties->sampleRate();
  }
  return Music(channels, bitrate, sample_rate);
}

/**
 * This method check if the file is a mp3
 * @param path : path of the file
 * @return success
 */ 
bool checkIfMp3(std::string path) {
  TagLib::FileRef f(path.c_str());
  std::string title, artist, year, duration;
  if (!f.isNull() && f.tag() && f.audioProperties()) {    
    return true;
  }else{
    return false;
  }
}
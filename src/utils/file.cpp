#include "file.h"
void removeMP3Selected(const std::string no) {
  std::string path= ("metadata/musique/" + no + ".mp3");
  if(remove(path.c_str()) == 0)
    std::cout<<"Le fichier "<< no << ".mp3 "<< "est supprimé."<<std::endl;
  else
    std::cout<<"Fichier introuvable"<<std::endl;
}

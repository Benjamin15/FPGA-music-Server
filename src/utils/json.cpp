#include "json.h"

/**
 * Get the JSON file which contain each routes
 * 
 */

rapidjson::Document getJsonFile(const char* path) {
  FILE* fp = fopen(path, "rb"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);
  return d;
}

void writeJsonFile(const char* path, const rapidjson::Document& d) {
  FILE* fp = fopen(path, "wb");
  char buffer_writer[65536];
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  d.Accept(writer);
  fclose(fp);
}

std::string getListForUser(std::vector<Music> musics) {
  std::stringstream result;
  const char* separator = ", \n";
  result << "{ \n  \"chansons\": [\n";
  for (unsigned int i = 0 ; i < musics.size() ; i++) {
    Music music = musics[i];
    result << music.toStringForUser() << separator ;
    if (i == musics.size() - 2)
      separator = "\n";
  };
  result << "]\n}";
  return result.str(); 
}

std::string getListForAdmin(std::vector<Music> musics) {
  std::stringstream result;
  const char* separator = ", \n";
  result << "{ \n  \"chansons\": [\n";
  for (unsigned int i = 0 ; i < musics.size() ; i++) {
    Music music = musics[i];
    result << music.toString() << separator ;
    if (i == musics.size() - 2)
      separator = "\n";
  };
  result << "]\n}";
  return result.str(); 
}

void remove_last_music() {
  FILE* fp = fopen("metadata/musiques.json", "rb");
  char buffer_reader[65536];
  char buffer_writer[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document doc;
  doc.ParseStream(is);
  fclose(fp);
  fp = fopen("metadata/musiques.json", "wb");
  rapidjson::Value& items = doc["musiques"];
  items.Erase(items.Begin());
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  doc.Accept(writer);
  fclose(fp);
}

std::string removeMusicSelected(const unsigned int idMusic, const unsigned int noMusic) {
  int pos=0;
  bool musiqueTrouvee=false;
  std::string titreMusique="";

  FILE* fp = fopen("metadata/musiques.json", "rb");
  char readBuffer[65536];
  char buffer_writer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);

  rapidjson::Value& musiques = d["musiques"]; 

  for (rapidjson::SizeType i = 0; i < musiques.Size(); i++) {
    if(musiques[i]["id"].GetUint()==idMusic && musiques[i]["no"].GetUint()==noMusic) {
      std::cout<<"ID: " << musiques[i]["id"].GetUint() << " et numéro: " << musiques[i]["no"].GetUint() << " trouvés." <<std::endl;
      titreMusique=musiques[i]["titre"].GetString();
      pos=(int)i;
      musiqueTrouvee=true;
    }
  }

  if(musiqueTrouvee) {
    fp = fopen("metadata/musiques.json", "wb");
    musiques.Erase(musiques.Begin()+ pos--);
    rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    d.Accept(writer);
    fclose(fp);
    std::cout<<"La musique dont l'ID est: " << idMusic << " et le numéro: " << noMusic << " est supprimée."<<std::endl;
  }

  return titreMusique;
}

void removeMP3Selected(const std::string titre) {
  std::string path= ("metadata/musique/" + titre + ".mp3");
  if(remove(path.c_str()) == 0)
    std::cout<<"Le fichier "<< titre << ".mp3 "<< "est supprimé."<<std::endl;
  else
    std::cout<<"Fichier introuvable"<<std::endl;
}



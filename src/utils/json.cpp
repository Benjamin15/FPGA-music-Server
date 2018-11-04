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

std::string registerIds(std::string parameter){
  FILE* fp = fopen("metadata/idLogs.json", "rb");
  char buffer_reader[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document readDoc;
  readDoc.ParseStream(is);
  fclose(fp);
  rapidjson::Document writeDoc;
  writeDoc.Parse<0>(parameter.c_str()).HasParseError();
  rapidjson::Value& value = readDoc["UsersLogs"];
  rapidjson::Value& mac = writeDoc["MAC"];
  bool macNotRegistered = true;
  std::string token = "";
  for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
    std::string temp = value[i]["MAC"].GetString();
    if(!temp.compare(mac.GetString())) {
      macNotRegistered = false;
      token = std::to_string(value[i]["Token"].GetUint());
    }
  }
  if(macNotRegistered){
    value.PushBack(writeDoc.GetObject(), readDoc.GetAllocator());
    fp = fopen("metadata/idLogs.json","w+");
    char buffer_writer[65536];
    rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    readDoc.Accept(writer);
    fclose(fp);
    return token;
  }
  return token;
}

std::string createIdentificationResponseJson(std::string token, std::string message){
  std::stringstream result;
  result << "{ "
    << "\"identificateur\": \"" << token << "\", "
    << "\"message\": \"" << message << " }";
  return result.str();
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

void registerMusic(Music music){
  std::string musicJson = music.toRegisterString();
  FILE* fp = fopen("metadata/musiques.json", "rb");
  char buffer_reader[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document readDoc;
  readDoc.ParseStream(is);
  fclose(fp);
  rapidjson::Document writeDoc;
  writeDoc.Parse<0>(musicJson.c_str()).HasParseError();
  rapidjson::Value& value = readDoc["musiques"];
  std::cout<<musicJson<<std::endl;
  value.PushBack(writeDoc.GetObject(), readDoc.GetAllocator());
  fp = fopen("metadata/musiques.json","w+");
  char buffer_writer[65536];
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  readDoc.Accept(writer);
  fclose(fp);
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


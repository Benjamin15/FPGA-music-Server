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

std::string registerIds(std::string parameter){
  FILE* fp = fopen("metadata/idLogs.json", "rb");
  char buffer_reader[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document readDoc;
  readDoc.ParseStream(is);
  fclose(fp);
  rapidjson::Document writeDoc;
  if(writeDoc.Parse<0>(parameter.c_str()).HasParseError()){
    return "Erreur";
  }
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
  }
  return token;
}

std::string createIdentificationResponseJson(std::string token, std::string message){
  std::stringstream result;
  result << "{ "
    << "\"identificateur\": " << token << ", "
    << "\"message\": \"" << message << "\" }";
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
  std::cout << "item remove" << std::endl;
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  doc.Accept(writer);
  fclose(fp);
}

void removeMusicSelected(const unsigned int idMusic, const unsigned int noMusic) {
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
    if(musiques[i]["no"].GetUint()==noMusic) {
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
}

void removeMP3Selected(const std::string no) {
  std::string path= ("metadata/musique/" + no + ".mp3");
  if(remove(path.c_str()) == 0)
    std::cout<<"Le fichier "<< no << ".mp3 "<< "est supprimé."<<std::endl;
  else
    std::cout<<"Fichier introuvable"<<std::endl;
}


/**
 * This method write every musics in the vector to the musiques.json
 * @param musics
 */ 
void write_music(const std::vector<Music> musics) {
  std::string json = getListForAdmin(musics);
  std::cout << "json : " << json.c_str() << std::endl;
  rapidjson::Document document;
  document.Parse(json.c_str());
  remove("metadata/musiques.json");
  FILE* fp = fopen("metadata/musiques.json", "wb"); // non-Windows use "w"
  char writeBuffer[65536];
  rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  document.Accept(writer);
  fclose(fp);
}

bool isValidToken(int token){
  rapidjson::Document document = getJsonFile("metadata/idLogs.json");
  rapidjson::Value& usersLogs = document["UsersLogs"];

  std::chrono::milliseconds timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) ;
  int time = timestamp.count();
  std::cout << "Current time: " << time << std::endl;  
  for (rapidjson::SizeType i = 0; i < usersLogs.Size(); i++) { 
    if(usersLogs[i]["Token"].GetUint()==token) {
      std::cout<<"Token: " << usersLogs[i]["Token"].GetUint() <<  " trouvé." <<std::endl;
      std::cout<<"Time: " << usersLogs[i]["Time"].GetUint() <<  " trouvé." <<std::endl;
      if(time - usersLogs[i]["Time"].GetUint() >= 24*60*60*1000){
        std::cout << "Token invalide" << std::endl; 
        return false;
      }
      std::cout << "Token toujours valide" << std::endl;
      return true;
    }
  }
  std::cout << "user not found" << std::endl;
  return false;

}

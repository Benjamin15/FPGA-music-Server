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

std::string find_token(rapidjson::Value& users, rapidjson::Value& mac) {
  std::string token = "0";
  for (rapidjson::SizeType i = 0; i < users.Size(); i++) {
    std::string temp = users[i]["MAC"].GetString();
    if(!temp.compare(mac.GetString())) {
      token = std::to_string(users[i]["Token"].GetUint());
    }
  }
  return token;
}

std::string registerIds(std::string parameter){
  FILE* fp = fopen("metadata/idLogs.json", "rb");
  char buffer_reader[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document readDoc;
  readDoc.ParseStream(is);
  fclose(fp);
  rapidjson::Document writeDoc;
  if(writeDoc.Parse<0>(parameter.c_str()).HasParseError())
    throw BadRequestException();
  rapidjson::Value& users = readDoc["UsersLogs"];
  rapidjson::Value& mac = writeDoc["MAC"];
  std::string token = find_token(users, mac);
  if (token == "0"){
    users.PushBack(writeDoc.GetObject(), readDoc.GetAllocator());
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
  }
}

/**
 * This method write every musics in the vector to the musiques.json
 * @param musics
 */ 
void write_music(std::vector<Music> musics) {
  std::string json = getListForAdmin(musics);
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
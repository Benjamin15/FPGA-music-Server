#include "json.h"

/**
 * Get the JSON file which contain each routes
 * @param path
 * @return document
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

/**
 * write a document in a file
 * @param path
 * @param document
 */ 
void writeJsonFile(const char* path, const rapidjson::Document& document) {
  FILE* fp = fopen(path, "wb");
  char buffer_writer[65536];
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  document.Accept(writer);
  fclose(fp);
}

/**
 * create the json to send when we try to connect the app
 * @param token
 * @param message
 * @return result
 */ 
std::string createIdentificationResponseJson(std::string token, std::string message){
  std::stringstream result;
  result << "{ "
    << "\"identificateur\": " << token << ", "
    << "\"message\": \"" << message << "\" }";
  std::cout << result.str() << std::endl;
  return result.str();
}

/**
 * get the list of the music for the user
 * @param list of musics
 * @return list_json
 */ 
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

/**
 * register a music in the metadata file 
 * @param music
 */ 
void registerMusic(Music music){
  std::string musicJson = music.toRegisterString();
  FILE* fp = fopen(music_json_path.c_str(), "rb");
  char buffer_reader[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document readDoc;
  readDoc.ParseStream(is);
  fclose(fp);
  rapidjson::Document writeDoc;
  writeDoc.Parse<0>(musicJson.c_str()).HasParseError();
  rapidjson::Value& value = readDoc[musics_log.c_str()];
  value.PushBack(writeDoc.GetObject(), readDoc.GetAllocator());
  fp = fopen(music_json_path.c_str(),"w+");
  char buffer_writer[65536];
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  readDoc.Accept(writer);
  fclose(fp);
}

/**
 * get the list of the music for the admin
 * @param list of musics
 * @return list_json
 */ 
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


/**
 * write the stats json 
 * 
 */ 
std::string getStats(int n_music, int n_user, int n_music_remove, std::string avg_duration) {
  std::stringstream result;
  result << "{ \"chansons\": " << n_music << "\n";
  result << "\"utilisateurs\": " << n_user << "\n";
  result << "\"elemines\": " << n_music_remove << "\n";
  result << "\"temps\": " << avg_duration << "\n";
  result << "}";
  return result.str(); 
}

 * get the list of the music for the metadate file
 * @param list of musics
 * @return list_json
 */ 
std::string getListForUsersMetadata(std::vector<Music> musics) {
  std::stringstream result;
  const char* separator = ", \n";
  result << "{ \n  \"musics\": [\n";
  for (auto it_music = musics.begin() ; it_music != musics.end() ; it_music++) {
    if (it_music + 1 == musics.end())
      separator = "\n";
    result << get_json_string(it_music->to_json()) << separator;
  }
  result << "]\n}";
  return result.str(); 
}

/**
 * get the list of users
 * @param list of musics
 * @return list_json
 */ 
std::string getListUsers(std::vector<User> users) {
  std::stringstream result;
  const char* separator = ", \n";
  result << "{ \n  \"users\": [\n";
  for(auto it_user = users.begin();it_user != users.end();it_user++){
    if (it_user + 1 == users.end())
      separator = "\n";
    result << it_user->to_string() << separator;
  }
  result << "]\n}";
}
/**
 * remove the last music in the metadata json file
 * 
 */ 
void remove_last_music() {
  FILE* fp = fopen(music_json_path.c_str(), "rb");
  char buffer_reader[65536];
  char buffer_writer[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document doc;
  doc.ParseStream(is);
  fclose(fp);
  fp = fopen(music_json_path.c_str(), "wb");
  rapidjson::Value& items = doc[musics_log.c_str()];
  items.Erase(items.Begin());
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  doc.Accept(writer);
  fclose(fp);
}


/**
 * remove a selected music in the metadata json file
 * @param idUser
 * @param noMusic
 * 
 */ 
void removeMusicSelected(const unsigned int noMusic) {
  int pos=0;
  bool musiqueTrouvee=false;

  FILE* fp = fopen(music_json_path.c_str(), "rb");
  char readBuffer[65536];
  char buffer_writer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);

  rapidjson::Value& musiques = d[musics_log.c_str()]; 

  for (rapidjson::SizeType i = 0; i < musiques.Size(); i++) {
    if(musiques[i][no_log.c_str()].GetUint()==noMusic) {
      pos=(int)i;
      musiqueTrouvee=true;
    }
  }

  if(musiqueTrouvee) {
    fp = fopen(music_json_path.c_str(), "wb");
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
  std::string json = getListForUsersMetadata(musics);
  rapidjson::Document document;
  document.Parse(json.c_str());
  remove(music_json_path.c_str());
  FILE* fp = fopen(music_json_path.c_str(), "wb"); // non-Windows use "w"
  char writeBuffer[65536];
  rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  document.Accept(writer);
  fclose(fp);
}

const std::string get_json_string(rapidjson::Document document)
{
  rapidjson::StringBuffer buffer;

  buffer.Clear();

  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  document.Accept(writer);

  return buffer.GetString();
}

/**
 * get the list of the user for the metadate file
 * @param list of users
 * @return list_json
 */ 
std::string getListUsersMetadata(std::vector<User> users) {
  std::stringstream result;
  const char* separator = ", \n";
  result << "{ \n  \"users\": [\n";
  for (auto it_user = users.begin() ; it_user != users.end() ; it_user++) {
    if (it_user + 1 == users.end())
      separator = "\n"; 
    result << get_json_string(it_user->to_json()) << separator;
  }
  result << "]\n}";
  return result.str(); 
}


/**
 * This method write every users in the vector to the users.json
 * @param users
 */ 
void write_users(std::vector<User> users) {
  std::string json = getListUsersMetadata(users);
  rapidjson::Document document;
  document.Parse(json.c_str());
  remove(user_log_path.c_str());
  FILE* fp = fopen(user_log_path.c_str(), "wb"); // non-Windows use "w"
  char writeBuffer[65536];
  rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  document.Accept(writer);
  fclose(fp);
}
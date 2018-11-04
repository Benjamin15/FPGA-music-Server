#include "ManagerMusic.h"
std::vector<Music> ManagerMusic::musics;

void ManagerMusic::get_usager_files(const std::shared_ptr< restbed::Session > session) {
  int id = gettid();
  std::cout << "id : " << id << std::endl;
  std::cout << "liste musique de l'usager" << std::endl;
  std::string result = getListForUser(musics);
  std::cout << result << std::endl;
  session->close( restbed::OK, result, { { "Content-Length", std::to_string(result.size()) }, { "Connection", "close" } } );
}

void ManagerMusic::insert_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "Insert musique" << std::endl;
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
     const std::string id = session->get_request()->get_path_parameter( "id" );
     const std::string musicTitle = session->get_request()->get_query_parameter( "title","Error getting Query parameter" );
     std::cout<<id<<std::endl;
     std::cout<<musicTitle<<std::endl;
     std::stringstream ss;
     for(auto byte: body){
       ss<< byte;
     }
     std::string mp3EncodedMusic = ss.str();
     std::string mp3DecodedMusic = base64_decode(mp3EncodedMusic);
     base64_toBinary(mp3DecodedMusic,musicTitle);
     std::string path = "metadata/musique/" + musicTitle ;
     Music music = ManagerMusic::get_info(path);
     User user = ManagerMusic::get_user_for_sent_music(std::stoi(id));
     music.setMusicUser(user);
     registerMusic(music);
     session->close(restbed::OK,mp3DecodedMusic,{{"Content-Length",std::to_string(mp3DecodedMusic.size())},
     {"Connection","close"}});
  });
}

void ManagerMusic::delete_usager__song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer musique utilisateur" << std::endl;
}

void ManagerMusic::get_superviser_files(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir musique superviseur" << std::endl;
  std::string result = getListForAdmin(musics);
  std::cout << result << std::endl;
  session->close( restbed::OK, result, { { "Content-Length", std::to_string(result.size()) }, { "Connection", "close" } } );
}

void ManagerMusic::delete_superviser_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer song avec le superviseur" << std::endl;
}

void ManagerMusic::reverse_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "inverser musique" << std::endl;
}

void ManagerMusic::get_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir le volume" << std::endl;
}

void ManagerMusic::set_up_volume(const std::shared_ptr< restbed::Session > session) {
    std::cout << "augmenter le volume" << std::endl;
}

void ManagerMusic::set_down_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "diminuer le volume" << std::endl;
}

void ManagerMusic::enabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "activer mute" << std::endl;
}

void ManagerMusic::disabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "désactiver mute" << std::endl;
}

User ManagerMusic::get_user_for_sent_music(int userId){
  rapidjson::Document document = getJsonFile("metadata/idLogs.json");
  rapidjson::Value& value = document["UsersLogs"];
  std::string userMac, userIp, userName;
  for (rapidjson::SizeType i = 0; i < value.Size(); i++) {
    int temp = value[i]["Token"].GetUint();
    if(temp == userId) {
      userMac = value[i]["MAC"].GetString();
      userIp = value[i]["ip"].GetString();
      userName = value[i]["nom"].GetString();
    }
  }
  User user(userId, userName, userIp, userMac);
  return user;
}
void ManagerMusic::create_list_music() {
  int id = gettid();
  std::cout << "id : " << id << std::endl;
  FILE* fp = fopen("metadata/musiques.json", "rb"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);
  const rapidjson::Value& musiques = d["musiques"];
 for (rapidjson::SizeType i = 0; i < musiques.Size(); i++) {
    std::string mac = musiques[i]["MAC"].GetString();
    int idUser = musiques[i]["id"].GetUint();
    std::string suggestBy = musiques[i]["proposeePar"].GetString();
    std::string ip = musiques[i]["ip"].GetString();
    int idMusic = musiques[i]["no"].GetUint();
    std::string duration = musiques[i]["duree"].GetString();
    std::string artist = musiques[i]["artiste"].GetString();
    std::string title = musiques[i]["titre"].GetString();
    User user(idUser, suggestBy, ip, mac);
    Music music(idMusic, title, artist, duration, user);
    musics.push_back(music);
    }
  std::cout << "Musiques bien ajouté" << std::endl;
}

void ManagerMusic::launch_music() {
  ManagerMicroService::run_player();
}

/**
 * decode info of music
 * @param path : link to the file
 * @return write info in model music
 */ 
Music ManagerMusic::get_info(std::string path) {
  TagLib::FileRef f(path.c_str());
  std::string title, artist, year, duration;
  if(!f.isNull() && f.tag() && f.audioProperties()) {
    TagLib::AudioProperties *properties = f.audioProperties();
    int seconds = properties->length() % 60;
    int minutes = (properties->length() - seconds) / 60;
    TagLib::Tag *tag = f.tag();
    return Music(tag->title().to8Bit(), tag->artist().to8Bit(), std::to_string(minutes) + ":" +std::to_string(seconds));
  }
}

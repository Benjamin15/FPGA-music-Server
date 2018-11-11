#include "ManagerMusic.h"
std::vector<Music> ManagerMusic::musics;

void ManagerMusic::get_usager_files(const std::shared_ptr< restbed::Session > session) {
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
     if (checkListSize()){
      std::string mp3EncodedMusic = ss.str();
      std::string mp3DecodedMusic = base64_decode(mp3EncodedMusic);
      std::string fileName = std::to_string(Music::getNextMusicId("metadata/musiques.json"))+".mp3";
      base64_toBinary(mp3DecodedMusic,fileName);
      std::string path = "metadata/musique/" + fileName;
      if(!ManagerMusic::checkIfMp3(path)){
        session->close(415,"Le fichier soumis n'est pas un MP3 ou n'a pas d'entête ID3",{{"Content-Length","20"},
      {"Connection","close"}});
      }
      Music music = ManagerMusic::get_info(path);
      User user = ManagerMusic::get_user_for_sent_music(std::stoi(id));
      music.setMusicUser(user);
      if(music.title_ == ""){
        music.setMusicTitle(musicTitle);
      }
      music.setMusicNumber("metadata/musiques.json");
      registerMusic(music);
      musics.push_back(music);
      SysLoggerSingleton::GetInstance().WriteLine("Soumission d'une nouvelle chanson: " + musicTitle);
      session->close(restbed::OK,mp3DecodedMusic,{{"Content-Length",std::to_string(mp3DecodedMusic.size())},
      {"Connection","close"}});
    }else{
      session->close(413,"La liste est pleine",{{"Content-Length","20"},
      {"Connection","close"}});
    }
  });
}

bool ManagerMusic::checkListSize(){
  bool canAddMusic = true;
  rapidjson::Document document = getJsonFile("metadata/musiques.json");
  const rapidjson::Value& musiques = document["musiques"];
  int size = 0;
  for (rapidjson::SizeType i = 0; i < musiques.Size(); i++) {
    size = i;
  }
  if(size == 9){
    canAddMusic = false;
  }
  return canAddMusic;
}

void ManagerMusic::delete_usager__song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer musique utilisateur" << std::endl;
  const unsigned int idMusic=atoi((session->get_request()->get_path_parameter("id")).c_str());
  const unsigned int noMusic=atoi((session->get_request()->get_path_parameter("no")).c_str());
  std::cout<<idMusic<<std::endl;
  std::cout<<noMusic<<std::endl;
  std::string musicToRemove=removeMusicSelected(idMusic, noMusic);
  removeMP3Selected(musicToRemove);
  std::string responseBody = "ok";
  session->close( restbed::OK, responseBody, { { "Content-Length", std::to_string(responseBody.size()) }, { "Connection", "close" } } );
  SysLoggerSingleton::GetInstance().WriteLine("Retrait de la chanson: " + musicToRemove);
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
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch( content_length, [ request ]( const std::shared_ptr< restbed::Session > session, const restbed::Bytes & body )
  {
    std::cout << "test" << std::endl;
    rapidjson::Document document;
    document.SetObject();
    std::cout << "after set object" << std::endl;
    std::string bodyString = std::string(body.begin(), body.end());
    std::cout << " body String : " << bodyString << std::endl;
    std::cout << "before parse" << std::endl;
    document.Parse<0>(bodyString.c_str(), bodyString.length());
    std::cout << "before une" << std::endl;
    int first = document["une"].GetInt();
    std::cout << "before autre" << std::endl;
    int second = document["autre"].GetInt();
    std::cout << "first : " << first << std::endl;
    std::cout << "second : " << second << std::endl;
    for (Music music : musics)
      std::cout << music.toString() << std::endl;    
    std::iter_swap(find(musics.begin(), musics.end(), first), find(musics.begin(), musics.end(), second));
    std::cout << "swap" << std::endl;
    for (Music music : musics)
      std::cout << music.toString() << std::endl;  
    write_music(musics);
    SysLoggerSingleton::GetInstance().WriteLine("Modification de l'ordre de passage des chansons");
    session->close( restbed::OK, "", { { "Content-Length", "0" }, { "Connection", "close" } } );
  });
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

bool ManagerMusic::checkIfMp3(std::string path){
  TagLib::FileRef f(path.c_str());
  std::string title, artist, year, duration;
  if(!f.isNull() && f.tag() && f.audioProperties()) {    
    return true;
  }else{
    return false;
  }
}

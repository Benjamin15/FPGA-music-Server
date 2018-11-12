#include "ManagerMusic.h"
std::vector<Music> ManagerMusic::musics;

void ManagerMusic::get_usager_files(const std::shared_ptr< restbed::Session > session) {
  const std::string id = session->get_request()->get_path_parameter( "id" );
  std::cout<<"id tentative get file :"<<id<<std::endl;
  bool canProceed = ManagerMusic::checkUserToken(std::stoi(id));
  std::cout<<"canproceed:"<<canProceed<<std::endl;
  if(!canProceed){
    ResponseGenerator::sendResponse(session,ResponseGenerator::createForbiddenResponse());
  }else if (canProceed){
    updateMusicsOwner(std::stoi(id));
    std::string result = getListForUser(musics);
    std::cout << result << std::endl;
    ResponseGenerator::sendResponse(session,ResponseGenerator::createOkResponse(result));
  }else{
    ResponseGenerator::sendResponse(session,ResponseGenerator::createInternalServerErrorResponse());
  }
}

void ManagerMusic::insert_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "Insert musique" << std::endl;
    std::cout << &musics << std::endl;
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    ManagerMicroService::insert_music(session,body);
  });
}

bool ManagerMusic::checkListSize(){
  return musics.size() < MAX_LIST_SIZE;
}

bool ManagerMusic::checkUserMusics(int userId){
  int userMusics = 0;
  for(int i=0;i< musics.size();i++){
    if(musics[i].user_.id_== userId){
      userMusics ++;
    }
  }
  return userMusics <5;
}

bool ManagerMusic::supressionPermission(int noMusic,int userId){
  for(int i=0;i< musics.size();i++){
    if(musics[i].id_== noMusic){
      if(musics[i].user_.id_ == userId){
        return true;
      }
      return false;
    }
  }
}

bool ManagerMusic::checkUserToken(int token){
   rapidjson::Document idLogs = getJsonFile("metadata/idLogs.json");
   rapidjson::Value& value = idLogs["UsersLogs"];
   for(int i=0; i < value.GetArray().Size(); i++){
      if(value[i]["Token"].GetUint()== token){
        return true;
      }
   }
  return false;
}

void ManagerMusic::delete_usager__song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer musique utilisateur" << std::endl;
  const unsigned int idUser=atoi((session->get_request()->get_path_parameter("id")).c_str());
  const unsigned int noMusic=atoi((session->get_request()->get_path_parameter("no")).c_str());
  std::cout<<idUser<<std::endl;
  std::cout<<noMusic<<std::endl;
  if(!ManagerMusic::checkUserToken(idUser)){
    ResponseGenerator::sendResponse(session,ResponseGenerator::createForbiddenResponse());
  }else if(!ManagerMusic::supressionPermission(noMusic,idUser)){
    ResponseGenerator::sendResponse(session,ResponseGenerator::createMethodNotAllowedResponse());
  }else{
    std::cout<<"permission : "<<ManagerMusic::supressionPermission(noMusic,idUser)<<std::endl;
    removeMusicSelected(idUser, noMusic);
    removeMP3Selected(std::to_string(noMusic));
    int i = 0;
    std::cout << "no : " << noMusic << std::endl;
    while (i < musics.size()) {
      std::cout << "no Music : " << musics[i].id_ << std::endl; 
      if (musics[i].id_ == noMusic) {
        std::cout << "musics find" << std::endl;
        musics.erase(musics.begin() + i);
      }
      i++;
    }
    ResponseGenerator::sendResponse(session,ResponseGenerator::createOkResponse());
    SysLoggerSingleton::GetInstance().WriteLine("Retrait de la chanson: " + noMusic);
  }
}

void ManagerMusic::get_superviser_files(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir musique superviseur" << std::endl;
  std::string result = getListForAdmin(musics);
  std::cout << result << std::endl;
  session->close( restbed::OK, result, { { "Content-Length", std::to_string(result.size()) }, { "Connection", "close" } } );
}

void ManagerMusic::updateMusicsOwner(int userId){
  for(int i = 0; i< musics.size();i++ ){
    if(musics[i].user_.id_ == userId ){
      musics[i].owner_ = true;
      std::cout<<"chanson du proprietaire trouvee :"<<musics[i].owner_<<std::endl;
    }
  }
}

void ManagerMusic::delete_superviser_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer song avec le superviseur" << std::endl;
  delete_usager__song(session);
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

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
}

void ManagerMusic::delete_usager__song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer musique utilisateur" << std::endl;
  const unsigned int idMusic=atoi((session->get_request()->get_path_parameter("id")).c_str());
  const unsigned int noMusic=atoi((session->get_request()->get_path_parameter("no")).c_str());
  std::cout<<idMusic<<std::endl;
  std::cout<<noMusic<<std::endl;
  std::string musicToRemove=removeMusicSelected(idMusic, noMusic);
  removeMP3Selected(musicToRemove);
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
    auto resource = std::make_shared<restbed::Resource>();
    rapidjson::Value::ConstMemberIterator itr = musiques[i].MemberBegin();
    const unsigned int idMusic = itr++->value.GetUint();
    const std::string title = itr++->value.GetString();
    const std::string artist = itr++->value.GetString();
    const std::string duration = itr++->value.GetString();
    const std::string suggestBy = itr++->value.GetString();
    const std::string ip = itr++->value.GetString();
    const std::string mac = itr++->value.GetString();
    const unsigned int idUser = itr->value.GetUint();
    User user(idUser, suggestBy, ip, mac);
    Music music(idMusic, title, artist, duration, user);
    musics.push_back(music);
  }
  std::cout << "Musique bien ajouté" << std::endl;
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

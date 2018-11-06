#include "ManagerMusic.h"
std::vector<Music> ManagerMusic::musics;

void ManagerMusic::get_usager_files(const std::shared_ptr< restbed::Session > session) {
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
  const auto& request = session->get_request( );
  size_t content_length = 0;
  request->get_header( "Content-Length", content_length );
  session->fetch( content_length, [ request ]( const std::shared_ptr< restbed::Session > session, const restbed::Bytes & body )
  {
    rapidjson::Document document;
    document.SetObject();
    std::string bodyString = std::string(body.begin(), body.end());
    document.Parse<0>(bodyString.c_str(), bodyString.length());
    int first = document["une"].GetInt();
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

void ManagerMusic::create_list_music() {
  int id = gettid();
  std::cout << "id : " << id << std::endl;
    FILE* fp = fopen("metadata/musiques.json", "rb"); // non-Windows use "r"
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    std::cout << "before parse" << std::endl;
    d.ParseStream(is);
    std::cout << "after parse" << std::endl;
    fclose(fp);
  const rapidjson::Value& musiques = d["chansons"];
  for (rapidjson::SizeType i = 0; i < musiques.Size(); i++) {
    auto resource = std::make_shared<restbed::Resource>();
    rapidjson::Value::ConstMemberIterator itr = musiques[i].MemberBegin();
    std::cout << "debut " << std::endl;
    const std::string title = itr++->value.GetString();
        std::cout << 1 << std::endl;
    const std::string artist = itr++->value.GetString();
        std::cout << 2 << std::endl;
    const std::string duration = itr++->value.GetString();
        std::cout << 3 << std::endl;
    const std::string ip = itr++->value.GetString();
      std::cout << 4 << std::endl;
    const std::string mac = itr++->value.GetString();
        std::cout << 5 << std::endl;
    const unsigned int idUser = itr++->value.GetUint();
        std::cout << 6 << std::endl;
    const std::string suggestBy = itr++->value.GetString();
        std::cout << 7 << std::endl;
    const unsigned int idMusic = itr->value.GetUint();
        std::cout << 8 << std::endl;
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

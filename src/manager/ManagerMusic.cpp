#include "ManagerMusic.h"
std::vector<Music> ManagerMusic::musics;

void ManagerMusic::get_usager_files(const std::shared_ptr< restbed::Session > session) {
  std::cout << "liste musique de l'usager" << std::endl;
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
  std::string val = result.str(); 
  std::cout << val << std::endl;
  session->close( restbed::OK, val, { { "Content-Length", std::to_string(val.size()) }, { "Connection", "close" } } );

}

void ManagerMusic::insert_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "Insert musique" << std::endl;
}

void ManagerMusic::delete_usager__song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer musique utilisateur" << std::endl;
}

void ManagerMusic::get_superviser_files(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir musique superviseur" << std::endl;

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
    const unsigned int id = itr++->value.GetUint();
    const std::string title = itr++->value.GetString();
    const std::string artist = itr++->value.GetString();
    const std::string duration = itr++->value.GetString();
    const std::string suggestBy = itr->value.GetString();
    Music music(id, title, artist, duration, suggestBy);
    musics.push_back(music);
  }
  std::cout << "Musique bien ajouté" << std::endl;
}
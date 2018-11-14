#include "DispatcherMusic.h"

void get_usager_files(const std::shared_ptr< restbed::Session > session) {
  const int token = atoi(session->get_request()->get_path_parameter( "id" ).c_str());
  if(!identify(token))
    sendResponse(session,createForbiddenResponse());
  else {
    updateMusicsOwner(token);
    std::string result = getListForUser(musics);
    std::cout << result << std::endl;
    sendResponse(session,createOkResponse(result));
  }
}

void insert_song(const std::shared_ptr< restbed::Session > session) {
  const auto request = session->get_request();
  const int token = atoi(request->get_path_parameter( "id" ).c_str());
  if (!identify(token))
    sendResponse(session,createForbiddenResponse());
  else {
    std::cout << "debut de l'insertion de la musique" << std::endl;
    size_t content_length = std::stoi(request->get_header("Content-Length"));
    session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
    const restbed::Bytes& body){
      ManagerMicroService::insert_music(session,body);
    });
  }
}

void delete_usager__song(const std::shared_ptr< restbed::Session > session) {
  const unsigned int token = atoi(session->get_request()->get_path_parameter( "id" ).c_str());
  if (!identify(token))
    sendResponse(session,createForbiddenResponse());
  else {
    std::cout << "debut de la suppression d'une musique" << std::endl;
    const unsigned int noMusic = atoi((session->get_request()->get_path_parameter("no")).c_str());
    if (!supressionPermission(noMusic,token))
      sendResponse(session, createMethodNotAllowedResponse());
    else {
      removeMusicSelected(token, noMusic);
      removeMP3Selected(std::to_string(noMusic));
      remove(noMusic);
      std::cout << "Retrait de la chanson " << std::endl;
      SysLoggerSingleton::GetInstance().WriteLine("Retrait de la chanson: " + noMusic);
      sendResponse(session, createOkResponse());
    }
  }
}

void get_superviser_files(const std::shared_ptr< restbed::Session > session) {
  std::string result = getListForAdmin(musics);
  session->close( restbed::OK, result, { { "Content-Length", std::to_string(result.size()) }, { "Connection", "close" } } );
}

void delete_superviser_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer song avec le superviseur" << std::endl;
  delete_usager__song(session);
}

void reverse_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "inverser musique" << std::endl; 
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch( content_length, [ request ]( const std::shared_ptr< restbed::Session > session, const restbed::Bytes & body )
  {
    rapidjson::Document document;
    document.SetObject();
    std::string bodyString = std::string(body.begin(), body.end());
    document.Parse<0>(bodyString.c_str(), bodyString.length());
    int first = document["une"].GetInt();
    int second = document["autre"].GetInt();
    reverse(first, second); 
    std::cout << "Modification de l'ordre de passage des chansons" << std::endl;
    SysLoggerSingleton::GetInstance().WriteLine("Modification de l'ordre de passage des chansons");
    session->close( restbed::OK, "", { { "Content-Length", "0" }, { "Connection", "close" } } );
  });
}

void get_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir le volume" << std::endl;
}

void set_up_volume(const std::shared_ptr< restbed::Session > session) {
    std::cout << "augmenter le volume" << std::endl;
}

void set_down_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "diminuer le volume" << std::endl;
}

void enabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "activer mute" << std::endl;
}

void disabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "désactiver mute" << std::endl;
}

void launch_music() {
  ManagerMicroService::run_player();
}
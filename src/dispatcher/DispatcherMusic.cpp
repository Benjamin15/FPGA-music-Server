#include "DispatcherMusic.h"

/**
 * endpoint to get each files
 * 
 */ 
void get_users_files(const std::shared_ptr< restbed::Session > session) {
  std::cout << "GET " << std::endl;
  const int token = atoi(session->get_request()->get_path_parameter( "id" ).c_str());
  if(!identify(token))
    sendResponse(session,createForbiddenResponse());
  else {
    std::cout << "id success" << std::endl;
    updateMusicsOwner(token);
    std::string result = getListForUser(getMusics());
    std::cout << result << std::endl;
    sendResponse(session,createOkResponse(result));
  }
}

/**
 * 
 * endpoint to insert a music in the list
 * 
 */ 
void insert_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "debut de l'insertion de la musique" << std::endl;
  size_t content_length = std::stoi(session->get_request()->get_header("Content-Length"));
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body) {
    const auto request = session->get_request();
    const unsigned int token = atoi(request->get_path_parameter( "id" ).c_str());
    std::cout << token << std::endl;
    if (!identify(token)){
      sendResponse(session,createForbiddenResponse());
    }
    else if (checkListSize() && checkUserMusics(token) && checkUserToken(token)) {
      std::string bodyString(body.begin(), body.end());
      std::string mp3EncodedMusic = bodyString;
      std::string mp3DecodedMusic = base64_decode(mp3EncodedMusic);
      std::string fileName = std::to_string(Music::getNextMusicId("metadata/musics.json"))+".mp3";
      base64_toBinary(mp3DecodedMusic,fileName);
      std::string path = "metadata/musics/" + fileName;
      if (!checkIfMp3(path))
        sendResponse(session, createUnsupportedMediaTypeResponse());
      Music music = get_info(path);
      User user = get_user(token); // Ca fait quoi ça ? Réponse: ça inscrit le user qui a envoyé la chanson
      music.user_ = user;
      music.setMusicNumber();
      insert(music);
      write_log("Soumission d'une nouvelle chanson: " + music.title_);
      sendResponse(session, createOkResponse());
    } else if (!checkListSize()) {
      std::cout << "checkListSize non respecte" << std::endl;
      sendResponse(session, createRequestEntityTooLargeResponse());
    }else {
      std::cout << "erreur interne" << std::endl;
      sendResponse(session, createInternalServerErrorResponse());
    }
  });
}

/**
 * 
 * endpoint to remove an music of the user
 * 
 */ 
void delete_usager_song(const std::shared_ptr< restbed::Session > session) {
  const unsigned int token = atoi(session->get_request()->get_path_parameter( "id" ).c_str());
  if (!identify(token))
    sendResponse(session,createForbiddenResponse());
  else {
    std::cout << "debut de la suppression d'une musique" << std::endl;
    const unsigned int noMusic = atoi((session->get_request()->get_path_parameter("no")).c_str());
    if (!canRemove(noMusic,token))
      sendResponse(session, createMethodNotAllowedResponse());
    else {
      removeMusicSelected(noMusic);
      removeMP3Selected(std::to_string(noMusic));
      remove(noMusic);
      write_log("Retrait de la chanson: " + noMusic);
      sendResponse(session, createOkResponse());
    }
  }
}

/**
 * 
 * endpoint to get the list for the admin
 * 
 */ 
void get_superviser_files(const std::shared_ptr< restbed::Session > session) {
  std::string result = getListForAdmin(getMusics());
  sendResponse(session, createOkResponse(result));
}


/**
 * 
 * endpoint to remove the music, when the user is admin
 * 
 */ 
void delete_superviser_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "supprimer song avec le superviseur" << std::endl;
  const unsigned int noMusic = atoi((session->get_request()->get_path_parameter("no")).c_str());
  removeMusicSelected(noMusic);
  removeMP3Selected(std::to_string(noMusic));
  remove(noMusic);
  write_log("Retrait de la chanson par le superviseur: " + noMusic);
  for (Music music : getMusics())
    std::cout << music.no_ << std::endl;
  add_remove_music();
  sendResponse(session, createOkResponse());
}

/**
 * 
 * endpoint to reverse two songs 
 * 
 */

void reverse_song(const std::shared_ptr< restbed::Session > session) {
  std::cout << "inverser musique" << std::endl; 
  const auto request = session->get_request();
  size_t content_length = std::stoi(request->get_header("Content-Length"));
  session->fetch( content_length, [ request ]( const std::shared_ptr< restbed::Session > session, const restbed::Bytes & body )
  {
    std::cout << "fetch " << std::endl;
    const std::string first_json = "une";
    const std::string second_json = "autre";
    rapidjson::Document document;
    document.SetObject();
    std::string bodyString = std::string(body.begin(), body.end());
    document.Parse<0>(bodyString.c_str(), bodyString.length());
    std::cout << "body : " << bodyString << std::endl;
    unsigned int first = document[first_json.c_str()].GetUint();
    unsigned int second = document[second_json.c_str()].GetUint();
    std::cout << "first : " << first << std::endl;
    std::cout << "second : " << second << std::endl;
    reverse(first, second) ; 
    write_log("Modification de l'ordre de passage des chansons");
    sendResponse(session, createOkResponse(""));
  });
}

/**
 * endpoint to get the volume of music
 * 
 */ 
void get_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "obtenir le volume" << std::endl;
  const std::string user = "admin";
  try {
    checkIfLogin(user);
    int volume = SoundControllerSingleton::GetInstance().getVolume();
    bool mute = SoundControllerSingleton::GetInstance().isMuted();
    std::string response = getVolume(volume, mute);
    std::cout << response << std::endl;
    sendResponse(session, createOkResponse(response));
  }catch (UnauthorizedException) {
    std::cout << "L'administrateur ne s'est pas connecté au préalable" << std::endl;
    sendResponse(session, createUnauthorizedResponse());
  }
}

/**
 * 
 * endpoint to up the volume of music
 * 
 */ 
void set_up_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "augmenter le volume" << std::endl;
  const std::string user = "admin";
  try {
    checkIfLogin(user);
    const int pc = atoi(session->get_request()->get_path_parameter( "pc" ).c_str());
    SoundControllerSingleton::GetInstance().increaseVolume(pc);
    write_log("Augmentation du volume");
    std::string response = getVolume(SoundControllerSingleton::GetInstance().getVolume(), SoundControllerSingleton::GetInstance().isMuted());
    std::cout << response << std::endl;
    sendResponse(session, createOkResponse(response));
  }catch (UnauthorizedException) {
    std::cout << "L'administrateur ne s'est pas connecté au préalable" << std::endl;
    sendResponse(session, createUnauthorizedResponse());
  }
}

/**
 * 
 * endpoint to down the volume of music 
 * 
 */ 
void set_down_volume(const std::shared_ptr< restbed::Session > session) {
  std::cout << "diminuer le volume" << std::endl;
  const std::string user = "admin";
  try {
    checkIfLogin(user);
    const int pc = atoi(session->get_request()->get_path_parameter( "pc" ).c_str());
    SoundControllerSingleton::GetInstance().decreaseVolume(pc);
    write_log("Diminution du volume");
    std::string response = getVolume(SoundControllerSingleton::GetInstance().getVolume(), SoundControllerSingleton::GetInstance().isMuted());
    std::cout << response << std::endl;
    sendResponse(session, createOkResponse(response));
  }catch (UnauthorizedException) {
    std::cout << "L'administrateur ne s'est pas connecté au préalable" << std::endl;
    sendResponse(session, createUnauthorizedResponse());
  }
}

/**
 * 
 * endpoint to mute the music 
 * 
 */ 

void enabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "activer mute" << std::endl;
  const std::string user = "admin";
  try {
    checkIfLogin(user);
    SoundControllerSingleton::GetInstance().mute();
    write_log("Activation de la sourdine");
    std::string response = getVolume(SoundControllerSingleton::GetInstance().getVolume(), SoundControllerSingleton::GetInstance().isMuted());
    std::cout << response << std::endl;
    sendResponse(session, createOkResponse(response));
  }catch (UnauthorizedException) {
    std::cout << "L'administrateur ne s'est pas connecté au préalable" << std::endl;
    sendResponse(session, createUnauthorizedResponse());
  }
}

/**
 * 
 * endpoint to disabled the mute of the music
 * 
 */ 

void disabledMute(const std::shared_ptr< restbed::Session > session) {
  std::cout << "désactiver mute" << std::endl;
  const std::string user = "admin";
  try {
    checkIfLogin(user);
    SoundControllerSingleton::GetInstance().unmute();
    write_log("Désactivation de la sourdine");
    std::string response = getVolume(SoundControllerSingleton::GetInstance().getVolume(), SoundControllerSingleton::GetInstance().isMuted());
    std::cout << response << std::endl;
    sendResponse(session, createOkResponse(response));
  }catch (UnauthorizedException) {
    std::cout << "L'administrateur ne s'est pas connecté au préalable" << std::endl;
    sendResponse(session, createUnauthorizedResponse());
  }
}
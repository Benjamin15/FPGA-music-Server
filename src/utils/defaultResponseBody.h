#pragma once

namespace responseBody{

    constexpr const auto OK = "Ok"; //200
    constexpr const auto BAD_REQUEST = "Mauvaise requete (erreur dans le json)"; //400
    constexpr const auto BAD_REQUEST_V2 = "Mauvaise requete (mauvais nouveau mot de passe ou erreur dans le json)"; //400
    constexpr const auto UNAUTHORIZED = "Utilisateur non authentifie"; //401
    constexpr const auto FORBIDDEN = "Acces refuse"; //403
    constexpr const auto FORBIDDEN_V2 = "Non autorise"; //403
    constexpr const auto METHOD_NOT_ALLOWED = "refuse (la chanson n'appartient pas a l'usager ou mauvaise chanson)"; //405
    constexpr const auto CONFLICT = "l'une ou l'autre (ou les deux) chansons ne sont plus dans la liste en attente"; //409
    constexpr const auto REQUEST_ENTITY_TOO_LARGE = "chanson trop longue, file pleine sur le serveur ou limite de chansons dans la file pour l'usager atteinte"; //413
    constexpr const auto UNSUPPORTED_MEDIA_TYPE = "le fichier soumis n'est pas un MP3 ou n'a pas d'en-tete ID3"; //415
    constexpr const auto INTERNAL_SERVER_ERROR = "erreur serveur (service non disponible)"; //500

}
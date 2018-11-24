#include "Rest.h"

void ready_handler( restbed::Service&  data)
{
    auto uri_http = data.get_http_uri();
    //auto uri_https = data.get_https_uri();
    fprintf( stderr, "Service PID is '%i'.\n", getpid( ) );
    std::cout << uri_http->to_string() <<  std::endl;
   // std::cout << uri_https->to_string() <<  std::endl;

} 

void failed_filter_validation_handler( const std::shared_ptr< restbed::Session > session )
{
    std::cout << "erreur " << std::endl;
    session->close( 400 );
}

/**
 * Map each functions
 * We need that because c++ doesn't implement reflection. So, it's the good way to do a pseudo reflection
 */ 
std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>> Rest::mapFunction(){
    std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>>  funcMap =
    {
        { "get_usager_identification_handler", connect},
        { "get_usager_file_handler", get_users_files},
        { "post_usager_chanson_handler", insert_song},
        { "delete_usager_chanson_handler", delete_usager_song},
        { "get_superviseur_file_handler", get_superviser_files},
        { "delete_superviseur_chanson_handler", delete_superviser_song},
        { "post_superviseur_inversion_handler", reverse_song},
        { "get_superviseur_volume_handler", get_volume},
        { "post_superviseur_volume_augmenter_handler", set_up_volume},
        { "post_superviseur_volume__diminuer_handler", set_down_volume},
        { "post_superviseur_volume_sourdine_activer_handler", enabledMute},
        { "post_superviseur_volume_sourdine_desactiver_handler", disabledMute},
        { "get_superviseur_statistiques", get_statistics},
        { "get_superviseur_bloquer", lock},
        { "get_superviseur_debloquer", unlock},
        { "get_superviseur_listenoire", get_black_list},
        { "post_superviseur_login", login},
        { "post_superviseur_logout", logout},
        { "post_superviseur_changement_mdp", set_password}, 
        { "get_superviseur_user", get_users}  
    };
    return funcMap;
}

Rest::Rest() {
    service_ = std::make_shared< restbed::Service >( );
    settings_ = std::make_shared< restbed::Settings >( );
    ssl_settings_ = std::make_shared< restbed::SSLSettings >( );
}


/**
 * Init service
 */ 
void Rest::initService() {
    service_->set_ready_handler( ready_handler );
}


/**
 * init settings server
 * 
 */ 
void Rest::initSettings(std::string address, uint16_t port) {
    settings_->set_worker_limit( 4 );
    //initSSLSettings(address);
    settings_->set_bind_address(address);
    settings_->set_port(port);
}

void Rest::initSSLSettings(std::string address) {
    ssl_settings_->set_private_key( restbed::Uri( "file://certificates/server.key" ) );
    ssl_settings_->set_certificate( restbed::Uri( "file://certificates/server.crt" ) );
    ssl_settings_->set_temporary_diffie_hellman( restbed::Uri( "file://certificates/dh768.pem" ) );
    ssl_settings_->set_bind_address(address);
    settings_->set_ssl_settings( ssl_settings_ );
}


/**
 * create each route in the server
 * 
 */ 
void Rest::createRoute(){
    std::vector<std::shared_ptr<restbed::Resource>> resources;
    rapidjson::Document document = getJsonFile("metadata/routes.json");
    const rapidjson::Value& routes = document["routes"];
    std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>> functions = mapFunction();
    for (rapidjson::SizeType i = 0; i < routes.Size(); i++) { // boucle chaque route
        auto resource = std::make_shared<restbed::Resource>();
        rapidjson::Value::ConstMemberIterator itr = routes[i].MemberBegin();
        const std::string type = itr++->value.GetString();
        const std::string url = itr++->value.GetString();
        const std::string handler = itr++->value.GetString();
        resource->set_path(url);
        resource->set_failed_filter_validation_handler( failed_filter_validation_handler );
        resource->set_method_handler( type,functions[handler] );  
        resources.push_back(resource);
    }
    for (auto resource : resources) {
        service_->publish(resource);
    }
}

/**
 * Run the rest server
 * 
 */ 
void Rest::run() {
    create_list_music();
    run_player();
    service_->start( settings_ );
}

int main( const int argc , const char* argv[] ) {
    Rest rest;
    const std::string ip_fpga = "132.207.89.35";
    const int port_fpga = 80;
    std::string address = argc > 1 ? argv[1] : ip_fpga;
    uint16_t port = argc > 2 ? std::stoi(argv[2]) : port_fpga;
    rest.initSettings(address, port);
    rest.initService();
    rest.createRoute();
    rest.run();
    return EXIT_SUCCESS;
}
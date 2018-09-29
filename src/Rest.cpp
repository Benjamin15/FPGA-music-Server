#include "Rest.h"

void ready_handler( restbed::Service& )
{
    fprintf( stderr, "Service PID is '%i'.\n", getpid( ) );
}

/**
 * Map each functions
 * We need that because c++ doesn't implement reflection. So, it's the good way to do a pseudo reflection
 */ 
std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>> Rest::mapFunction(){
    std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>>  funcMap =
        {
            { "get_usager_identification_handler", ManagerUser::identify},
            { "get_usager_file_handler", ManagerMusic::get_usager_files},
            { "post_usager_chanson_handler", ManagerMusic::insert_song},
            { "delete_usager_chanson_handler", ManagerMusic::delete_usager__song},
            { "get_superviseur_file_handler", ManagerMusic::get_superviser_files},
            { "delete_superviseur_chanson_handler", ManagerMusic::delete_superviser_song},
            { "post_superviseur_inversion_handler", ManagerMusic::reverse_song},
            { "get_superviseur_volume_handler", ManagerMusic::get_volume},
            { "post_superviseur_volume_augmenter_handler", ManagerMusic::set_up_volume},
            { "post_superviseur_volume__diminuer_handler", ManagerMusic::set_down_volume},
            { "post_superviseur_volume_sourdine_activer_handler", ManagerMusic::enabledMute},
            { "post_superviseur_volume_sourdine_desactiver_handler", ManagerMusic::disabledMute},
            { "get_superviseur_statistiques", ManagerStatistiques::get_statistiques},
            { "get_superviseur_bloquer", ManagerUser::lock},
            { "get_superviseur_debloquer", ManagerUser::unlock},
            { "get_superviseur_listenoire", ManagerUser::get_black_list},
            { "post_superviseur_login", ManagerUser::login},
            { "post_superviseur_logout", ManagerUser::logout},
            { "post_superviseur_changement_mdp", ManagerUser::set_password}
        };
    return funcMap;
}

Rest::Rest() {
    service = std::make_shared< restbed::Service >( );
    settings = std::make_shared< restbed::Settings >( );
}


/**
 * Init service
 */ 
void Rest::initService() {
    service->set_ready_handler( ready_handler );
}


/**
 * init settings server
 * 
 */ 
void Rest::initSettings() {
    settings->set_port( 80 );
    settings->set_default_header( "Connection", "close" );
    settings->set_bind_address("132.207.89.35");
}

/**
 * create each route in the server
 * 
 */ 
void Rest::createRoute(){
    std::vector<std::shared_ptr<restbed::Resource>> resources;
    rapidjson::Document document = getJsonFile();
    const rapidjson::Value& routes = document["routes"];
    std::map<std::string, std::function<void( const std::shared_ptr< restbed::Session > session )>> functions = mapFunction();
    std::cout << functions.size() << std::endl;
    for (rapidjson::SizeType i = 0; i < routes.Size(); i++) { // boucle chaque route
        auto resource = std::make_shared<restbed::Resource>();
        rapidjson::Value::ConstMemberIterator itr = routes[i].MemberBegin();
        const std::string type = itr++->value.GetString();
        const std::string url = itr++->value.GetString();
        const std::string handler = itr->value.GetString();
        resource->set_path(url);
        resource->set_method_handler( type, functions[handler] );  
        resources.push_back(resource);
    }
    for (auto resource : resources){
        service->publish(resource);
    }
}


/**
 * Get the JSON file which contain each routes
 * 
 */ 
rapidjson::Document Rest::getJsonFile(){
    fprintf( stderr, "get JSON FILE" );
    FILE* fp = fopen("routes.json", "rb"); // non-Windows use "r"
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream(is);
    fclose(fp);
    return d;
}

/**
 * Run the rest server
 * 
 */ 
void Rest::run(){
    service->start( settings );
}

int main( const int, const char** )
{
    Rest rest;
    rest.initSettings();
    rest.initService();
    rest.createRoute();
    rest.run();
    return EXIT_SUCCESS;
}
#include "Rest.h"

void sighup_handler( const int signal_number )
{
    fprintf( stderr, "Received SIGINT signal number '%i'.\n", signal_number );
}

void sigterm_handler( const int signal_number )
{
    fprintf( stderr, "Received SIGTERM signal number '%i'.\n", signal_number );
}

void ready_handler( restbed::Service& )
{
    fprintf( stderr, "Service PID is '%i'.\n", getpid( ) );
}

void post_identification_handler( const std::shared_ptr< restbed::Session > session )
{
    fprintf( stderr, "I am post_identification_handler" );
}

Rest::Rest(){
    resource = std::make_shared< restbed::Resource >( );
    service = std::make_shared< restbed::Service >( );
    settings = std::make_shared< restbed::Settings >( );
}

void Rest::initService() {
    service->set_ready_handler( ready_handler );
    service->set_signal_handler( SIGINT, sighup_handler );
    service->set_signal_handler( SIGTERM, sigterm_handler );
}

void Rest::initSettings() {
    settings->set_port( 80 );
    settings->set_default_header( "Connection", "close" );
    settings->set_bind_address("132.207.89.35");
}

void Rest::createRoute(){
    resource->set_path( "/usager/identification" );
    resource->set_method_handler( "GET", post_identification_handler );  
    service->publish( resource );
}

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
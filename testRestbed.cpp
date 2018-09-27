#include <memory>
#include <cstdlib>
#include <restbed>
#include <csignal>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
using namespace restbed;

void post_method_handler( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );

    int content_length = request->get_header( "Content-Length", 0 );
    
    session->fetch( content_length, [ ]( const shared_ptr< Session > session, const Bytes & body )
    {
        fprintf( stdout, "%.*s\n", ( int ) body.size( ), body.data( ) );
        session->close( OK, "Hello, World!", { { "Content-Length", "13" } } );
    } );
}

void sighup_handler( const int signal_number )
{
    fprintf( stderr, "Received SIGINT signal number '%i'.\n", signal_number );
}

void sigterm_handler( const int signal_number )
{
    fprintf( stderr, "Received SIGTERM signal number '%i'.\n", signal_number );
}

void ready_handler( Service& )
{
    fprintf( stderr, "Service PID is '%i'.\n", getpid( ) );

}

int main( const int, const char** )
{
    Service service;
    auto resource = make_shared< Resource >( );
    resource->set_path( "/resource" );
    resource->set_method_handler( "GET", post_method_handler );

    service.set_ready_handler( ready_handler );
    service.set_signal_handler( SIGINT, sighup_handler );
    service.set_signal_handler( SIGTERM, sigterm_handler );

    auto settings = make_shared< Settings >( );
    settings->set_port( 80 );
    settings->set_default_header( "Connection", "close" );
    settings->set_bind_address("132.207.89.35");
  
    service.publish( resource );
    service.start( settings );

    return EXIT_SUCCESS;
}
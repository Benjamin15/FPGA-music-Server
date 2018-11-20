#include <memory>
#include <cstdlib>
#include <restbed>
#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <cstdio>

using namespace std;
using namespace restbed;


void ready_handler( restbed::Service&  data)
{
    auto uri = data.get_http_uri();
    fprintf( stderr, "Service PID is '%i'.\n", getpid( ) );
    std::cout << uri->to_string() <<  std::endl;
} 


void get_method_handler( const shared_ptr< Session > session )
{
    session->close( OK, "Hello, World!", { { "Content-Length", "13" }, { "Connection", "close" } } );
}

int main( const int, const char** )
{
    auto resource = make_shared< Resource >( );
    resource->set_path( "/resource" );
    resource->set_method_handler( "GET", get_method_handler );
    
    auto ssl_settings = make_shared< SSLSettings >( );
    ssl_settings->set_http_disabled( true );
    
    ssl_settings->set_private_key( Uri( "file://server.key" ) );
    ssl_settings->set_certificate( Uri( "file://server.crt" ) );
    ssl_settings->set_temporary_diffie_hellman( Uri( "file://dh768.pem" ) );
    ssl_settings->set_bind_address("192.168.2.48");
    std::cout << "ip ssl : " << ssl_settings->get_bind_address()  <<std::endl;
    std::cout << "port ssl : " << ssl_settings->get_port()  << std::endl;

    auto settings = make_shared< Settings >( );
    settings->set_ssl_settings( ssl_settings );
    settings->set_bind_address("192.168.2.48");
    
    settings->set_port(8080);
    Service service;
    service.set_ready_handler( ready_handler );
    service.publish( resource );
    service.start( settings );
    
    return EXIT_SUCCESS;
}
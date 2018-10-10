#include "catch.hpp"
#include <curl/curl.h>
#include <vector>
#include <string>
#include "./../src/model/Music.h"
#include "./../src/manager/ManagerMusic.h"

int sendCurlRequest(CURL* curl, CURLoption curlRequest, std::string urlRequest){
    int httpCode(0);
    curl_easy_setopt(curl, CURLOPT_URL, urlRequest);
    curl_easy_setopt(curl, curlRequest, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    return httpCode;
}

TEST_CASE("Insert a song"){
    int httpCode(0);
    std::string badURL= "http://132.207.84.35:80/usager/chanson/3";
    std::string correctURL= "http://132.207.89.35:80/usager/chanson/3";
    CURL *curl = curl_easy_init();
    if(curl) {  
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }       
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Add volume"){
    int httpCode(0);
    std::string badURL= "http://132.207.89.35:80/superviseur/volume/augmenter/12";
    std::string correctURL= "http://132.207.89.35:80/superviseur/volume/augmenter/12"; 
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }      
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Low volume"){
    int httpCode(0);
    std::string badURL= "http://132.207.85.35:80/superviseur/volume/diminuer/12";
    std::string correctURL= "http://132.207.89.35:80/superviseur/volume/diminuer/12";
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }       
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Deactivate mute"){
    int httpCode(0);
    std::string badURL= "http://132.207.89.35:80/supervieur/volume/sourdine/desactiver";
    std::string correctURL= "http://132.207.89.35:80/superviseur/volume/sourdine/desactiver"; 
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }       
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Block a user by supervisor"){
    int httpCode(0);
    std::string badURL= "http://132.207.89.35:80/superviser/bloquer";
    std::string correctURL= "http://132.207.89.35:80/superviseur/bloquer"; 
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }    
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Unblock a user by supervisor"){
    int httpCode(0);
    std::string badURL= "http://132.207.85.35:80/superviseur/debloquer";
    std::string correctURL= "http://132.207.89.35:80/superviseur/debloquer";
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }     
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Supervisor login"){
    int httpCode(0);
    std::string badURL= "http://132.207.85.35:80/superviseur/login";
    std::string correctURL= "http://132.207.89.35:80/superviseur/login"; 
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }    
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Supervisor logout"){
    int httpCode(0);
    std::string badURL= "http://132.207.85.35:80/superviseur/logout";
    std::string correctURL= "http://132.207.89.35:80/superviseur/logout"; 
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }       
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("Change password"){
    int httpCode(0);
    std::string badURL= "http://132.207.89.35:80/supervisr/changement_mdp";
    std::string correctURL= "http://132.207.89.35:80/superviseur/changement_mdp"; 
    CURL *curl = curl_easy_init();
    if(curl) { 
        SECTION("When URL is incorrect it should return ERROR 22 in CURL = ERROR 400 in HTTP"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, badURL);
            REQUIRE(httpCode==401);
        }
        SECTION("When URL is correct it should return code 0"){
            httpCode= sendCurlRequest(curl, CURLOPT_POST, correctURL);
            REQUIRE(httpCode==200);
        }      
        curl_easy_cleanup(curl); 
    }
}

TEST_CASE("List of musics creation") {
    ManagerMusic managerMusic;
    std::vector<Music>expected;
    std::vector<Music>actual;

    User user1(3, "personne1", "127.0.0.1", "A0:B1:C3:D4:E5:F6");
    User user2(2,"personne2", "132.192.169.1", "A1:B2:C3:D1:E9:F4");
    User user3(1, "personne3", "127.109.0.2", "A2:B4:C6:D2:E7:F5");
    
    Music music1(1, "titre1", "artiste1", "7.00", user1);
    Music music2(2,"titre2", "artiste2", "6.00", user2);
    Music music3(3, "titre3", "artiste3", "9.00", user3);
    
    expected.push_back(music1);
    expected.push_back(music2);
    expected.push_back(music3);

    managerMusic.create_list_music();

    SECTION ("The expected vector's size is equal to the actual one'") {
        REQUIRE(expected.size()== managerMusic.musics.size());
    }
    
    SECTION("The data in the expected vector is equal to the actual one") {
        REQUIRE(expected==managerMusic.musics);
    }
    
}
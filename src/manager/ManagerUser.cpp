#include "ManagerUser.h"

void ManagerUser::identify(const std::shared_ptr< restbed::Session > session){
  std::cout << "identification en cours" << std::endl;
  
  const auto request = session->get_request();
  size_t content_length = request->get_body().size();

  std::multimap<std::string, std::string> headers = request->get_headers();
  
  std::multimap<std::string, std::string> params = session->get_request()->get_query_parameters();
  auto pars = session->get_request()->get_path_parameters();
  auto body = session->get_request()->get_body();
  std::cout << body.data() << std::endl;

for (auto itMap : params)
{
  ManagerUser::Register("sdbskj");
}
for(auto it : headers)
{
    std::cout << it.first << std::endl;
    std::cout << it.second << std::endl;

}
  
  session->fetch(content_length,[](const std::shared_ptr< restbed::Session >& session,
  const restbed::Bytes& body){
    std::cout << "ici :" <<  body.data();
    fprintf( stdout, "%.*s\n", ( int ) body.size( ), body.data( ));
    //ManagerIdentification::registerId(std::string(body.begin(),body.end()));
    session->close(restbed::OK,"Réponse",{{"Content-Length","18"},
    {"Connection","close"}});
  });
}

int ManagerUser::Register(std::string body){
  FILE* fp = fopen("idLogs.json", "rb");
  char buffer_reader[65536];
  char buffer_writer[65536];
  rapidjson::FileReadStream is(fp, buffer_reader, sizeof(buffer_reader));
  rapidjson::Document doc;
  doc.ParseStream(is);
  fclose(fp);
  fp = fopen("idLogs.json", "wb");
  rapidjson::Value& items = doc["Users"];
  items.Erase(items.Begin());
  doc.Parse(body.c_str());
  rapidjson::FileWriteStream os(fp, buffer_writer, sizeof(buffer_writer));
  rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
  doc.Accept(writer);
  fclose(fp);
  return 0;
}

void ManagerUser::lock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "bloquer" << std::endl;
}

void ManagerUser::unlock(const std::shared_ptr< restbed::Session > session) {
  std::cout << "debloquer" << std::endl;
}

void ManagerUser::get_black_list(const std::shared_ptr< restbed::Session > session) {
  std::cout << "liste noire" << std::endl;
}

void ManagerUser::login(const std::shared_ptr< restbed::Session > session) {
  std::cout << "login" << std::endl;
}

void ManagerUser::logout(const std::shared_ptr< restbed::Session > session) {
  std::cout << "logout" << std::endl;
}

void ManagerUser::set_password(const std::shared_ptr< restbed::Session > session) {
  std::cout << "changement mot de passe" << std::endl;
}
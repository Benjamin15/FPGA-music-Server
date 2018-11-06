#include "base64.h"



const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
/**
 * Check if the char is base64
 * 
 */
inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

/**
 * Decode base-64 to string
 * 
 */

std::string base64_decode(std::string input) {
  using namespace boost::archive::iterators;
  typedef transform_width<binary_from_base64<remove_whitespace
      <std::string::const_iterator> >, 8, 6> ItBinaryT;

  try
  {
    // If the input isn't a multiple of 4, pad with =
    size_t num_pad_chars((4 - input.size() % 4) % 4);
    input.append(num_pad_chars, '=');

    size_t pad_chars(std::count(input.begin(), input.end(), '='));
    std::replace(input.begin(), input.end(), '=', 'A');
    std::string output(ItBinaryT(input.begin()), ItBinaryT(input.end()));
    output.erase(output.end() - pad_chars, output.end());
    return output;
  }
  catch (std::exception const&)
  {
    return std::string("");
  }
}

void base64_toBinary(std::string base64music,std::string title){
    std::string path = "metadata/musique/" + title ;
    FILE* fp = fopen(path.c_str(),"w+");
    fclose(fp);
    char * buf;
    int size = base64music.size();
    std::ofstream output;
    output.open(path, std::ios::out | std::ios::binary);
    if(!output){
        std::cout<<"Can't open file"<<std::endl;
        exit(1);
    }
    output.write(reinterpret_cast<char*>(&size),sizeof(int));
    output.write(base64music.c_str(),size);
    output.flush();
    output.close();
}
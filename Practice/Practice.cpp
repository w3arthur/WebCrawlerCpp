
#include <iostream>
#include <filesystem>



#include <uri-parser/Uri.h>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <network/uri.hpp>  //https://github.com/cpp-netlib/uri

using namespace std;



// trim from both ends (in place)



int main() {
    
    ////use string_view
    //std::string uri1 = "https://example.com/";
    //std::string uri2 = "/path/to/resource";
    //std::cout << combiner(uri1, uri2) << std::endl;
    std::filesystem::path p1 = "/a/b/c";
    std::filesystem::path p2 = "../d/e/f";

      p1 += p2;
    cout << p1;
    return 0;
}







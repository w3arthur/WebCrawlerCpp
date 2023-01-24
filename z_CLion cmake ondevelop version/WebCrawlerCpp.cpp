// WebCrawler Entry Point
#include "WebCrawlerCpp.h"

#include <iostream>
#include <curl/curl.h>  //vcpkg
#include <fstream>
#include <nlohmann/json.hpp>  //vcpkg

using std::cout, std::boolalpha, std::endl;
using WebCrawlerCpp::Main;
using json = nlohmann::json;

bool Main::isCliProduction {false};
int Main::timeout {5 * 60}; //sec
string Main::filePath {};
string Main::filePath_Debugger {};
string Main::fileName {"results.json"};
string Main::url {};
int Main::depth {0};

//https://github.com/nlohmann/json#examples

size_t got_data(char* buffer, size_t itemsize, size_t nitems, void* ignorethis)
{
    size_t bytes = itemsize * nitems;
    cout << "chunk size " <<  bytes << endl;
    /* print each line, numbering example */
    int linenumber{0};
    for(int i{}; i < bytes; ++i) { cout << buffer[i];  if (buffer[i] == '\n') cout << ++linenumber << "\t"; }
    cout << endl << endl;
    /* */

    return bytes;
}




int Main::main(int argc, char *argv[])
{
    cout << "WebCrawler Cpp" << endl;
    //if(argc > 0 || isCliProduction){} else{}


    CURL *curl = curl_easy_init();
    if(!curl) { cout << "init failed" << endl; return EXIT_FAILURE; }
    curl_easy_setopt(curl, CURLOPT_URL, "https://arthurcam.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);
    string response;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode result = curl_easy_perform(curl); //download the content
    if(result != CURLE_OK) cout << "download problem" << endl;

    cout << endl << "ok" << endl;

    curl_easy_cleanup(curl);
    return EXIT_SUCCESS;
}



int main(int argc, char *argv[])  //run the app
{
    try { return Main::main(argc, argv); }
    catch (...) { return EXIT_FAILURE; }
}


//please fix program on a visual-studio
//please fix cmake boost warning.


// -G Ninja -DCMAKE_BUILD_TYPE=Debug "-DCMAKE_PROGRAM=C:/Program Files/JetBrains/Clion 2022.2.3/bin/ninja/win/ninja.exe" -DCMAKE_TOOLCHAIN_FILE=C:/src/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake

//installing info
//(integrated) vcpkg.exe install curl:x64-windows
//
// or     (C:\src\vcpkg)  .\vcpkg\vcpkg install curl:x64-windows
// or set env VCPKG_DEFAULT_TRIPLET = x64-windows   (C:\src\vcpkg)  .\vcpkg\vcpkg install curl
//
// install https://github.com/microsoft/vcpkg
// install https://curl.se/download.html
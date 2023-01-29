// CppDevelopment.cpp : This file contains the 'main' function. Program execution 
// begins and ends there.
//

#define DEBUG_MODE 1

//#include <iostream>

#include "../CrawlerLib/pch.h"
#include <CrawlerRun.h>

using std::cout;
using std::endl;

// make the testing
// to do, to recheck 'curl' package for x64, with DLL from VCPKG or from them site
// create task time limit to compile, like an exit token


int main(int argc, char* argv[])
{

    cout << "WebCrawler Cpp:" << endl;

    std::string uri1;
    size_t limitLevels;

    //limitLevels = 3 ;
    //uri1 = "https://arthurcam.com";

    if (argc != 3)  // https://arthurcam.com 3
    {
        std::cout << "enogh args" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    uri1 = static_cast<string>(argv[1]);
    limitLevels = static_cast<size_t>(std::stoi(argv[2]));





    ICrawlerRun* crowlerRun = new CrawlerRun(uri1, limitLevels) ;

    crowlerRun->write_to_file("result.json");


    cout << "\n\n\nImages HERE:\n";
    crowlerRun->print();

    cout << "\n\n" << endl;

    auto returnString{ crowlerRun->to_string() };
   // cout << returnString << endl;
    cout << "\n\n" <<endl; //crashed!

    delete crowlerRun;





#if DEBUG_MODE == 1
   std::cout << "\n\nDebugging mode\n";
   std::cin.get();  //debug  
#endif // DEBUG
}

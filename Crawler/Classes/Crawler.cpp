// CppDevelopment.cpp : This file contains the 'main' function. Program execution 
// begins and ends there.
//

//https://github.com/nlohmann/json


#include "CrawlerRun.h"

#define DEBUG_MODE 1




//#define combiner AddressCombiner::combiner;

//using namespace std



int main(int argc, char* argv[])
{

    cout << "WebCrawler Cpp:" << endl;
    size_t limitLevels{ 3 };

    string uri1{ "https://arthurcam.com" };

    CrawlerRun crowlerRun(uri1, limitLevels);

    crowlerRun.write_to_file("result.json");


    cout << "\n\n\nImages HERE:\n";
    crowlerRun.print();

    cout << "\n\n" << endl;

    auto returnString{ crowlerRun.to_string() };
   // cout << returnString << endl;
    cout << "\n\n" <<endl; //crashed!




    //if (argc != 2)
    //{
    //    std::cout << "Please provide a valid English word" << std::endl;
    //    exit(EXIT_FAILURE);
    //}
    //if(argc > 0 || isCliProduction){} else{}










#if DEBUG_MODE == 1
    std::cout << "\n\nDebugging mode\n";
    //for (auto p : persons)
    //    cout << p << "\n";
   // cin.get();  //debug  
#endif // DEBUG
}








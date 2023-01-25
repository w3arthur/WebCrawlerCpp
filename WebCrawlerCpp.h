#pragma once

#include <string>
using std::string;

namespace WebCrawlerCpp
{
    class Main
    {
    public:
        static bool isCliProduction;
        static int timeout;   //sec
        static string filePath;
        static string filePath_Debugger;
        static string fileName;
        static string url;
        static int depth;
        static int main(int argc, char *argv[]);
    };
}


int main(int argc, char *argv[]);


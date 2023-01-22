#include "pch.h"
#include "AddressCombiner.h"

// add case for ../
// add loop for recheck, and another if needed
// another cases
// check for another cases with include


string combiner(string uri1, string& uri2)
{

    {    //uri 1 as copy
        if (uri2.find("://") != std::string::npos) return uri2;
        auto trim = [](string& s)
        {
            while (!s.empty() && s.front() == ' ') s = s.substr(1);
            while (!s.empty() && s.back() == ' ') s.pop_back();
        };
        trim(uri1);
        trim(uri2);
        while (uri1.back() == '/') uri1.pop_back();
        while (uri2.back() == '/') uri2.pop_back();
        if (string str = "//";  uri2.substr(0, 2) == "//" && uri2.length() > 2)
        {
            auto endOfStr = std::find_end(uri1.begin(), uri2.end(), str.begin(), str.end());
            uri1 = uri1.substr(0, distance(uri1.begin(), endOfStr));
            uri2 = uri2.substr(2);
        }
        while (uri2.substr(0, 2) == "./") { uri2 = uri2.substr(2); }
        while (uri2.substr(0, 3) == "../") {}//contains  [A-Z]/ regex
        if (uri1.back() != '/' && uri2.front() != '/') uri1 += '/';
        string combined_uri = uri1 + uri2;
        return combined_uri;
    }

}

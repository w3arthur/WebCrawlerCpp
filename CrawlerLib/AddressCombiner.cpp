#include "pch.h"
#include "AddressCombiner.h"


// add case for ../
// add loop for recheck, and another if needed, and empty() checking
// another cases
// check for another cases with include
// fix //
// test all the cases
//move to string_view
// default http
// default not require www



void dotDotSlashCase(string& uri1, string& uri2);

string combiner(string uri1, string uri2)
{
    {    //uri 1 as copy
        //if (uri1.find("://" != uri1.npos())
        if (uri2.find("://") != std::string::npos) return uri2;
        //else  return uri1 + uri2;

        auto trim = [](string& s)
        {
            while (!s.empty() && s.front() == ' ') s = s.substr(1);
            while (!s.empty() && s.back() == ' ') s.pop_back();
        };
        trim(uri1);
        trim(uri2);
        while (!uri1.empty() && uri1.back() == '/') uri1.pop_back();
        while (!uri2.empty() && uri2.back() == '/')  uri2.pop_back();

        /*       if (string str = "//";  !uri2.empty() && uri2.length() > 2 && uri2.substr(0, 2) == "//")
               {
                   auto endOfStr = std::find_end(uri1.begin(), uri2.end(), str.begin(), str.end());
                   uri1 = uri1.substr(0, distance(uri1.begin(), endOfStr));
                   uri2 = uri2.substr(2);
               }*/



     
        

        while (!uri2.empty())   //not enooght for a break condition!
        {
            if (uri2.size() > 2)
            {
                string sub1Char = uri2.substr(0, 1);  //not needed
                string sub2Char = uri2.substr(0, 2);
                string sub3Char = uri2.substr(0, 3);

                if (sub2Char == "./") { uri2 = uri2.substr(2); }
                else if (sub3Char == "../")
                {
                    dotDotSlashCase(uri1, uri2);
                }
                else if (sub2Char == "//") 
                {
                    uri2 = uri2.substr(2);
                    size_t doubleslash = uri1.find("//") + 2;
                    string before = uri1.substr(0, doubleslash);
                    if (!before.empty()) uri1 = before;
                    else uri1 = "";
                    break;
                }
                else if (sub1Char == "/")
                {
                   
                    uri2 = uri2.substr(1);
                    size_t doubleslash = uri1.find("//") + 1;

                    size_t slash = uri1.find_last_of("/");
                    string before = uri1.substr(0, slash);
                    string after = uri1.substr(slash + 1);;
     
                    while (!before.empty() && before != after && doubleslash != slash)
                    {
                        uri1 = before;
                        slash = uri1.find_last_of("/");
                        before = uri1.substr(0, slash);
                        after = uri1.substr(slash + 1);
                    }

                    break;
                }
                // stupied cases when starts with ////////////////// for uri1 and 2
                //else if (sub1Char == " ") { uri2 = "%20" + uri2.substr(1); }  //future fixes for URI address


                else break; //no other cases awailable

            }
            else if (uri2.size() > 1 && uri2.substr(0, 2) == "..")
            {
                dotDotSlashCase(uri1, uri2);
            }
            else if (uri2.size() > 0 && uri2.substr(0, 1) == ".")
            {
                uri2 = uri2.substr(1);
            }
            else if (uri2.size() > 0 && uri2.substr(0, 1) == "/")
            {
                // --, copy the case ...
            }
            else break; //no other cases awailable
        }




        //if first char is ' '

        while (!uri2.empty() && uri2.substr(0, 2) == "//")
        {
            //string/it subStringDistance = uri1 //find distance from the end
            //if(! (distance uri1.begin(), subStringDistance == 0 or -1 == '/') )
                // { erase uri1.begin() - subStringDistance, uri1.end() }
        }//contains  [A-Z]/ regex

        //combined rule, adding a '/' betwin uri1/uri2
        if (!uri1.empty() && !uri2.empty() && uri1.back() != '/' && uri2.front() != '/') uri1 += '/';
        string combined_uri = uri1 + uri2;
        return combined_uri;
    }

}




void dotDotSlashCase(string& uri1, string& uri2)
{
    uri2 = uri2.size() > 2 ? uri2.substr(3) : "";
    size_t slash = uri1.find_last_of("/");
    size_t doubleslash = uri1.find("//") + 1;   //or :// +2
    string before = uri1.substr(0, slash);
    string after = uri1.substr(slash + 1);
    //bool isAnAddress
    if (!before.empty() && before != after && doubleslash != slash)  uri1 = before; //uri1 = uri1.substr(0, slash)
}
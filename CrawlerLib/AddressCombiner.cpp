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





string combiner(string uri1, string uri2)
{
    {    //uri 1 as copy
        //if (uri1.find("://" != uri1.npos())
        string_view sub_uri1 = uri1;
        string_view sub_uri2 = uri2;
        if (sub_uri2.find("://") != uri2.npos) return uri2;
        else if (!sub_uri2.empty() && sub_uri2.size() == 1 && sub_uri2.substr(0, 1) == "/")
            slashCase(sub_uri1, sub_uri2);

        auto trim = [](string_view& s)
        {
            while (!s.empty() && s.front() == ' ') s = s.substr(1);
            while (!s.empty() && s.back() == ' ') s = s.substr(0, s.size() - 1);//s.pop_back();
        };


        trim(sub_uri1);
        trim(sub_uri2);

        while (!sub_uri1.empty() && sub_uri1.back() == '/') sub_uri1 = sub_uri1.substr(0, sub_uri1.size() - 1);
        while (!sub_uri2.empty() && sub_uri2.back() == '/')  sub_uri2 = sub_uri2.substr(0, sub_uri2.size() - 1);


        while (!sub_uri2.empty() && sub_uri2.size() > 2)   //not enooght for a break condition!
        {
            string_view sub1Char = sub_uri2.substr(0, 1);  //not needed
            string_view sub2Char = sub_uri2.substr(0, 2);
            string_view sub3Char = sub_uri2.substr(0, 3);

            if (sub2Char == "./") { sub_uri2 = sub_uri2.substr(2); }
            else if (sub3Char == "../")
            {
                dotDotSlashCase(sub_uri1, sub_uri2);
            }
            else if (sub2Char == "//")
            {
                sub_uri2 = sub_uri2.substr(2);
                size_t doubleslash = sub_uri1.find("//") + 2;
                string_view before = sub_uri1;
                before = before.substr(0, doubleslash);
                if (!before.empty()) sub_uri1 = before;
                else sub_uri1 = "";
                break;
            }
            else if (sub1Char == "/")
            {
                slashCase(sub_uri1, sub_uri2);
                break;
            }
            // stupied cases when starts with ////////////////// for uri1 and 2
            //else if (sub1Char == " ") { uri2 = "%20" + uri2.substr(1); }  //future fixes for URI address
            else break; //no other cases awailable
        }




        if (!sub_uri2.empty() && sub_uri2.size() == 2 && sub_uri2.substr(0, 2) == "..")
            dotDotSlashCase(sub_uri1, sub_uri2);
        else if (!sub_uri2.empty() && sub_uri2.size() == 1 && sub_uri2.substr(0, 1) == ".")
            sub_uri2 = sub_uri2.substr(1);

        bool slash = false;
        if (!sub_uri1.empty() && !sub_uri2.empty() && sub_uri1.back() != '/' && sub_uri2.front() != '/') slash = true;

        string combined_uri
        {
            string(sub_uri1) + (slash ? "/" : "") + string(sub_uri2)
        };
        

        return combined_uri;
    }

}



void dotDotSlashCase(string_view& uri1, string_view& uri2)
{
    uri2 = uri2.size() > 2 ? uri2.substr(3) : "";
    size_t slash = uri1.find_last_of("/");
    size_t doubleslash = uri1.find("//") + 1;   //or :// +2
    string_view before = uri1.substr(0, slash);
    string_view after = uri1.substr(slash + 1);
    //bool isAnAddress
    if (!before.empty() && before != after && doubleslash != slash)  uri1 = before; //uri1 = uri1.substr(0, slash)
}



void slashCase(string_view& uri1, string_view& uri2)
{
    uri2 = uri2.substr(1);
    size_t doubleslash = uri1.find("//") + 1;

    size_t slash = uri1.find_last_of("/");
    string_view before = uri1.substr(0, slash);
    string_view after = uri1.substr(slash + 1);;

    while (!before.empty() && before != after && doubleslash != slash)
    {
        uri1 = before;
        slash = uri1.find_last_of("/");
        before = uri1.substr(0, slash);
        after = uri1.substr(slash + 1);
    }
}

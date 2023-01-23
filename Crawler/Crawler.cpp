// CppDevelopment.cpp : This file contains the 'main' function. Program execution 
// begins and ends there.
//

//https://github.com/nlohmann/json


#include <AddressCombiner.h>
#include <HtmlRequest.h>

#define DEBUG_MODE 1

#include <iostream>
#include <fstream>
#include <string> 
#include <exception>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <functional>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>

#include <gumbo-query/gumbo.h>  // included
#include "Classes/Image.h"
//using namespace rapidjson;
#include <nlohmann/json.hpp>    //included
using json = nlohmann::json;



using std::string;
using std::ifstream;
using std::exception;
using std::vector;
using std::ws;
using std::cout;
using std::endl;
using std::cin;
using std::hash;
using std::map;
using std::pair;
using std::function;
using std::set;
using std::list;
using std::queue;
using std::unordered_map;
using std::thread;
using std::mutex;
using std::chrono::duration;
using std::chrono::seconds;

//#define combiner AddressCombiner::combiner;

//using namespace std;


mutex visitedUri_m;
set<string> visitedUri;

mutex levels_m;
unordered_map<size_t, list<string>> levels;


mutex visitedImageUri_m;
set<string> visitedImageUri;
list<Image> images;             //to delete
json json_images;


static const void search_for_links(GumboNode* node, const string& uri, const size_t& level);

void crawler(const string& uri, const size_t level = 1) //run on thread
{
    visitedUri_m.lock();
    if (visitedUri.find(uri) != visitedUri.end()) 
    {
        visitedUri_m.unlock(); 
        return; 
    }
    else
    {
        visitedUri.insert(uri);
        visitedUri_m.unlock();
    }
    string contents = getHtml(uri);
    GumboOutput* output = gumbo_parse(contents.c_str());
    search_for_links(output->root, uri, level);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}


static const void search_for_links(GumboNode* node, const string& uri, const size_t& level)
{
    if (node->type != GUMBO_NODE_ELEMENT) return;
    if
        (
            GumboAttribute* href;
            node->v.element.tag == GUMBO_TAG_A
            && (href = gumbo_get_attribute(&node->v.element.attributes, "href"))
            )
    {
        levels_m.lock();
       // levels.emplace(level + 1, href->value);
        string hrefValue{ combiner(uri, static_cast<string>(href->value)) };
        levels[level + 1].push_back(hrefValue);
        levels_m.unlock();
    }
    else if
        (
            GumboAttribute* src;
            node->v.element.tag == GUMBO_TAG_IMG
            && (src = gumbo_get_attribute(&node->v.element.attributes, "src"))
            )
    {
        visitedImageUri_m.lock();
        if
            (
                string srcValue{ combiner(uri, static_cast<string>(src->value)) };
                !(visitedImageUri.find(srcValue) != visitedImageUri.end())
                )
        {
            visitedImageUri.insert(srcValue);
            images.push_back(Image(srcValue, uri, level));
            json_images["results"].emplace_back(Image(srcValue, uri, level).to_json());
        }
        visitedImageUri_m.unlock();
    }
    GumboVector* children = &node->v.element.children;  //moving through all the nodes in HTML document
    ////vector<thread> threadList;
    ////if (children->length > 0) threadList.reserve(children->length);
    for (size_t i{ 0 }; i < children->length; ++i)
    {
        auto childNode
            { static_cast<GumboNode*>(children->data[i]) };
        ///threadList.push_back(thread(search_for_links, childNode, uri, level));
        search_for_links(childNode, uri, level);
    }

    ////for (auto& th : threadList) th.join();

}




int main(int argc, char* argv[])
{

    //if (argc != 2)
    //{
    //    std::cout << "Please provide a valid English word" << std::endl;
    //    exit(EXIT_FAILURE);
    //}
    //if(argc > 0 || isCliProduction){} else{}


    cout << "WebCrawler Cpp:" << endl;
    size_t limitLevels{ 3 };

    string uri1{ "https://arthurcam.com" };

    levels[1].push_back(uri1);


    for (size_t i{ 1 }; i <= limitLevels; ++i)
    {
        vector<thread> threadGlobalList;
        if (levels.find(i) != levels.end() && !levels.at(i).empty()) threadGlobalList.reserve(levels[i].size());
        else break; //no elements on i level
        for(auto& levelEl: levels[i])
            threadGlobalList.push_back(thread(crawler, levelEl, i));
        for (auto&t : threadGlobalList)
            t.join();
    }




    cout << "\n\n\nImages HERE:\n";

    //json j_list;
    //j_list["results"] = {};
    for (const auto& el : images)
    {
        // j_list["results"].emplace_back(el.to_json());
        cout << el.print();
    }

    std::ofstream MyFile("result.json");
    MyFile << json_images;
    MyFile.close();

    cout << "\n\n\n";

    auto a = json_images.dump();
    cout << a << endl << endl << endl;
    //cout << "\n\n\nUri HERE:\n";
    //for (auto el : levels)
    //{
    //    for (const auto& listEl: el.second) 
    //        cout << listEl << endl;
    //}

    //vector<Person> persons;
    //string buffer{ "" };
    //int personCounter{ 0 };

    //string fileName = "mock-data.json";
    //ifstream inFile;
    //inFile.open(fileName);
    //if (!inFile.is_open()) throw exception("cant read the file");
    //while (inFile) 
    //{
    //    string line;
    //    getline(inFile >> ws, line);
    //    buffer += line;
    //}

    //inFile.close();

    //auto jsonItems = json::parse(buffer);

    //for (auto& el : jsonItems.items()) {   /*array list*/
    //    auto key = el.key();
    //    ++personCounter;
    //    auto poupleValue = el.value();
    //    Person p;
    //    for (auto& [subkey, subval] : poupleValue.items()) 
    //        p.setValue(subkey, subval);
    //    persons.push_back(p);
    //}



#if DEBUG_MODE == 1
    std::cout << "\n\nDebugging mode\n";
    //for (auto p : persons)
    //    cout << p << "\n";
   // cin.get();  //debug  
#endif // DEBUG
}








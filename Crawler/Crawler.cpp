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

#include <functional>
#include <algorithm>

#include <gumbo-query/gumbo.h>  // https://github.com/google/gumbo-parser
#include "Classes/Image.h"

//using namespace rapidjson;
#include <nlohmann/json.hpp>    //include
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
using std::unordered_map;




//using namespace std;

list<Image> images;
json json_images;
set<string> visitedUri;
set<string> visitedImageUri;
unordered_map<size_t, list<string>> levels;


static const void search_for_links(GumboNode* node, const string& uri, const size_t& level);

void crawler(const string& uri, const size_t level = 1) //run on thread
{
    //mutex1.lock
    if (visitedUri.find(uri) != visitedUri.end()) return;
    visitedUri.insert(uri);
    //mutex1.unlock
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
        //mutex2.lock
       // levels.emplace(level + 1, href->value);
        string hrefValue{ combiner(uri, static_cast<string>(href->value)) };
        levels[level + 1].push_back(hrefValue);
        //mutex2.unlock
    }
    else if
        (
            GumboAttribute* src;
            node->v.element.tag == GUMBO_TAG_IMG
            && (src = gumbo_get_attribute(&node->v.element.attributes, "src"))
            )
    {
        //mutex3.lock
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
        //mutex3.unlock
    }
    GumboVector* children = &node->v.element.children;  //moving through all the nodes in HTML document
    for (size_t i{ 0 }; i < children->length; ++i)
    {
        auto childNode
        {
            static_cast<GumboNode*>(children->data[i])
        };
        search_for_links(childNode, uri, level);
    }
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
    size_t limitLevels = 2;

    string uri1{ "https://arthurcam.com" };

    levels[1].push_back(uri1);
    //auto f = levels[1].front();
    //crawler(f, 1); levels[1].pop_front();


    for (size_t i{ 1 }; i <= limitLevels; ++i)
    {
        while (levels.find(i) != levels.end() && !levels.at(i).empty())
        {
            //run multithreading
            auto front = levels[i].front();
            levels[i].pop_front();
            crawler(front, i);
        }
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








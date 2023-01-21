// CppDevelopment.cpp : This file contains the 'main' function. Program execution 
// begins and ends there.
//

//https://github.com/nlohmann/json

#define DEBUG_MODE 1
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <libxml/HTMLparser.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
//#include "gumbo-query/Document.h"
//#include "gumbo-query/Node.h"


//#include <libxml++/libxml++.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <string> 
#include <fstream>
#include <exception>
//#include <nlohmann/json.hpp>
//#include <Person.h>

#include <functional>
#include <algorithm>
#include <curl/curl.h>      //installed with vcpkg  //also here https://github.com/curl/curl.git
//#include <gumbo.h>          //installed with vcpkg  //also here https://github.com/google/gumbo-parser.git
#include <gumbo-query/gumbo.h>  // https://github.com/google/gumbo-parser

//using namespace rapidjson;


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
using std::unordered_map;
//using json = nlohmann::json;
//using namespace std;


typedef size_t(*curl_write)(char*, size_t, size_t, std::string*);

std::string getHtml(string url) {
    CURLcode res_code = CURLE_FAILED_INIT;
    CURL* curl = curl_easy_init();
    string result;
    curl_global_init(CURL_GLOBAL_ALL);

    if (curl) {
        curl_easy_setopt(
            curl, CURLOPT_WRITEFUNCTION
            , static_cast <curl_write> (
                [](
                    char* contents, size_t size, size_t nmemb
                    , std::string* data) -> size_t 
                    {
                        size_t new_size = size * nmemb;
                        if (data == NULL) return 0; //or error handling
                        data->append(contents, new_size);
                        return new_size;
                    }
            )
        );
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "simple scraper");

        res_code = curl_easy_perform(curl);

        if (res_code != CURLE_OK) return curl_easy_strerror(res_code);  //or error handling
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return result;
}






static std::string cleantext(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
    }
    else if (node->type == GUMBO_NODE_ELEMENT &&
        node->v.element.tag != GUMBO_TAG_SCRIPT &&
        node->v.element.tag != GUMBO_TAG_STYLE) {
        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = cleantext((GumboNode*)children->data[i]);
            if (i != 0 && !text.empty()) {
                contents.append(" ");
            }
            contents.append(text);
        }
        return contents;
    }
    else {
        return "";
    }
}



set<string> s1;
set<string> s2;
static const void search_for_links(GumboNode* node) 
{
    if (node->type != GUMBO_NODE_ELEMENT) return;

    if (
        GumboAttribute* href;
        node->v.element.tag == GUMBO_TAG_A
        && (href = gumbo_get_attribute(&node->v.element.attributes, "href"))
        )
    {
        s1.insert(href->value);
    }
    else if (
        GumboAttribute* src;
        node->v.element.tag == GUMBO_TAG_IMG
        && (src = gumbo_get_attribute(&node->v.element.attributes, "src"))
        )
    {
        s2.insert(src->value);
    }


    GumboVector* children = &node->v.element.children;  //moving through all the nodes in HTML document
    for (unsigned int i = 0; i < children->length; ++i) {
        auto childNode = static_cast<GumboNode*>(children->data[i]);
        search_for_links(childNode);
    }
}




int main(int argc, char* argv[])
{

    //if (argc != 2)
    //{
    //    std::cout << "Please provide a valid English word" << std::endl;
    //    exit(EXIT_FAILURE);
    //}

    cout << "WebCrawler Cpp, Word Dictionary show" << endl;
    //if(argc > 0 || isCliProduction){} else{}

    string word = "esoteric";
    //string html = getHtml("https://www.merriam-webster.com/dictionary/" + word); //https://arthurcam.com





    std::string contents = getHtml("https://arthurcam.com");
    GumboOutput* output = gumbo_parse(contents.c_str());
    search_for_links(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    cout << "\n\n\nHERE:\n";
    for (const auto& el : ::s2) { cout << el << '\n'; }


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

// CppDevelopment.cpp : This file contains the 'main' function. Program execution 
// begins and ends there.
//

//https://github.com/nlohmann/json


#include <AddressCombiner.h>
#include <HtmlRequest.h>

#define DEBUG_MODE 1

#include <iostream>
#include <string> 
#include <exception>
#include <vector>
#include <set>
#include <map>

#include <functional>
#include <algorithm>

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

void crawler(const string& uri) 
{
    std::string contents = getHtml(uri);
    GumboOutput* output = gumbo_parse(contents.c_str());
    search_for_links(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

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


    string uri1 = "https://arthurcam.com";
    crawler(uri1);
    cout << "\n\n\nHERE:\n";
    for (auto el : ::s2) { cout << combiner(uri1, el) << '\n'; }


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








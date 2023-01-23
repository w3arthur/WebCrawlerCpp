#pragma once

#include <AddressCombiner.h>
#include <HtmlRequest.h>
#include "Image.h"

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


class CrawlerRun
{
    //string begin_address;
    //size_t crawler_levels;
private:
    mutex visitedUri_m;
public:
    set<string> visitedUri;

private:
    mutex levels_m;
public:
    unordered_map<size_t, list<string>> levels;

private:
    mutex visitedImageUri_m;
public:
    set<string> visitedImageUri;
    list<Image> images;             //to delete
    json json_images;

public:
    void search_for_links(GumboNode* node, const string& uri, const size_t& level);
    void crawler(const string& uri, size_t level); //run on thread
public:
    explicit CrawlerRun(const string& begin_address, size_t crawler_levels);
    void print();
    void write_to_file(const string& file_address_name);
    string to_string() const;

   
};



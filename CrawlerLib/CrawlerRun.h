#pragma once

#include "Image.h"  //#include

//#include <set>
//#include <gumbo-query/gumbo.h>  // included

class CrawlerRun
{
    //string begin_address;
    //size_t crawler_levels;
private:
    std::mutex visitedUri_m;
public:
    std::set<std::string> visitedUri;

private:
    std::mutex levels_m;
public:
    std::unordered_map<size_t, std::list<std::string>> levels;

private:
    std::mutex visitedImageUri_m;
public:
    std::set<std::string> visitedImageUri;
    std::list<Image> images;             //to delete
    nlohmann::json json_images;

public:
    void search_for_links(GumboNode* node, const std::string& uri, const size_t& level);
    void crawler(const std::string& uri, size_t level); //run on thread
public:
    explicit CrawlerRun(const std::string& begin_address, size_t crawler_levels);
    void print();
    void write_to_file(const std::string& file_address_name);
    std::string to_string() const;

};



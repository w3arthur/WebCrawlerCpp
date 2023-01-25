#pragma once

#include "Image.h"  //#include
#include "ICrawlerRun.h"
//#include <set>
//#include <gumbo-query/gumbo.h>  // included

class CrawlerRun : public ICrawlerRun
{
    //string begin_address;
    //size_t crawler_levels;
private:
    std::mutex visitedUri_m;
    std::set<std::string> visitedUri;

private:
    std::mutex levels_m;
    std::unordered_map<size_t, std::list<std::string>> levels;

private:
    std::mutex visitedImageUri_m;
    std::set<std::string> visitedImageUri;
    std::list<Image> images;             //to delete
    nlohmann::json json_images;

public: //set as protected!, rty to delete virtual
    string get_html(const string& uri);
    void search_for_links(GumboNode* node, const std::string& uri, const size_t& level) ;
    void crawler(const std::string& uri, size_t level); //run on thread
public:
    explicit CrawlerRun(const std::string& begin_address, size_t crawler_levels);
    explicit CrawlerRun() = default;
    void print() const;
    void write_to_file(const std::string& file_address_name) const;
    std::string to_string() const;

};



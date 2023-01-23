#include "CrawlerRun.h"




    //constructor
CrawlerRun::CrawlerRun(const string& begin_address, size_t crawler_levels)
{
    levels[1].push_back(begin_address);
    for (size_t i{ 1 }; i <= crawler_levels; ++i)
    {
        vector<thread> threadGlobalList;
        if (levels.find(i) != levels.end() && !levels.at(i).empty()) threadGlobalList.reserve(levels[i].size());
        else break; //no elements on i level
        for (auto& address : levels[i])
        {
            //crawler(address, i);
            threadGlobalList.push_back(thread(&CrawlerRun::crawler, this, address, i));   /////
        }
        for (auto& t : threadGlobalList)
            t.join();
    }
}

    //public
void CrawlerRun::print()
{
    //json j_list;
    //j_list["results"] = {};
    for (const auto& el : images)
    {
        // j_list["results"].emplace_back(el.to_json());
        cout << el.print();
    }
}

void CrawlerRun::write_to_file(const string& file_address_name)
{
    std::ofstream MyFile(file_address_name);
    MyFile << json_images;
    MyFile.close();
}

string CrawlerRun::to_string() const
{
    auto images_j = json_images;
    auto images_s = images_j.dump();
    return images_s;
}


    //private

void CrawlerRun::crawler(const string& uri, size_t level)
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


void CrawlerRun::search_for_links(GumboNode* node, const string& uri, const size_t& level)
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

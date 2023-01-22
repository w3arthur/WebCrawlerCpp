#pragma once
#include <string>
#include <map>
using std::string;
using std::to_string;


#include <nlohmann/json.hpp>    //include
using json = nlohmann::json;



class Image
{
public:
	string imageUrl;
	string sourceUrl;
	size_t depth;
	explicit Image(string imageUrl, string sourceUrl, size_t depth) : imageUrl{ imageUrl }, sourceUrl{ sourceUrl }, depth{ depth } {}


	json to_json() const
	{
		json j;
		j["imageUrl"] = imageUrl;
		j["sourceUrl"] = sourceUrl;
		j["depth"] = depth;
		
		return j;
	}


	string print() const
	{
		return "imageUrl: " + imageUrl + " sourceUrl: " + sourceUrl + " depth: " + to_string(depth) + "\n";
	}
	//json
	//friend ...
};


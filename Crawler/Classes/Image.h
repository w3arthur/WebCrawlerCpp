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
		return "imageUrl: " + (imageUrl.length() > 35 ? imageUrl.substr(0, 25) + "..." + imageUrl.substr(imageUrl.length() -10 ) : imageUrl)
			+ " sourceUrl: " + (sourceUrl.length() > 35 ? sourceUrl.substr(0, 25) + "..." + sourceUrl.substr(sourceUrl.length() - 10 ) : sourceUrl)
			+ " depth: " + to_string(depth) + "\n";
	}
	//json
	//friend ...
};


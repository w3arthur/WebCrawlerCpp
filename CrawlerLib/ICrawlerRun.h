#pragma once


class ICrawlerRun	//set to interface
{
public:
	virtual string get_html(const string& uri) { return ""; };
	virtual void search_for_links(GumboNode* node, const string& uri, const size_t& level) {};
	virtual void crawler(const string& uri, size_t level) {};
	virtual void print() const {};
	virtual void write_to_file(const string& file_address_name) const
	{};
	virtual string to_string() const 
	{
		return "";
	};
};

#pragma once
#pragma once

#include <CrawlerRun.h> //#include <IHtmlRequest.h>


class MyCrawlerRun : public CrawlerRun	//fake object too
{
public:	//all protected method set to public:
	void setHtmlRequest(IHtmlRequest* html_request)
	{
		CrawlerRun::setHtmlRequest(html_request);
	}
public:
	void init(const std::string& begin_address, size_t crawler_levels)
	{
		CrawlerRun::init(begin_address, crawler_levels);
	}

	//MOCK_METHOD(void, init, (const std::string, size_t));
//	void search_for_links(GumboNode* node, const std::string& uri, const size_t& level)
//	void crawler(const std::string& uri, size_t level)

//public:			//Delete premenatly
//	virtual string html_get(const string& uri) const
//	{
//		return CrawlerRun::html_get(uri);
//	}
};

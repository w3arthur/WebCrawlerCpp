#pragma once

#include "MyCrawlerRun.h" //#include <IHtmlRequest.h>


class MockCrawlerRun : public MyCrawlerRun	//fake object too
{
public:
	MOCK_METHOD(void, setHtmlRequest, (std::shared_ptr<IHtmlRequest>));
	MOCK_METHOD(void, init, (const std::string, size_t));
	MOCK_METHOD(void, search_for_links, (GumboNode*, const std::string&, const size_t&));
	MOCK_METHOD(void, crawler, (const std::string, size_t));
	MOCK_METHOD(string, html_get, (const std::string));
};

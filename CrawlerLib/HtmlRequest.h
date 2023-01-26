#pragma once
#include "IHtmlRequest.h"
class HtmlRequest : public IHtmlRequest
{
public:
	std::string getHtml(std::string url);
};




#pragma once
class IHtmlRequest
{
public:
	virtual std::string getHtml(std::string url) = 0;
};


#pragma once
#include <IHtmlRequest.h>
class MockHtmlRequest : public IHtmlRequest
{
public:
	MOCK_METHOD1(getHtml, std::string(const std::string uri));

};

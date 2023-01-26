#pragma once

#include "MyCrawlerRun.h"//#include <CrawlerRun.h> //#include <IHtmlRequest.h>
#include "Utils.h"


class CrawlerRunMockTest : public ::testing::Test
{
private:
	MyCrawlerRun* mock_cr;
	IHtmlRequest* mockhtml;

private:
	class MockHtmlRequest : public IHtmlRequest
	{
	public:
		MOCK_METHOD1(getHtml, std::string(const std::string uri));
	};

public:
	void CrawlerRunMockTest::SetUp()
	{
		mock_cr = new MyCrawlerRun();
		mockhtml = new MockHtmlRequest();
		mock_cr->setHtmlRequest(mockhtml);
	}

	void CrawlerRunMockTest::TearDown()
	{
		delete mock_cr;
	} // delete mockhtml will done inside mock_cr

public:
	MockHtmlRequest& getMockHtmlRequest() { return dynamic_cast<MockHtmlRequest&>(*mockhtml); }

public:
	void initMock(size_t levels);

	string printMock();

	void writeMockToFile(const string& file_address_name);


public:	//mock basic data
	const string mock_string{ R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
				<img src="image1.jpg">
			</body></html>
		)V0G0N" };

	const string mock_address{ "http://someaddress.com/folder1/" };

	const string mock_result{ R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"}]})" };

	const string mockFileName{ "test_case.json" };

};

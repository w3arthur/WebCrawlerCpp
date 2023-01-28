#pragma once

//https://github.com/google/googletest/blob/main/docs/gmock_cook_book.md#knowing-when-to-expect


	//::testing::NiceMock<MockHtmlRequest> mockhtml2; //try test with NiceMock

#include "MyCrawlerRun.h"//#include <CrawlerRun.h> //#include <IHtmlRequest.h>
#include "Utils.h"


class CrawlerRunMockTest : public ::testing::Test
{

private:
	class MockHtmlRequest : public IHtmlRequest
	{
	public:
		MOCK_METHOD(std::string, getHtml, (const std::string));//, (const)
	};

private:
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
	//	virtual string html_get(const string& uri) const//Delete premenatly
	};

public:
	MyCrawlerRun* mock_cr;
	IHtmlRequest* mockhtml;


public:
	MockHtmlRequest& getMockHtmlRequest()
	{
		//auto& sptr_mockHtmlRequest = std::dynamic_pointer_cast<MockHtmlRequest>(mockhtml);
		//auto* p_mockHtmlRequest = sptr_mockHtmlRequest.get();
		return dynamic_cast<MockHtmlRequest&>(*mockhtml);
	}
	MyCrawlerRun& getMyCrawlerRun()
	{
		return *mock_cr;
	}



public:
	void SetUp()
	{
		mock_cr = new MyCrawlerRun();
		mockhtml = new MockHtmlRequest();
		mock_cr->setHtmlRequest(mockhtml);
	}

	void TearDown()
	{
		delete mock_cr;
	} // delete mockhtml will done inside mock_cr




public:	//class methods
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

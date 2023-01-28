#pragma once

//https://github.com/google/googletest/blob/main/docs/gmock_cook_book.md#knowing-when-to-expect


	//::testing::NiceMock<MockHtmlRequest> mockhtml2; //try test with NiceMock

#include "MyCrawlerRun.h"//#include <CrawlerRun.h> //#include <IHtmlRequest.h>
#include "Utils.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::Return;

class CrawlerRunMockTest : public ::testing::Test
{

private:
	class MockHtmlRequest : public IHtmlRequest
	{
	public:
		MOCK_METHOD(std::string, getHtml, (const std::string));//, (const)
	};

protected:
	class MockCrawlerRun : public CrawlerRun
	{
	public:
		MOCK_METHOD(void, search_inside_element, (GumboNode* node, const std::string& uri, const size_t& level));
		MOCK_METHOD(void, crawler, (const std::string& uri, size_t level));
		MOCK_METHOD(string, to_string, ());
	};


private:
	class MyCrawlerRun : public MockCrawlerRun	//fake object too
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

	public:
		void search_inside_element(GumboNode* node, const std::string& uri, const size_t& level)
		{
			MockCrawlerRun::search_inside_element(node, uri, level);	// only count
			CrawlerRun::search_inside_element(node, uri, level);
		}
		void crawler(const std::string& uri, size_t level)
		{
			MockCrawlerRun::crawler(uri, level);	// only count
			CrawlerRun::crawler(uri, level);
		}
		string to_string()
		{
			string str = MockCrawlerRun::to_string();	// only count
			return !str.empty() ? str : CrawlerRun::to_string();
		}
	};

public:
	MyCrawlerRun* mock_cr;
	IHtmlRequest* mockhtml;


public:
	MockHtmlRequest& getMockHtmlRequest()
	{	//auto& sptr_mockHtmlRequest = std::dynamic_pointer_cast<MockHtmlRequest>(mockhtml);
		//auto* p_mockHtmlRequest = sptr_mockHtmlRequest.get();
		return dynamic_cast<MockHtmlRequest&>(*mockhtml);
	}
	MyCrawlerRun& getMyCrawlerRun()
	{
		return dynamic_cast<MyCrawlerRun&>(*mock_cr);
	}



public:
	void SetUp()
	{
		mock_cr = new MyCrawlerRun();
		mockhtml = new MockHtmlRequest();
		mock_cr->setHtmlRequest(mockhtml);
		EXPECT_CALL(getMyCrawlerRun(), crawler(_, _)).Times(AtLeast(1));
		EXPECT_CALL(getMyCrawlerRun(), search_inside_element(_, _, _))
			.Times(AtLeast(at_laset_mock_string_levels));
		EXPECT_CALL(getMyCrawlerRun(), to_string()).Times(AtMost(1));
	}

	void TearDown()
	{
		delete mock_cr;
	} // delete mockhtml done inside mock_cr




public:	//class methods
	void initMock(size_t levels);

	string printMock();

	void writeMockToFile(const string& file_address_name);




public:	//mock basic data
	const size_t at_laset_mock_string_levels = 3;

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

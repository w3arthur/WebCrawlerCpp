#pragma once

//https://github.com/google/googletest/blob/main/docs/gmock_cook_book.md#knowing-when-to-expect


	//::testing::NiceMock<MockHtmlRequest> mockhtml2; //try test with NiceMock

#include "MyCrawlerRun.h"//#include <CrawlerRun.h> //#include <IHtmlRequest.h>
#include "Utils.h"


class CrawlerRunMockTest : public ::testing::Test
{
public:
	MyCrawlerRun* mock_cr;
	std::shared_ptr<IHtmlRequest> mockhtml;
	
private:
	class MockHtmlRequest : public IHtmlRequest
	{
	public:
		MOCK_METHOD(std::string, getHtml, (const std::string));//, (const)
		//MOCK_METHOD1(getHtml, std::string(const std::string uri));
	};



public:
	MockHtmlRequest& getMockHtmlRequest() 
	{
		auto& sptr_mockHtmlRequest = std::dynamic_pointer_cast<MockHtmlRequest>(mockhtml);
		MockHtmlRequest* p_mockHtmlRequest = sptr_mockHtmlRequest.get();
		return *p_mockHtmlRequest;
	}

public:
	void SetUp()
	{
		mock_cr = new MyCrawlerRun();
		mockhtml = std::make_shared<MockHtmlRequest>();
		mock_cr->setHtmlRequest(mockhtml);
		
	}

	void TearDown()
	{
		delete mock_cr;
		
	} // delete mockhtml will done inside mock_cr



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

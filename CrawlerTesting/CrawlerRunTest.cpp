#include "pch.h"
//#include <ICrawlerRun.h>
#include <CrawlerRun.h>
#include <IHtmlRequest.h>
#include "Utils.h"

//https://stackoverflow.com/questions/60486110/how-to-use-googlemock-in-visual-studio
//#include "gmock/gmock.h"
#pragma warning(disable:4996)

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::DoDefault;
using ::testing::DoAll;



class MockHtmlRequest : public IHtmlRequest
{
public:
	MOCK_METHOD1(getHtml, std::string(const std::string uri));

};


//class MyCrawlerRun : public ICrawlerRun		//all methods should be public
//{
//	ICrawlerRun* cr;
//public:	//all protected method set to public:
//	MyCrawlerRun() { cr = new CrawlerRun(); }
//	~MyCrawlerRun() { delete cr; }
//	//void setHtmlRequest(IHtmlRequest* html_request)
//	//{
//	//	cr->setHtmlRequest(html_request);
//	///.....
//};




class MyCrawlerRun : public CrawlerRun	//fake one
{
public:	//all protected method set to public:
	void setHtmlRequest(IHtmlRequest* html_request)
	{
		CrawlerRun::setHtmlRequest(html_request);
	}
	void init(const std::string& begin_address, size_t crawler_levels)
	{
		CrawlerRun::init(begin_address, crawler_levels);
	}
	void search_for_links(GumboNode* node, const std::string& uri, const size_t& level)
	{
		CrawlerRun::search_for_links(node, uri, level);
	}
	void crawler(const std::string& uri, size_t level)
	{
		CrawlerRun::crawler(uri, level);
	}

};



class CrawlerRunMockTest : public ::testing::Test
{
private:
	MyCrawlerRun* mock_cr;
	IHtmlRequest* mockhtml;

public:
	void SetUp()
	{
		mock_cr = new MyCrawlerRun();
		mockhtml = new MockHtmlRequest();
		mock_cr->setHtmlRequest(mockhtml);
	}
	void TearDown() { delete mock_cr; } // delete mockhtml will done inside mock_cr

public:
	MockHtmlRequest& getMockHtmlRequest() { return dynamic_cast<MockHtmlRequest&>(*mockhtml); }

public:
	const string mock_string{ R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
				<img src="image1.jpg">
			</body></html>
		)V0G0N" };
	
	const string mock_address{ "http://someaddress.com/folder1/" };

	const string mock_result{ R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"}]})" };

	const string mockFileName{ "test_case.json" };

public:
	void initMock(size_t levels) { mock_cr->init(mock_address, levels); }

	string printMock() { return mock_cr->to_string(); }

	void writeMockToFile(const string& file_address_name) { empti_the_file(file_address_name); mock_cr->write_to_file(file_address_name); }

};


TEST_F(CrawlerRunMockTest, EnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	ON_CALL(getMockHtmlRequest(), getHtml(_)).WillByDefault(Return(mock_string));
	initMock(1);	//levels 1
	auto assume{ printMock() };
	auto result{ mock_result };
	EXPECT_EQ(assume, result);
}



TEST_F(CrawlerRunMockTest, EnteredRegularHtmlPageWithFourImageLevel1Level2_ExpectToReturnRightJsonString)
{
	const auto mock_string_level1 =
		R"V0G0N(
		<!doctype><html><head></head><body>
			<h1> Title 1 <h1>
			<img src="image1.jpg">
			<img src="./image2.jpg">
			<img src="../image3.jpg">
			<a href="./link_level2">click1</a>
		</body></html>
	)V0G0N";

	const auto mock_string_level2 =
		R"V0G0N(
		<!doctype><html><head></head><body>
			<h1> Title 2 <h1>
			<img src="image4.jpg">
		</body></html>
	)V0G0N";

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(AtLeast(1))
		.WillOnce(Return(mock_string_level1))
		.WillOnce(Return(mock_string_level2));

	initMock(2); //levels 2

	auto assume = printMock();

	string result = R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/folder1/image2.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/image3.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":2,"imageUrl":"http://someaddress.com/folder1/link_level2/image4.jpg","sourceUrl":"http://someaddress.com/folder1/link_level2"}]})";

	//also starts with {
	EXPECT_EQ(assume, result);
}



TEST_F(CrawlerRunMockTest, WriteReadFromFileEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonStringFromTheFile)
{
	ON_CALL(getMockHtmlRequest(), getHtml(_)).WillByDefault(Return(mock_string));
	initMock(1);	//levels 1
	writeMockToFile(mockFileName);

	auto assume{ read_from_file(mockFileName) };
	string result{ mock_result };

	//also starts with {
	EXPECT_EQ(assume, result);

	empti_the_file(mockFileName);//empti the file
}




TEST_F(CrawlerRunMockTest, RunningTimeDurationIsLessFrom100MilliSecEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	auto startTime{ std::chrono::high_resolution_clock::now() };

	ON_CALL(getMockHtmlRequest(), getHtml(_)).WillByDefault(Return(mock_string));

	initMock( 1);	//levels 1
	printMock();	//just for running
	writeMockToFile(mockFileName);
	
	auto stopTime{ std::chrono::high_resolution_clock::now()};

	auto duration{ std::chrono::duration_cast<std::chrono::milliseconds> (stopTime - startTime) };


	long long assumeMilliSec{ 100 };
	auto resultMilliSec{ duration.count() };

	//also starts with {
	EXPECT_GE(assumeMilliSec, resultMilliSec);
	empti_the_file(mockFileName);//empti the file
}





TEST_F(CrawlerRunMockTest, EnteredRegularHtmlPageNoImageOnlyLevel1_ExpectToReturnRightJsonString) //to fix
{
	const auto mock_string_no_image =
		R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
			</body></html>
		)V0G0N";

	ON_CALL(getMockHtmlRequest(), getHtml(_)).WillByDefault(Return(mock_string_no_image));
	initMock(1);//levels 1
	auto assume = printMock();
	string result = R"(null)";	//to fix and get results":[]}

	EXPECT_EQ(assume, result);
}

#include "pch.h"
//#include "gmock/gmock.h" //https://stackoverflow.com/questions/60486110/how-to-use-googlemock-in-visual-studio
#include "CrawlerRunMockTest.h"	
//#include "MockHtmlRequest" #include "MyCrawlerRun.h" #include <ICrawlerRun.h>  #include <CrawlerRun.h> 
//#include "Utils.h"




#pragma warning(disable:4996)

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::DoDefault;
using ::testing::DoAll;

extern class CrawlerRunMockTest;



#include "MockCrawlerRun.h"



TEST(MockCrawlerExam, A1)
{

	MockCrawlerRun cr;

	EXPECT_CALL(cr, html_get(_)).Times(AtLeast(1))
		.WillOnce(Return("<img src=\"ssss.jpg\" >"));

	EXPECT_CALL(cr, init(_, _)).Times(AtLeast(1));

	cr.init("http://www.site.com", 1);	//levels 1

	auto assume{ cr.to_string() };

	auto result{ "{result:[{}]}" };
	EXPECT_EQ(assume, result);
}





TEST_F(CrawlerRunMockTest, DISABLED_EnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string));
	initMock(1);	//levels 1
	auto assume{ printMock() };
	auto result{ mock_result };
	EXPECT_EQ(assume, result);
}



TEST_F(CrawlerRunMockTest, DISABLED_EnteredRegularHtmlPageWithFourImageLevel1Level2_ExpectToReturnRightJsonString)
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

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(2)	//AtLeast(1)
		.WillOnce(Return(mock_string_level1))
		.WillOnce(Return(mock_string_level2));

	initMock(2); //levels 2

	auto assume{ printMock() };

	string result = R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/folder1/image2.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/image3.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":2,"imageUrl":"http://someaddress.com/folder1/link_level2/image4.jpg","sourceUrl":"http://someaddress.com/folder1/link_level2"}]})";

	EXPECT_EQ(assume, result);
}



TEST_F(CrawlerRunMockTest, DISABLED_WriteReadFromFileEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonStringFromTheFile)
{
	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string));
	initMock(1);	//levels 1
	writeMockToFile(mockFileName);

	auto assume{ read_from_file(mockFileName) };
	string result{ mock_result };


	EXPECT_EQ(assume, result);

}




TEST_F(CrawlerRunMockTest, DISABLED_RunningTimeDurationIsLessFrom200MilliSecEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	auto startTime{ std::chrono::high_resolution_clock::now() };

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string));

	initMock( 1);	//levels 1
	printMock();	//just for running
	writeMockToFile(mockFileName);
	
	auto stopTime{ std::chrono::high_resolution_clock::now()};

	auto duration{ std::chrono::duration_cast<std::chrono::milliseconds> (stopTime - startTime) };

	long long assumeMilliSec{ 200 };
	auto resultMilliSec{ duration.count() };

	EXPECT_GE(assumeMilliSec, resultMilliSec);
	

	
}



TEST_F(CrawlerRunMockTest, DISABLED_EnteredRegularHtmlPageNoImageOnlyLevel1_ExpectToReturnRightJsonString) //to fix
{
	const auto mock_string_no_image =
		R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
			</body></html>
		)V0G0N";

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string_no_image));	
	initMock(1);//levels 1
	auto assume = printMock();
	string result = R"(null)";	//to fix and get results":[]}
	EXPECT_EQ(assume, result);
}
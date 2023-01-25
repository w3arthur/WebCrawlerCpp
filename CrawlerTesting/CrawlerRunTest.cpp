#include "pch.h"
//#include <ICrawlerRun.h>
#include <CrawlerRun.h>

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



class FakeCrawlerRun
{

};

//
//class IICrawlerRun: ICrawlerRun	//set to interface
//{
//public:
//	virtual string get_html(const string& uri) const { return ""; };
//	virtual void search_for_links(GumboNode* node, const string& uri, const size_t& level){};
//	virtual void crawler(const string& uri, size_t level) {};
//	virtual void print() const {};
//	virtual void write_to_file(const string& file_address_name) const {};
//	virtual string to_string() const {};
//};
//

class MyCrawlerRun : public CrawlerRun
{

public:	//protected methods from class CrawlerRun
	MyCrawlerRun () = default;
	void init(const std::string begin_address, size_t crawler_levels) 
	{
		CrawlerRun::init(begin_address, crawler_levels);
	}
	std::string get_html(const std::string& uri)
	{
		return CrawlerRun::get_html(uri);
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



class MockCrawlerRun : public MyCrawlerRun //, ICrawlerRun
{
public:
	MOCK_METHOD1(get_html, string (const string& uri));
};

#include <string>

TEST(CrawlerRunMockTest, EnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	MockCrawlerRun mock_cr;

	const auto mock_string =
		R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
				<img src="image1.jpg">
			</body></html>
		)V0G0N";

	ON_CALL(mock_cr, get_html(_)).WillByDefault(Return(mock_string));

	mock_cr.init("", 1);

	auto assume = mock_cr.to_string();
	string result = R"({"results":[{"depth":1,"imageUrl":"image1.jpg","sourceUrl":""}]})";
	
	//also starts with {
	EXPECT_EQ(assume, result);
}








TEST(CrawlerRunMockTest, EnteredRegularHtmlPageWithFourImageLevel1Level2_ExpectToReturnRightJsonString)
{
	MockCrawlerRun mock_cr;

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

	EXPECT_CALL(mock_cr, get_html(_)).Times(AtLeast(1)).WillOnce(Return(mock_string_level1)).WillOnce(Return(mock_string_level2));

	mock_cr.init("http://someaddress.com/folder1/", 2);

	auto assume = mock_cr.to_string();
	string result = R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/folder1/image2.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/image3.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":2,"imageUrl":"http://someaddress.com/folder1/link_level2/image4.jpg","sourceUrl":"http://someaddress.com/folder1/link_level2"}]})";

	//also starts with {
	EXPECT_EQ(assume, result);
}






TEST(CrawlerRunMockTest, EnteredRegularHtmlPageNoImageOnlyLevel1_ExpectToReturnRightJsonString) //to fix
{
	MockCrawlerRun mock_cr;

	const auto mock_string =
		R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
			</body></html>
		)V0G0N";

	ON_CALL(mock_cr, get_html(_)).WillByDefault(Return(mock_string));

	mock_cr.init("", 1);

	auto assume = mock_cr.to_string();
	string result = R"(null)";	//to fix and get results":[]}

	EXPECT_EQ(assume, result);
}

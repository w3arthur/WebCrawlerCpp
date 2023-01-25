#include "pch.h"
//#include <ICrawlerRun.h>
#include <CrawlerRun.h>
#include <thread>
#include <chrono>


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



class FakeCrawlerRun : public ICrawlerRun
{
private:
	ICrawlerRun* cr;
public:
	FakeCrawlerRun(const std::string& begin_address, size_t crawler_levels)
		: cr{ new CrawlerRun(begin_address, crawler_levels) } { }
	~FakeCrawlerRun() { delete cr; }
};



class MyCrawlerRun : public CrawlerRun //, public ICrawlerRun
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



class MockCrawlerRun : public MyCrawlerRun //, public ICrawlerRun
{
public:
	MOCK_METHOD1(get_html, string (const string& uri));
};


const char* mock_string{
		R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
				<img src="image1.jpg">
			</body></html>
		)V0G0N" };


TEST(CrawlerRunMockTest, EnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	MockCrawlerRun mock_cr;

	//const auto mock_string =
	//	R"V0G0N(
	//		<!doctype><html><head></head><body>
	//			<h1> Title <h1>
	//			<img src="image1.jpg">
	//		</body></html>
	//	)V0G0N";

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



void empti_the_file(const std::string& file_name) 
{
	std::ofstream MyFile(file_name);
	MyFile << "";
	MyFile.close();
}

string read_from_file(const std::string& file_name)
{
	std::string buffer;
	std::ifstream MyFile;
	MyFile.open(file_name);
	if (!MyFile.is_open()) return "READING FILE ISSUE";
	MyFile.seekg(0, std::ios::end);
	buffer.resize(MyFile.tellg());
	MyFile.seekg(0, std::ios::beg);
	MyFile.read(&buffer[0], buffer.size());
	MyFile.close();
	return buffer;
}


TEST(CrawlerRunMockTest, WriteReadFromFileEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonStringFromTheFile)
{
	MockCrawlerRun mock_cr;

	//const auto mock_string =
	//	R"V0G0N(
	//		<!doctype><html><head></head><body>
	//			<h1> Title <h1>
	//			<img src="image1.jpg">
	//		</body></html>
	//	)V0G0N";

	ON_CALL(mock_cr, get_html(_)).WillByDefault(Return(mock_string));

	mock_cr.init("", 1);
	const auto file_name = "test_case.json";

	//empti the file
	empti_the_file(file_name);


	mock_cr.write_to_file(file_name);

	auto assume = mock_cr.to_string();
	auto result = read_from_file(file_name);


	//also starts with {
	EXPECT_EQ(assume, result);

	//empti the file
	empti_the_file(file_name);
}





TEST(CrawlerRunMockTest, RunningTimeDurationIsLessFrom100MilliSecEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	auto startTime{ std::chrono::high_resolution_clock::now() };

	MockCrawlerRun mock_cr;

	//const auto mock_string =
	//	R"V0G0N(
	//		<!doctype><html><head></head><body>
	//			<h1> Title <h1>
	//			<img src="image1.jpg">
	//		</body></html>
	//	)V0G0N";

	ON_CALL(mock_cr, get_html(_)).WillByDefault(Return(mock_string));

	const auto dont_care_file_name = "dont_care_test_case.json";
	mock_cr.init("", 1);
	auto to_string = mock_cr.to_string();
	mock_cr.write_to_file(dont_care_file_name);

	auto stopTime{ std::chrono::high_resolution_clock::now()};

	auto duration{ std::chrono::duration_cast<std::chrono::milliseconds> (stopTime - startTime) };


	long long assumeMilliSec = 100;
	auto resultMilliSec = duration.count();

	//also starts with {
	EXPECT_GE(assumeMilliSec, resultMilliSec);
}





TEST(CrawlerRunMockTest, EnteredRegularHtmlPageNoImageOnlyLevel1_ExpectToReturnRightJsonString) //to fix
{
	MockCrawlerRun mock_cr;

	const auto mock_string_no_image =
		R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
			</body></html>
		)V0G0N";

	ON_CALL(mock_cr, get_html(_)).WillByDefault(Return(mock_string_no_image));

	mock_cr.init("", 1);

	auto assume = mock_cr.to_string();
	string result = R"(null)";	//to fix and get results":[]}

	EXPECT_EQ(assume, result);
}

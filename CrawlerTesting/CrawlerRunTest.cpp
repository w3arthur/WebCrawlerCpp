#include "pch.h"
//#include <ICrawlerRun.h>
#include <CrawlerRun.h>


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


	//will not use
//class FakeCrawlerRun : public CrawlerRun
//{
//private:
//	ICrawlerRun* cr;
//public:
//	FakeCrawlerRun(): cr{ new CrawlerRun() } { }
//	~FakeCrawlerRun() { delete cr; }
//};



class MyCrawlerRun : public CrawlerRun //, public ICrawlerRun
{
//	ICrawlerRun* cr;
public:	//protected methods from class CrawlerRun
	MyCrawlerRun() = default;
	void init(const std::string& begin_address, size_t crawler_levels) 
	{
		CrawlerRun::init(begin_address, crawler_levels);
	}
	std::string get_html(const std::string& uri)
	{
		return "<Will Override Inside Mock>";
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



class MockCrawlerRun : public MyCrawlerRun
{
public:
	MOCK_METHOD1(get_html, std::string (const std::string& uri));
	//MOCK_METHOD(std::string, get_html, (const std::string& uri), (override)); // ot(const, override)
};



class CrawlerRunMockTest: public ::testing::Test
{
public:
	MockCrawlerRun mock_cr;

	const string mock_string{ R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
				<img src="image1.jpg">
			</body></html>
		)V0G0N" };
	const string mock_address{ "http://someaddress.com/folder1/" };
	const string mock_result{ R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"}]})" };
	const string mockFileName{ "test_case.json" };
	
	void initMock(size_t levels) { mock_cr.init(mock_address, levels); }
	string printMock() { return mock_cr.to_string(); }
	void writeMockToFile(const string& file_address_name) { empti_the_file(file_address_name); mock_cr.write_to_file(file_address_name); }
	MockCrawlerRun& getMockCrawlerRun() { return mock_cr; }
	void SetUp() {}
	void TearDown() { }
};


TEST_F(CrawlerRunMockTest, EnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{

	ON_CALL(getMockCrawlerRun(), get_html(_)).WillByDefault(Return(mock_string));
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

	EXPECT_CALL(getMockCrawlerRun(), get_html(_)).Times(AtLeast(1))
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
	ON_CALL(mock_cr, get_html(_)).WillByDefault(Return(mock_string));
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

	ON_CALL(getMockCrawlerRun(), get_html(_)).WillByDefault(Return(mock_string));

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

	ON_CALL(getMockCrawlerRun(), get_html(_)).WillByDefault(Return(mock_string_no_image));
	initMock(1);//levels 1
	auto assume = printMock();
	string result = R"(null)";	//to fix and get results":[]}

	EXPECT_EQ(assume, result);
}

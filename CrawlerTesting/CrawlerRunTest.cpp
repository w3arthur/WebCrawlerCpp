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

//class MyCrawlerRun
//{
//private:
//	ICrawlerRun& cr;
//public:
//	MyCrawlerRun(ICrawlerRun& cr) : cr{ cr } {}	//ctr
//	string get_html(string str) {
//		return cr.get_html(str); 
//	}
//	void print() {
//
//		cr.print();
//	}
//};
//


class MockCrawlerRun : public CrawlerRun //, ICrawlerRun
{
public:
	//MockCrawlerRun() = default;
	// 0 means no argoments
	//MOCK_METHOD0(print, void () );	// 0 means no argoments
	//MOCK_METHOD0(to_string, void ());
	MOCK_METHOD1(get_html, string (const string& uri));
	//MOCK_METHOD1(write_to_file, void (const string& file_address_name));
	///MOCK_METHOD2(crawler, void (const std::string& uri, size_t level));
	//MOCK_METHOD3(search_for_links, void (GumboNode* node, const std::string& uri, const size_t& level));
};



TEST(TrueTest243534534, NoNameMethodTest_EnteredTrueScenario_ExpectedGetTrue234534) 
{

	MockCrawlerRun mock_cr;
	//MyCrawlerRun my_cr(mock_cr);

	ON_CALL(mock_cr, get_html("")).WillByDefault(Return("46456"));


	/*
	
	Return(
	R"V0G0N(
<!doctype><html><head></head><body>
	<h1> Title <h1>
	<img src="image1.jpg">
	<img src="./image2.jpg">
	<img src="../image2.jpg">
	<a href="./link1">click1</a>
</body></html>
			)V0G0N"
	)
	
	*/
	auto assume = mock_cr.get_html("");
	string result = "46456";

	mock_cr.print();



	EXPECT_EQ(assume, result);
}
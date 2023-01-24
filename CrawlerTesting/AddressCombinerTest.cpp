#include "pch.h"
using ::testing::AddGlobalTestEnvironment;
using ::testing::Bool;
using ::testing::Combine;
using ::testing::Message;
using ::testing::Range;
using ::testing::TestWithParam;
using ::testing::Values;
using ::testing::ValuesIn;


//#include "AddressCombiner.h"

//what to do about www or redirections ?
//	issue //
// issue /
// improve string to string_view for better performance


class Data
{
public:
	std::string primaryAddress, secondaryAddress, expectedResult;
};


struct AddressCombinerTestResplacingBySecondaryAddressTest : ::testing::Test
{
	const std::string& primaryAddress = "http://www.site1.com:1234/folder1/";
	const std::string& secondaryAddress = "http://www.site2.com/folder2/";
	Data* d;
	void SetUp() { d = new Data{ primaryAddress, secondaryAddress, secondaryAddress }; }
	void TearDown() { delete d; }
	// another way
	//ResplacingBySecondaryAddressTest() { account = new Data{/*...*/}; }
	//~ResplacingBySecondaryAddressTest() { delete d; }
};



TEST_F(AddressCombinerTestResplacingBySecondaryAddressTest, EnteredSecondaryFullPathAddress_ExpectedReturnTheSecondaryAddress)
{	
	const std::string& result = d->expectedResult;
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, result);
}


TEST(AddressCombinerTest, EnteredPrimaryWithSlashSecondaryWithSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "/folder2/";
	const std::string& result = "http://www.site1.com:1234/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}




TEST(AddressCombinerTest, EnteredPrimaryWithSlashSecondaryNoSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}


TEST(AddressCombinerTest, EnteredPrimaryNoSlashSecondaryNoSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, EnteredPrimaryNoSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "./folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, EnteredPrimaryWithSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "./folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}



TEST(AddressCombinerTest, InputTrimTestForBothAddresses_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "        http://www.site1.com:1234/folder1//////////////               ";
	std::string secondaryAddress = "                 folder2////////////         ";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, InputPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressWithFolderBack) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "../folder2/";
	const std::string& result = "http://www.site1.com:1234/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}


TEST(AddressCombinerTest, InputPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressNoFolderBack) {
	std::string primaryAddress = "http://www.site1.com:1234/";
	std::string secondaryAddress = "../folder2/";
	const std::string& result = "http://www.site1.com:1234/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}


TEST(AddressCombinerTest, EnteredPrimaryNoSlashSecondaryWithSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "/folder2/";			//WRONG !!!
	const std::string& result = "http://www.site1.com:1234/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, EnteredPrimaryNoSlashSecondaryWithDoubleSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "//site2.com/folder2/";
	const std::string& result = "http://site2.com/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}





class CombineAddressTest : public ::testing::TestWithParam<Data> {
public:
	string assume;
	string result;
	CombineAddressTest() {
		auto as = GetParam();
		assume = combiner(as.primaryAddress, as.secondaryAddress);
		result = as.expectedResult;
	}
	~CombineAddressTest() {}

};

TEST_P(CombineAddressTest, ExternalGenerator) {
	//auto as = GetParam();
	//const std::string& assume = combiner(as.primaryAddress, as.secondaryAddress);
	//const std::string& result = as.expectedResult;
	EXPECT_EQ(assume, result);
}

INSTANTIATE_TEST_CASE_P(AllUnExpectedValueTests, CombineAddressTest, ::testing::Values(
	Data{
		"http://www.site1.com:1234/folder1//////////////" , "folder2////////////"
		, "http://www.site1.com:1234/folder1/folder2" 
		}
	, Data{ "http://www.site1.com:1234/","./","http://www.site1.com:1234" }
	, Data{ "http://www.site1.com:1234/folder1/","./","http://www.site1.com:1234/folder1" }
	, Data{ "http://www.site1.com:1234/","../","http://www.site1.com:1234" }
	, Data{ "http://www.site1.com:1234/","//","http://www.site1.com:1234" }
	, Data{ "http://www.site1.com:1234/folder1/","../","http://www.site1.com:1234" }	//to fix
), );



//disabling tes https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test
TEST(AddressCombinerTest, DISABLED_PrivateCaseForP) {	//DISABLED_
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "../";
	const std::string& result = "http://www.site1.com:1234";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}
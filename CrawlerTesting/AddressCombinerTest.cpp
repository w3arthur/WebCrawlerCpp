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


class AddressCombinerTestClass: public ::testing::Test
{
	const std::string& primaryAddress = "http://www.site1.com:1234/folder1/";
	const std::string& secondaryAddress = "http://www.site2.com/folder2/";
	
public:
	Data* d;
	const string& getExpectedResult() const { return d->expectedResult; }
	void setExpectedResult(const string& str) { d->expectedResult = str; }
	void setD(const Data& data) { d = new Data(data); }

	void SetUp() {}
	void TearDown() { delete d; }
	// another way
	//ResplacingBySecondaryAddressTest() { account = new Data{/*...*/}; }
	//~ResplacingBySecondaryAddressTest() { delete d; }
};



TEST_F(AddressCombinerTestClass, EnteredSecondaryFullPathAddress_ExpectedReturnTheSecondaryAddress)
{	
	setD({ "http://www.site1.com:1234/folder1/", "http://www.site2.com/folder2/" });
	setExpectedResult(d->secondaryAddress);
	const auto& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryWithSlashSecondaryWithSlash_ExpectedCombineRighBothAddress) 
{
	setD({ "http://www.site1.com:1234/folder1/", "/folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryWithSlashSecondaryNoSlash_ExpectedCombineRighBothAddress)
{
	setD({ "http://www.site1.com:1234/folder1/", "folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryNoSlash_ExpectedCombineRighBothAddress)
{
	setD({ "http://www.site1.com:1234/folder1", "folder2/"  });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}

TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress)
{
	setD({ "http://www.site1.com:1234/folder1", "./folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}

TEST_F(AddressCombinerTestClass, EnteredPrimaryWithSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress)
{
	setD({ "http://www.site1.com:1234/folder1/", "./folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}

TEST_F(AddressCombinerTestClass, InputTrimTestForBothAddresses_ExpectedCombineRighBothAddress)
{
	setD({
		"        http://www.site1.com:1234/folder1//////////////               "
		, "                 folder2////////////         " 
		});
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}

TEST_F(AddressCombinerTestClass, InputPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressWithFolderBack)
{
	setD({"http://www.site1.com:1234/folder1/", "../folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}


TEST_F(AddressCombinerTestClass, InputPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressNoFolderBack)
{
	setD({ "http://www.site1.com:1234/", "../folder2/"  });
	setExpectedResult("http://www.site1.com:1234/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryWithSlash_ExpectedCombineRighBothAddress)
{
	setD({ "http://www.site1.com:1234/folder1", "/folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}

TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryWithDoubleSlash_ExpectedCombineRighBothAddress)
{
	setD({ "http://www.site1.com:1234/folder1", "//site2.com/folder2/" });
	setExpectedResult("http://site2.com/folder2");
	const std::string& assume = combiner(d->primaryAddress, d->secondaryAddress);
	EXPECT_EQ(assume, getExpectedResult());
}





class CombineAddressTest : public ::testing::TestWithParam<Data> 
{
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

TEST_P(CombineAddressTest, ExternalGenerator)
{
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
TEST(AddressCombinerTest, DISABLED_PrivateCaseForP) 
{	//DISABLED_
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "../";
	const std::string& result = "http://www.site1.com:1234";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}
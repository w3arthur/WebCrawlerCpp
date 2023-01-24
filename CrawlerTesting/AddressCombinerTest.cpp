#include "pch.h"

//#include "AddressCombiner.h"

//what to do about www or redirections ?
//	issue //
// issue /
// improve string to string_view for better performance

TEST(AddressCombinerTest, EnteredSecondaryFullPathAddress_ExpectedReturnTheSecondaryAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "http://www.site2.com/folder2/";
	const std::string& result = secondaryAddress;
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
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



//disabling tes https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test


TEST(AddressCombinerTest, AddressCombiner_TrimTestForBothAddresses_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "        http://www.site1.com:1234/folder1//////////////               ";
	std::string secondaryAddress = "                 folder2////////////         ";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, EnteredPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressWithFolderBack) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "../folder2/";
	const std::string& result = "http://www.site1.com:1234/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}


TEST(AddressCombinerTest, EnteredPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressNoFolderBack) {
	std::string primaryAddress = "http://www.site1.com:1234/";
	std::string secondaryAddress = "../folder2/";
	const std::string& result = "http://www.site1.com:1234/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, EnteredSecondaryWithDoubleDotSlashOnly_ExpectedNoChangingAdrees) {
	std::string primaryAddress = "http://www.site1.com:1234/";
	std::string secondaryAddress = "../";
	const std::string& result = "http://www.site1.com:1234";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, EnteredSecondaryWithDotSlashOnly_ExpectedNoChangingAdrees) {
	std::string primaryAddress = "http://www.site1.com:1234/";
	std::string secondaryAddress = "./";
	const std::string& result = "http://www.site1.com:1234";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, EnteredPrimaryAndSecondaryWithExtraSlashesAfter_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1//////////////";
	std::string secondaryAddress = "folder2////////////";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}





////to fix !!!
TEST(AddressCombinerTest, EnteredPrimaryNoSlashSecondaryWithSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "/folder2/";			//WRONG !!!
	const std::string& result = "http://www.site1.com:1234/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

//to fix !!!
TEST(AddressCombinerTest, EnteredPrimaryNoSlashSecondaryWithDoubleSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "//site2.com/folder2/";
	const std::string& result = "http://site2.com/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}


#include "pch.h"

//#include "AddressCombiner.h"

//what to do about www or redirections ?
//	issue //
// issue /
// improve string to string_view for better performance

TEST(AddressCombinerTest, AddressCombiner_EnteredSecondaryFullPathAddress_ExpectedReturnTheSecondaryAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "http://www.site2.com/folder2/";
	const std::string& result = secondaryAddress;
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryWithSlashSecondaryWithSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "/folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

////to fix !!!
//TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryNoSlashSecondaryWithSlash_ExpectedCombineRighBothAddress) {
//	std::string primaryAddress = "http://www.site1.com:1234/folder1";
//	std::string secondaryAddress = "/folder2/";			//WRONG !!!
//	const std::string& result = "http://www.site1.com:1234/folder2";
//	const std::string& assume = combiner(primaryAddress, secondaryAddress);
//	EXPECT_EQ(assume, result);
//}
//
////to fix !!!
//TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryNoSlashSecondaryWithDoubleSlash_ExpectedCombineRighBothAddress) {
//	std::string primaryAddress = "http://www.site1.com:1234/folder1";
//	std::string secondaryAddress = "//site2.com/folder2/";			//WRONG !!!
//	const std::string& result = "http://site2.com/folder2";
//	const std::string& assume = combiner(primaryAddress, secondaryAddress);
//	EXPECT_EQ(assume, result);
//}

TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryWithSlashSecondaryNoSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}


TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryNoSlashSecondaryNoSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryNoSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "./folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}

TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryWithSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
	std::string secondaryAddress = "./folder2/";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}
//
////to fix !!!
//TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressWithFolderBack) {
//	std::string primaryAddress = "http://www.site1.com:1234/folder1/";
//	std::string secondaryAddress = "../folder2/";
//	const std::string& result = "http://www.site1.com:1234/folder2";
//	const std::string& assume = combiner(primaryAddress, secondaryAddress);
//	EXPECT_EQ(assume, result);
//}
//
////to fix !!!
//TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressNoFolderBack) {
//	std::string primaryAddress = "http://www.site1.com:1234/";
//	std::string secondaryAddress = "../folder2/";
//	const std::string& result = "http://www.site1.com:1234/folder2";
//	const std::string& assume = combiner(primaryAddress, secondaryAddress);
//	EXPECT_EQ(assume, result);
//}



TEST(AddressCombinerTest, AddressCombiner_EnteredPrimaryAndSecondaryWithExtraSlashesAfter_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "http://www.site1.com:1234/folder1//////////////";
	std::string secondaryAddress = "folder2////////////";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}


TEST(AddressCombinerTest, AddressCombiner_TrimTestForBothAddresses_ExpectedCombineRighBothAddress) {
	std::string primaryAddress = "        http://www.site1.com:1234/folder1//////////////               ";
	std::string secondaryAddress = "                 folder2////////////         ";
	const std::string& result = "http://www.site1.com:1234/folder1/folder2";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}
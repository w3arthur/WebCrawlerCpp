#include "pch.h"
#include "CrawlerRunMockTest.h"


void CrawlerRunMockTest::initMock(size_t levels)
{
	mock_cr->init(mock_address, levels);
}

string CrawlerRunMockTest::printMock()
{
	return mock_cr->to_string();
}

void CrawlerRunMockTest::writeMockToFile(const string& file_address_name)
{
	empti_the_file(file_address_name);
	mock_cr->write_to_file(file_address_name);
}

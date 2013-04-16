#ifndef ContainsTest_h
#define ContainsTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ContainsTest : public CPPUNIT_NS::TestFixture
{ 
	CPPUNIT_TEST_SUITE( ContainsTest );
	//CPPUNIT_TEST(testGetIsContainsTest);
	//CPPUNIT_TEST(testGetContainsTest);
	//CPPUNIT_TEST(testGetContainsStarTest);
	CPPUNIT_TEST(testGetIsContainsStarTest);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testGetContainsTest();
	void testGetIsContainsTest();
	void testGetContainsStarTest();
	void testGetIsContainsStarTest();
};
#endif
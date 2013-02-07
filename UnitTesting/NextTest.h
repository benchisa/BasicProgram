
#ifndef NextTest_h
#define NextTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class NextTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
	CPPUNIT_TEST_SUITE( NextTest ); // Note 3 
	CPPUNIT_TEST( testIsNext);
	CPPUNIT_TEST( testGetNext);
	CPPUNIT_TEST_SUITE_END();


	public:
	void setUp();
	void tearDown();
	void testIsNext();
	void testGetNext();
	std::string src;
};

#endif
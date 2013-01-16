#ifndef QueryPKBTest_h
#define QueryPKBTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class QueryPKBTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
	CPPUNIT_TEST_SUITE( QueryPKBTest ); // Note 3 
	CPPUNIT_TEST( testSampleInput1);
	CPPUNIT_TEST( testSampleInput2);
	CPPUNIT_TEST_SUITE_END();


public:
	void setUp();
	void tearDown();
	
	void testSampleInput1();
	void testSampleInput2();
};

#endif
#ifndef ParserTest_h
#define ParserTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParserTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
	CPPUNIT_TEST_SUITE( ParserTest ); // Note 3 
	CPPUNIT_TEST( testCorrectSource );
	CPPUNIT_TEST( testGetLastError);
	CPPUNIT_TEST( testGetPKB);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// methods to test source correctness and error msg
	void testCorrectSource();
	void testGetLastError();
	void testGetPKB();

	// methods to test parser's logic
};
#endif
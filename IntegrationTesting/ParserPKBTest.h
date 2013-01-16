#ifndef ParserPKBTest_h
#define ParserPKBTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParserPKBTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
	CPPUNIT_TEST_SUITE( ParserPKBTest ); // Note 3 
	CPPUNIT_TEST( testSampleInput1);
	CPPUNIT_TEST( testSampleInput2);
	CPPUNIT_TEST( testSampleInput3);
	CPPUNIT_TEST( testSampleInput4);
	CPPUNIT_TEST( testInvalidInput1);
	CPPUNIT_TEST( testInvalidInput2);
	CPPUNIT_TEST_SUITE_END();


public:
	void setUp();
	void tearDown();

	void testSampleInput1();
	void testSampleInput2();
	void testSampleInput3();
	void testSampleInput4();

	void testInvalidInput1();
	void testInvalidInput2();
	void testInvalidInput3();
	void testInvalidInput4();
	void testInvalidInput5();
};

#endif
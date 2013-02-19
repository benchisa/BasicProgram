#ifndef ParserTest_h
#define ParserTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParserTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
	CPPUNIT_TEST_SUITE( ParserTest ); // Note 3 
	CPPUNIT_TEST(testComplex);
	CPPUNIT_TEST( testCorrectSource );
	CPPUNIT_TEST( testGetLastError);
	CPPUNIT_TEST( testGetPKB);
	CPPUNIT_TEST(testOperators);
	CPPUNIT_TEST(testMultipleProcedures);
	CPPUNIT_TEST(testIf);
	CPPUNIT_TEST(testCall);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// methods to test source correctness and error msg
	void testComplex();
	void testCorrectSource();
	void testGetLastError();
	void testGetPKB();
	void testOperators();
	void testMultipleProcedures();
	void testIf();
	void testCall();
	// methods to test parser's logic
};
#endif
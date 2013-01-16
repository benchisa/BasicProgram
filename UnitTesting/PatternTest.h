
#ifndef PatternTest_h
#define PatternTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class PatternTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( PatternTest ); // Note 3 
CPPUNIT_TEST( testGetPatternStmt);
CPPUNIT_TEST( testIsPattern);
CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();

void testGetPatternStmt();
void testIsPattern();
};

#endif
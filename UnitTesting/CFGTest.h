
#ifndef CFGTest_h
#define CFGTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class CFGTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( CFGTest ); // Note 3 
CPPUNIT_TEST( testCFG);
CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();
void testCFG();
};

#endif

#ifndef PKBTest_h
#define PKBTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class PKBTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( PKBTest ); // Note 3 
CPPUNIT_TEST( testGetASTBy);
CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testGetASTBy();
};

#endif
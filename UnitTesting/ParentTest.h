#ifndef ParentTest_h
#define ParentTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParentTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( ParentTest ); // Note 3 
CPPUNIT_TEST( testInsertParent );
CPPUNIT_TEST(testIsParent);
CPPUNIT_TEST(testGetParent);
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testInsertParent();
void testIsParent();
void testGetParent();

};
#endif
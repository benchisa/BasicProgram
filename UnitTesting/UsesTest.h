#ifndef UsesTest_h
#define UsesTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class UsesTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( UsesTest ); // Note 3 
CPPUNIT_TEST( testInsertUses );
CPPUNIT_TEST(testIsUses);
CPPUNIT_TEST(testGetUses);
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testInsertUses();
void testIsUses();
void testGetUses();

};
#endif
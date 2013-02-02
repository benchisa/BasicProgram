#ifndef CallTableTest_h
#define CallTableTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class CallTableTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( CallTableTest ); // Note 3 
CPPUNIT_TEST( testInsertCall );
CPPUNIT_TEST(testIsCallExist);
CPPUNIT_TEST(testGetCall);
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testInsertCall();
void testIsCallExist();
void testGetCall();

};
#endif

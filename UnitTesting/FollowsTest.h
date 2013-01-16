#ifndef FollowsTest_h
#define FollowsTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class FollowsTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( FollowsTest ); // Note 3 
CPPUNIT_TEST( testInsertFollows );
CPPUNIT_TEST(testIsFollows);
CPPUNIT_TEST(testGetFollows);
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testInsertFollows();
void testIsFollows();
void testGetFollows();

};
#endif
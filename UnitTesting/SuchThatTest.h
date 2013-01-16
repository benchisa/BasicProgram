#ifndef SuchThatTest_h
#define SuchThatTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class SuchThatTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( SuchThatTest ); // Note 3 
CPPUNIT_TEST( testGetParentResult );
CPPUNIT_TEST( testGetParentStarResult );
CPPUNIT_TEST(testGetIsParentResult);
CPPUNIT_TEST(testGetFollowsResult);
CPPUNIT_TEST(testGetFollowsStarResult);
CPPUNIT_TEST(testGetIsFollowsResult);
CPPUNIT_TEST(testGetModifiesResult);
CPPUNIT_TEST(testGetIsModifiesResult);
CPPUNIT_TEST(testGetUsesResult);
CPPUNIT_TEST(testGetIsUsesResult);
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

void testGetParentResult();
void testGetParentStarResult();
void testGetFollowsResult();
void testGetFollowsStarResult();
void testGetIsParentResult();
void testGetIsFollowsResult();
void testGetModifiesResult();
void testGetIsModifiesResult();
void testGetUsesResult();
void testGetIsUsesResult();

};
#endif
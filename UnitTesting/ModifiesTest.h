#ifndef ModifiesTest_h
#define ModifiesTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ModifiesTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( ModifiesTest ); // Note 3 
CPPUNIT_TEST( testInsertModifies );
CPPUNIT_TEST(testIsModifies);
CPPUNIT_TEST(testGetModifies);
CPPUNIT_TEST_SUITE_END();

public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testInsertModifies();
void testIsModifies();
void testGetModifies();

};
#endif
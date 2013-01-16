
#ifndef ASTTest_h
#define ASTTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ASTTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( ASTTest ); // Note 3 
CPPUNIT_TEST( testConstructor);
CPPUNIT_TEST( testSetFirstDescendant);
CPPUNIT_TEST( testSetAncestor);
CPPUNIT_TEST( testAddSibling);
CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testConstructor();
void testSetFirstDescendant();
void testSetAncestor();
void testAddSibling();
};

#endif

#ifndef VarTableTest_h
#define VarTableTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class VarTableTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( VarTableTest ); // Note 3 
CPPUNIT_TEST( testInsertVar );
CPPUNIT_TEST( testGetVarTableSize);
CPPUNIT_TEST( testGetVarName);
CPPUNIT_TEST( testGetVarIndex);
CPPUNIT_TEST( testIsExists);
CPPUNIT_TEST( testGetAllVar);
CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testInsertVar();
void testGetVarTableSize();
void testGetVarName();
void testGetVarIndex();
void testIsExists();
void testGetAllVar();
};

#endif
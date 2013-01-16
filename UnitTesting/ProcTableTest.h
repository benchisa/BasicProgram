
#ifndef ProcTableTest_h
#define ProcTableTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ProcTableTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( ProcTableTest ); // Note 3 
CPPUNIT_TEST( testInsertProc);
CPPUNIT_TEST( testGetProcTableSize);
CPPUNIT_TEST( testGetProcedure);
CPPUNIT_TEST( testGetProcIndex);
CPPUNIT_TEST( testIsExists);
CPPUNIT_TEST( testIsInSameProc);
CPPUNIT_TEST( testGetAllProc);

CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();

// method to test the assigning and retrieval of grades
void testInsertProc();
void testGetProcTableSize();
void testGetProcedure();
void testGetProcIndex();
void testIsExists();
void testIsInSameProc();
void testGetAllProc();

};

#endif
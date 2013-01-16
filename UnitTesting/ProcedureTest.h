
#ifndef ProcedureTest_h
#define ProcedureTest_h
// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ProcedureTest : public CPPUNIT_NS::TestFixture // Note 2 
{ 
CPPUNIT_TEST_SUITE( ProcedureTest ); // Note 3 
CPPUNIT_TEST( testSetEndProgLine);
CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();

void testSetEndProgLine();
};

#endif
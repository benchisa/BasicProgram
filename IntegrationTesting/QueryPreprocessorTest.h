
#ifndef QueryPreprocessorTest_h
#define QueryPreprocessorTest_h

#include <cppunit/extensions/HelperMacros.h>
#include "../SPA/QueryPreprocessor.h"
#include "../SPA/PKB.h"

class QueryPreprocessorTest : public CPPUNIT_NS::TestFixture  
{ 
CPPUNIT_TEST_SUITE( QueryPreprocessorTest ); 
CPPUNIT_TEST( testCorrectPreProcessing );
CPPUNIT_TEST_SUITE_END();


public:
void setUp();
void tearDown();

void testCorrectPreProcessing();

QueryPreprocessor *qpp;

};

#endif
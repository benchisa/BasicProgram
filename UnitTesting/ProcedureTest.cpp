#include "ProcedureTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Procedure.h>

void ProcedureTest::setUp()
{
}


void ProcedureTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ProcedureTest ); // Note 4 

void ProcedureTest::testSetEndProgLine(){
	Procedure myProcedure("John",1);
	myProcedure.setEndProgLine(30);
	CPPUNIT_ASSERT(myProcedure.getProcName()=="John");
	CPPUNIT_ASSERT_EQUAL(1,myProcedure.getStartProgLine());
	CPPUNIT_ASSERT_EQUAL(30,myProcedure.getEndProgLine());
}


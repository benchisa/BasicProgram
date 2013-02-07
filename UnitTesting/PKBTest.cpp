#include <cppunit/config/SourcePrefix.h>
#include <../SPA/PKB.h>
#include "PKBTest.h"

void PKBTest::setUp()
{
}


void PKBTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PKBTest ); // Note 4 
PKB *myPKB;

// method to test the assigning and retrieval of grades
void PKBTest::testGetASTBy(){
	myPKB = new PKB();
	AST *AST1;
	AST *AST2;
	AST1 = myPKB->createAST(PROCEDURE,1,1,-1);
	AST2 = myPKB->createAST(WHILE,1,2,-1);

	//myPKB.setRootAST(AST1);
	AST_LIST* myList;
	myList = myPKB->getASTBy(2);
	
	AST_LIST::iterator itr;
	itr = myList->begin();
	CPPUNIT_ASSERT((*itr)->getRootType()==WHILE);
}

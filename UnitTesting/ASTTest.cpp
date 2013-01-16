#include "ASTTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/AST.h>

void ASTTest::setUp()
{
}


void ASTTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ASTTest ); // Note 4 


// method to test the assigning and retrieval of grades
void ASTTest::testConstructor(){
	//create a new procedure
	AST myAST(PROCEDURE,1,1);
	CPPUNIT_ASSERT(myAST.getRootType()==PROCEDURE);
	CPPUNIT_ASSERT_EQUAL(1,myAST.getRootData());
	CPPUNIT_ASSERT_EQUAL(1,myAST.getRootStatementNum());
}
void ASTTest::testSetFirstDescendant(){
	
	//create a new AST
	AST myAST1(PROCEDURE,1,1);
	AST myAST2(STMT_LIST,2,-1);

	//add first descendant
	AST *ast2Pointer;
	ast2Pointer = new AST(myAST2);
	bool tf=myAST1.setFirstDescendant(ast2Pointer);
	CPPUNIT_ASSERT(tf==true);

	//attempt to add the first descendant, when it already exits
	AST myAST3(ASSIGNMENT,3,-1);
	AST *ast3Pointer;
	ast3Pointer = new AST(myAST3);
	CPPUNIT_ASSERT(myAST1.setFirstDescendant(ast3Pointer)==false);
	CPPUNIT_ASSERT(myAST1.getFirstDescendant()==ast2Pointer);
	
}
void ASTTest::testSetAncestor(){
	AST myAST1(PROCEDURE,1,1);
	AST myAST2(STMT_LIST,2,-1);

	myAST2.setAncestor(&myAST1);
	CPPUNIT_ASSERT(myAST2.getAncestor()==&myAST1);
	
}
void ASTTest::testAddSibling(){
	AST myAST1(WHILE,3,-1);
	AST myAST2(ASSIGNMENT,4,-1);

	myAST1.addSibling(&myAST2);
	CPPUNIT_ASSERT(myAST1.getRightSibling()==&myAST2);
	CPPUNIT_ASSERT(myAST2.getLeftSibling()==&myAST1);
}
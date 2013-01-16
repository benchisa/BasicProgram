#include "PatternTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Pattern.h>

void PatternTest::setUp()
{
}


void PatternTest::tearDown()
{
}

// Registers the fixture into the 'registry'
//CPPUNIT_TEST_SUITE_REGISTRATION( PatternTest ); // Note 4 

void PatternTest::testIsPattern(){
	/*PKB pkb;
	//1. x=y
	pkb.insertModifies(ASSIGNMENT,1,2);
	pkb.insertUses(ASSIGNMENT,1,3);

	//create root AST
	AST* rootAST;
	AST* firstD;

	rootAST = pkb.createAST(ASSIGNMENT,1,-1);
	pkb.setRootAST(rootAST);
	//create nodes for x(2) and y(3)
	
	firstD = pkb.createAST(VARIABLE,1,2);
	pkb.setFirstDescendant(rootAST,firstD);
	pkb.addSibling(firstD,pkb.createAST(VARIABLE,1,3));

	Pattern myPattern;
	myPattern.setPKB(new PKB(pkb));

	//test ASSIGNMENT
	//test pattern a ("x",_"y"_)
	AST expr1(VARIABLE,0,3);
	bool tf1 = myPattern.isPattern(ASSIGNMENT,2,new AST(expr1));
	CPPUNIT_ASSERT(tf1==true);
	//test pattern a ("x",_"x"_)
	AST expr2(VARIABLE,0,2);
	bool tf2 = myPattern.isPattern(ASSIGNMENT,2,new AST(expr2));
	cout<<tf2;
	CPPUNIT_ASSERT(tf2==false);
	//test pattern a("x","y")
	bool tf3 = myPattern.isPattern(ASSIGNMENT,2,new AST(*rootAST));
	CPPUNIT_ASSERT(tf3==true);
	//test pattern a (_,_"y"_)
	bool tf4 = myPattern.isPattern(ASSIGNMENT,NULL,new AST(expr1));
	CPPUNIT_ASSERT(tf4==true);
	//test pattern a (_,_"x"_)
	bool tf5 = myPattern.isPattern(ASSIGNMENT,NULL,new AST(expr2));
	CPPUNIT_ASSERT(tf5==false);
	//test pattern a(_,"y")
	bool tf6 = myPattern.isPattern(ASSIGNMENT,NULL,new AST(*rootAST));
	CPPUNIT_ASSERT(tf6==true);

	
	//test WHILE
	//4. while i... i(5)
	pkb.insertUses(WHILE,4,5);
	rootAST->addSibling(pkb.createAST(WHILE,4,-1));
	rootAST->getRightSibling()->setFirstDescendant(pkb.createAST(VARIABLE,4,5));

	//test pattern w(i,_)
	bool tf7 = myPattern.isPattern(WHILE,5,new AST(*rootAST));
	CPPUNIT_ASSERT(tf7==true);
	//test pattern w(_,_)
	bool tf8 = myPattern.isPattern(WHILE,NULL,new AST(*rootAST));
	CPPUNIT_ASSERT(tf8==true);
	//test pattern w(x,_)
	bool tf9 = myPattern.isPattern(WHILE,2,new AST(*rootAST));
	CPPUNIT_ASSERT(tf9==false);
	//test pattern w(y,_)
	bool tf10 = myPattern.isPattern(WHILE,3,new AST(*rootAST));
	CPPUNIT_ASSERT(tf10==false);
	
	*/
}

void PatternTest::testGetPatternStmt(){
	/*PKB pkb;
	//1. x=y
	pkb.insertModifies(ASSIGNMENT,1,2);
	pkb.insertUses(ASSIGNMENT,1,3);

	//create root AST
	AST* rootAST;
	AST* firstD;

	rootAST = pkb.createAST(ASSIGNMENT,1,-1);
	pkb.setRootAST(rootAST);
	//create nodes for x(2) and y(3)
	
	firstD = pkb.createAST(VARIABLE,1,2);
	pkb.setFirstDescendant(rootAST,firstD);
	pkb.addSibling(firstD,pkb.createAST(VARIABLE,1,3));

	Pattern myPattern;
	myPattern.setPKB(new PKB(pkb));

	//test ASSIGNMENT
	//test pattern a ("x",_"y"_)
	AST expr1(VARIABLE,0,3);
	STATEMENT_LIST *myList;
	myList =  myPattern.getPatternStmt(ASSIGNMENT,2,new AST(expr1));
	CPPUNIT_ASSERT_EQUAL(1,*myList->begin());

	//test pattern a ("x",_"x"_)
	AST expr2(VARIABLE,0,2);
	myList = myPattern.getPatternStmt(ASSIGNMENT,2,new AST(expr2));
	CPPUNIT_ASSERT((myList==NULL)==true);
	
	//test pattern a("x","y")
	myList = myPattern.getPatternStmt(ASSIGNMENT,2,new AST(*rootAST));
	CPPUNIT_ASSERT_EQUAL(1,*myList->begin());
	//test pattern a (_,_"y"_)

	myList = myPattern.getPatternStmt(ASSIGNMENT,NULL,new AST(expr1));
	CPPUNIT_ASSERT_EQUAL(1,*myList->begin());

	//test pattern a (_,_"x"_)
	myList = myPattern.getPatternStmt(ASSIGNMENT,NULL,new AST(expr2));
	CPPUNIT_ASSERT((myList==NULL)==true);

	//test pattern a(_,"y")
	myList = myPattern.getPatternStmt(ASSIGNMENT,NULL,new AST(*rootAST));
	CPPUNIT_ASSERT_EQUAL(1,*myList->begin());
	
	//test WHILE
	//4. while i... i(5)
	pkb.insertUses(WHILE,4,5);
	rootAST->addSibling(pkb.createAST(WHILE,4,-1));
	rootAST->getRightSibling()->setFirstDescendant(pkb.createAST(VARIABLE,4,5));

	//test pattern w(i,_)
	myList = myPattern.getPatternStmt(WHILE,5,new AST(*rootAST));
	CPPUNIT_ASSERT_EQUAL(4,*myList->begin());
	//test pattern w(_,_)
	myList = myPattern.getPatternStmt(WHILE,NULL,new AST(*rootAST));
	CPPUNIT_ASSERT_EQUAL(4,*myList->begin());
	//test pattern w(x,_)
	myList = myPattern.getPatternStmt(WHILE,2,new AST(*rootAST));
	CPPUNIT_ASSERT((myList==NULL)==true);
	//test pattern w(y,_)
	myList= myPattern.getPatternStmt(WHILE,3,new AST(*rootAST));
	CPPUNIT_ASSERT((myList==NULL)==true);
	*/
}
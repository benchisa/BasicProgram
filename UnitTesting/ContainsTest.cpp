#include "ContainsTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Parser.h>
#include <../SPA/PKB.h>
#include <../SPA/DesignExtractor.h>

using namespace std;


std::string ContainTestSrc="";

void ContainsTest::setUp()
{
	ContainTestSrc = " procedure xylo{\n"
		/*1*/	"     apple=apple + 1 + 3;\n"
		/*2*/	"     banana=apple+1-1;\n"
		/*3*/	"     carrot=apple;\n"
		/*4*/	"     banana=carrot*(egg+fish-apple);\n"
		/*5*/	"     donut=0;\n"
		/*6*/	"     egg=1;\n"
		/*7*/	"     fish=egg;\n"
		/*8*/	"     call yellow;\n"
		/*9*/	"     while apple{\n"
		/*10*/	"          while egg{\n"
		/*11*/	"              innard = 0;\n"
		/*12*/	"               while donut{\n"
		/*13*/	"                  gummy=egg+1;}\n"
		/*14*/	"               while fish{\n"
		/*15*/	"                    gummy=0;\n"
		/*16*/	"                    while innard{\n"
		/*17*/	"                         fish=innard;}}\n"
		/*18*/	"               ham=gummy;}\n"
		/*19*/	"          gummy=apple+9;}\n"
		/*20*/	"   apple=11;\n"
		/*21*/	"   if innard then{\n"
		/*22*/	"        jam=apple;}\n"
		/**/	"	else{\n"
		/*23*/	"        jam=gummy;\n"
		/*24*/	"        if egg then{\n"
		/*25*/	"             gummy=33;}\n"
			"          else{\n"
		/*26*/	"             gummy=99;}}}\n"
		/**/	"procedure yellow{\n"
		/*27*/	"	kimchi = 88;\n"
		/*28*/	"	leek=0;\n"
		/*29*/	"	while leek{\n"
		/*30*/	"      kimchi=kimchi+1-3;}\n"
		/*31*/	"	call zebra;\n"
		/*32*/	"	apple=kimchi;\n"
		/*33*/	"	jam=egg+2;}"
		/**/	"	procedure zebra{\n"
		/*34*/	"	 meat=1;\n"
		/*35*/	"	 banana=meat+10;\n"
		/*36*/	"	 if meat then{\n"
		/*37*/	"		  carrot=banana+22;}\n"
		/**/	"	 else{\n"
		/*38*/	"		  carrot=banana;}\n"
		/*39*/	"	 apple=1;\n"
		/*40*/	"	 kimchi=apple+10;\n"
		/*41*/	"	 call extra;}";
}


void ContainsTest::tearDown()
{
}

//CPPUNIT_TEST_SUITE_REGISTRATION( ContainsTest ); // Note 4 

void ContainsTest::testGetContainsStarTest(){
	Parser p;
	DesignExtractor *de;
	PKB *pkb;
	p.setSource(ContainTestSrc);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	de = new DesignExtractor(pkb);

	/*
	cout << "\n=============ContainsStar=============\n";
	cout << "============= UNKNOWN, UNKNOWN ===========\n";
	cout << de->getContainStarResult(STMT_LIST, 0, STMT_LIST, 0).size() << "\n";
	cout << de->getContainStarResult(STMT_LIST, 10, STMT_LIST, 0).size() << "\n";
	cout << de->getContainStarResult(STMT_LIST, 0, STMT_LIST, 13).size() << "\n";

	cout << de->getContainStarResult(STATEMENT, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, ASSIGNMENT, 0).size() <<"\n"; 

	cout << de->getContainStarResult(STATEMENT, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(ASSIGNMENT, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(WHILE, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, ASSIGNMENT, 0).size() <<"\n"; 
	cout << de->getContainStarResult(WHILE, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(IF, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, ASSIGNMENT, 0).size() <<"\n"; 
	cout << de->getContainStarResult(IF, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, MINUS, -1).size() <<"\n\n"; //

	cout << "============= UNKNOWN, KNOWN ===========\n";
	cout << de->getContainStarResult(STATEMENT, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, STATEMENT, 44).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, CONSTANT, pkb->getConstantIndex(1)).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, VARIABLE, pkb->getVarIndex("meat")).size() <<"\n\n"; //

	cout << de->getContainStarResult(PROCEDURE, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, MINUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, VARIABLE, pkb->getVarIndex("b")).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, CONSTANT, pkb->getConstantIndex(11)).size() <<"\n\n"; //
	*/
}

void ContainsTest::testGetIsContainsStarTest(){
	Parser p;
	DesignExtractor *de;
	PKB *pkb;
	p.startParse();
	pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	de = new DesignExtractor(pkb);

	cout << de->getIsContainStarResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainStarResult(MINUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, CONSTANT, pkb->getConstantIndex(8)) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainStarResult(MINUS, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";
	cout << de->getIsContainStarResult(MULTIPLY, -1, CONSTANT, pkb->getConstantIndex(8)) << "\n";
}

void ContainsTest::testGetIsContainsTest(){

	Parser p;
	DesignExtractor *de;
	PKB *pkb;
	p.setSource(ContainTestSrc);
	p.startParse();
	pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	de = new DesignExtractor(pkb);

	/*cout << de->getIsContainResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainResult(MINUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainResult(MINUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainResult(PLUS, -1, CONSTANT, pkb->getConstantIndex(1)) << "\n";
	cout << de->getIsContainResult(MULTIPLY, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";
	cout << de->getIsContainResult(MINUS, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";*/

}
void ContainsTest::testGetContainsTest(){
	Parser p;
	DesignExtractor *de;
	PKB *pkb;
	p.setSource(ContainTestSrc);
	pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	de = new DesignExtractor(pkb);

	/*CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 1, WHILE, 0);
	CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 1, STMT_LIST, 0);
	CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 1, IF, 0);
	
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, STMT_LIST, 0);
	CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 0, STMT_LIST, 0);
	CONTAIN_LIST cList = de->getContainResult(WHILE, 0, STMT_LIST, 0);
	CONTAIN_LIST cList = de->getContainResult(IF, 0, STMT_LIST, 0);
	CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, STATEMENT, 0);
	CONTAIN_LIST cList = de->getContainResult(IF, 0, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(WHILE, 0, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, CONSTANT, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, CONSTANT, 0);
	

	CONTAIN_LIST cList = de->getContainResult(IF, 0, VARIABLE, pkb->getVarIndex("innard"));
	CONTAIN_LIST cList = de->getContainResult(WHILE, 0, VARIABLE, pkb->getVarIndex("apple"));
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, VARIABLE, pkb->getVarIndex("apple"));
	CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, PLUS, -1);
	CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, MINUS, -1);
	CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, MULTIPLY, -1);
	CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, VARIABLE, pkb->getVarIndex("apple"));
	CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, CONSTANT, 88);
	CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, STATEMENT, 10);
	CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, CALL, 8);
	CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, ASSIGNMENT, 11);
	CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, IF, 24);
	CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, WHILE, 16);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, STMT_LIST, 15);
	CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 0, STMT_LIST, 1);
	CONTAIN_LIST cList = de->getContainResult(WHILE, 0, STMT_LIST, 17);
	CONTAIN_LIST cList = de->getContainResult(IF, 0, STMT_LIST, 23);

	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 1, STATEMENT, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 2, STATEMENT, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 11, STATEMENT, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 11, WHILE, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 11, ASSIGNMENT, 0);
	CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 11, ASSIGNMENT, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 22, STATEMENT, 0);

	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 1, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 27, CONSTANT, 0);
	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 21, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(IF, 21, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(IF, 21, STMT_LIST, 0);

	CONTAIN_LIST cList = de->getContainResult(STATEMENT, 10, VARIABLE, 0);
	CONTAIN_LIST cList = de->getContainResult(WHILE, 10, VARIABLE, 0);

	cout << de->getContainResult(STATEMENT, 30, STMT_LIST, 0).size() << "\n";
	cout << de->getContainResult(STATEMENT, 36, STMT_LIST, 0).size() << "\n";
	cout << de->getContainResult(PROCEDURE, pkb->getProcIndex("Goneril"), STMT_LIST, 0).size() << "\n";
	cout << de->getContainResult(STATEMENT, 99, STMT_LIST, 0).size() << "\n";

	cout << cList.size() << "\n";

	*/
}
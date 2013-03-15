#include "ParserTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Parser.h>
#include <../SPA/PKB.h>
#include <../SPA/DesignExtractor.h>
using namespace std;

void ParserTest::setUp()
{
}


void ParserTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest ); // Note 4 

void ParserTest::testComplex(){
	Parser p;
	std::string src, src1, src2, src3;

	src = " procedure xylo{\n"
		/*1*/	"     apple=apple+1+1;\n"
		/*2*/	"     banana=apple+1;\n"
		/*3*/	"     carrot=apple;\n"
		/*4*/	"     banana=carrot*(egg+fish-apple);\n"
		/*5*/	"     donut=0;\n"
		/*6*/	"     egg=11;\n"
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
		/*30*/	"      kimchi=kimchi+1;}\n"
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
		/*41*/	"	 call extra;}\n"
		/**/	"	procedure extra{\n"
		/*42*/	"	if a then{\n"
		/*43*/	"	 b = c+1;}\n"
		/*44*/	"	else{ b = c;}}\n";

	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	DesignExtractor *de = new DesignExtractor(pkb);
	de->createCFG();
	cout << "\n=============ContainsStar=============\n";
	cout << "============= UNKNOWN, UNKNOWN ===========\n";
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
	CONTAIN_LIST result =  de->getContainStarResult(PROCEDURE, 0, CONSTANT, pkb->getConstantIndex(11));
	CONTAIN_LIST::iterator itr = result.begin();

	while(itr!=result.end()){
		cout << "Contain(" << itr->first << ", " << itr->second <<")\n";
		itr++;
	}
	cout << "============= KNOWN, UNKNOWN ===========\n";
	//CONTAIN_LIST result = de->getContainStarResult(STATEMENT, 9, VARIABLE, 0); correct
	//CONTAIN_LIST result = de->getContainStarResult(STATEMENT, 9, CONSTANT, 0); correct
	//CONTAIN_LIST result = de->getContainStarResult(STATEMENT, 9, PLUS, 0); // find this weird to return something. but nevertheless...
	//CONTAIN_LIST result = de->getContainStarResult(PROCEDURE, 4, PLUS, -1); correct
	//CONTAIN_LIST result = de->getContainStarResult(PROCEDURE, 1, PLUS, -1);
	//CONTAIN_LIST result = de->getContainStarResult(PROCEDURE, 1, MINUS, -1);
	/*CONTAIN_LIST result = de->getContainStarResult(PROCEDURE, 1, CONSTANT, -1);
	CONTAIN_LIST::iterator itr = result.begin();

	while(itr!=result.end()){
		cout << "Contain(" << itr->first << ", " << itr->second <<")\n";
		itr++;
	}
	cout << de->getContainStarResult(PLUS, -1, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(PLUS, -1, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(PLUS, -1, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PLUS, -1, MINUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PLUS, -1, MULTIPLY, -1).size() <<"\n\n"; //
	
	cout << de->getContainStarResult(MINUS, -1, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(MINUS, -1, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(MINUS, -1, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(MINUS, -1, MINUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(MINUS, -1, MULTIPLY, -1).size() <<"\n\n"; //
	
	cout << de->getContainStarResult(MULTIPLY, -1, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(MULTIPLY, -1, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(MULTIPLY, -1, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(MULTIPLY, -1, MINUS, -1).size() <<"\n"; //*/
	//cout << de->getContainStarResult(MULTIPLY, -1, MULTIPLY, -1).size() <<"\n"; //

	/*cout << "============= KNOWN, KNOWN ===========\n";
	cout << de->getIsContainStarResult(STATEMENT, 36, STATEMENT, 37) << "\n"; //true
	cout << de->getIsContainStarResult(STATEMENT, 9, VARIABLE, pkb->getVarIndex("fish")) << "\n\n"; //true

	cout << de->getIsContainStarResult(PROCEDURE, 1, VARIABLE,  pkb->getVarIndex("carrot")) << "\n"; //true
	cout << de->getIsContainStarResult(PROCEDURE, 1, VARIABLE,  pkb->getVarIndex("asd")) << "\n"; //false
	cout << de->getIsContainStarResult(PROCEDURE, 1, CONSTANT, pkb->getConstantIndex(1)) << "\n"; //true
	cout << de->getIsContainStarResult(PROCEDURE, 1, STATEMENT, 2) << "\n"; //true
	cout << de->getIsContainStarResult(PROCEDURE, 1, PLUS, -1) << "\n"; //true
	cout << de->getIsContainStarResult(PROCEDURE, 1, MINUS, -1) << "\n"; //true
	cout << de->getIsContainStarResult(PROCEDURE, 1, MULTIPLY, -1) << "\n\n"; //true
	cout << de->getIsContainStarResult(PROCEDURE, 2, MINUS, -1) << "\n"; //false
	cout << de->getIsContainStarResult(PROCEDURE, 2, STATEMENT, 1) << "\n\n"; //false

	cout << de->getIsContainStarResult(PLUS, -1, VARIABLE, pkb->getVarIndex("carrot")) << "\n"; //false
	cout << de->getIsContainStarResult(PLUS, -1, VARIABLE, pkb->getVarIndex("egg")) << "\n"; //true
	cout << de->getIsContainStarResult(PLUS, -1, CONSTANT, pkb->getConstantIndex(1)) << "\n"; //true
	cout << de->getIsContainStarResult(MINUS, -1, CONSTANT, pkb->getConstantIndex(1)) << "\n"; // false
	cout << de->getIsContainStarResult(MULTIPLY, -1, VARIABLE, pkb->getVarIndex("carrot")) << "\n"; //true
	cout << de->getIsContainStarResult(MULTIPLY, -1, VARIABLE, pkb->getVarIndex("egg")) << "\n"; //true
	cout << de->getIsContainStarResult(PLUS, -1, PLUS,-1) << "\n"; //true
	cout << de->getIsContainStarResult(PLUS, -1, MINUS,-1) << "\n"; //false
	cout << de->getIsContainStarResult(PLUS, -1, MULTIPLY,-1) << "\n"; //false
	cout << de->getIsContainStarResult(MINUS, -1, PLUS,-1) << "\n"; //true
	cout << de->getIsContainStarResult(MULTIPLY, -1, PLUS,-1) << "\n"; //true
	cout << de->getIsContainStarResult(MULTIPLY, -1, MINUS,-1) << "\n\n"; //true

	cout << de->getIsContainStarResult(STMT_LIST, -1, STATEMENT,2) << "\n"; //true
	cout << de->getIsContainStarResult(STMT_LIST, -1, PLUS,-1) << "\n"; //true
	cout << de->getIsContainStarResult(STMT_LIST, -1, MINUS,-1) << "\n"; //true
	cout << de->getIsContainStarResult(STMT_LIST, -1, MULTIPLY,-1) << "\n"; //true
	cout << de->getIsContainStarResult(STMT_LIST, -1, VARIABLE, pkb->getVarIndex("carrot")) << "\n"; //true
	cout << de->getIsContainStarResult(STMT_LIST, -1, VARIABLE, pkb->getVarIndex("carr")) << "\n"; //false
	cout << de->getIsContainStarResult(STMT_LIST, -1, STATEMENT,45) << "\n\n"; //false
	



	/*cout << "=============Contains=============\n";
	cout << "============= KNOWN, KNOWN ===========\n";
	cout << de->getIsContainResult(PLUS, 0, VARIABLE, pkb->getVarIndex("apple")) << "\n"; // true
	cout << de->getIsContainResult(PLUS, 0, CONSTANT, pkb->getConstantIndex(22)) << "\n"; // true
	cout << de->getIsContainResult(PLUS, 0, PLUS, 0) << "\n"; //true
	cout << de->getIsContainResult(PLUS, 0, MINUS, 0) << "\n";  //true
	cout << de->getIsContainResult(PLUS, 0, MULTIPLY, 0) << "\n"; //false,
	cout << de->getIsContainResult(STMT_LIST, 0, STATEMENT,1) << "\n"; // true because procedure below is stmt1
	cout << de->getIsContainResult(STMT_LIST, 0, STATEMENT,2) << "\n"; // false
	cout << de->getIsContainResult(STATEMENT, 1, STATEMENT, 2) << "\n"; // false
	cout << de->getIsContainResult(STATEMENT, 29, STATEMENT, 38) << "\n"; //false
	cout << de->getIsContainResult(STATEMENT, 33, STATEMENT, 34) << "\n"; //false
	cout << de->getIsContainResult(STATEMENT, 36, STATEMENT, 37) << "\n"; //true
	cout << de->getIsContainResult(STATEMENT, 36, STATEMENT, 38) << "\n"; //true
	cout << de->getIsContainResult(PROCEDURE, 1, STATEMENT, 1) << "\n"; // true
	cout << de->getIsContainResult(PROCEDURE, 1, STATEMENT, 2) << "\n"; // false
	cout << de->getIsContainResult(PROCEDURE, 1, STMT_LIST, 0) << "\n"; //true
	cout << de->getIsContainResult(PROCEDURE, 1, WHILE, 9) << "\n"; //false
	cout << de->getIsContainResult(PROCEDURE, pkb->getProcIndex("extra"), STATEMENT, 42) << "\n";
	
	
	cout << "============= KNOWN, UNKNOWN ===========\n";
	cout << de->getContainResult(STATEMENT, 1, STATEMENT, 0).size() << "\n"; // 0
	cout << de->getContainResult(STATEMENT, 9, STATEMENT, 0).size() << "\n"; // 1
	cout << de->getContainResult(STATEMENT, 21, STATEMENT, 0).size() << "\n"; // 2
	
	cout << de->getContainResult(STATEMENT, 29, ASSIGNMENT, 0).size() << "\n"; // 1
	cout << de->getContainResult(STATEMENT, 21, ASSIGNMENT, 0).size() << "\n"; // 2
	cout << de->getContainResult(STATEMENT, 21, WHILE, 0).size() << "\n"; // 0
	cout << de->getContainResult(STATEMENT, 9, WHILE, 0).size() << "\n"; // 1
	cout << de->getContainResult(STATEMENT, 42, IF, 0).size() << "\n"; // 0

	cout << de->getContainResult(PROCEDURE, 1, STATEMENT, 0).size() << "\n"; // 1
	cout << de->getContainResult(PROCEDURE, 1, WHILE, 0).size() << "\n"; // 0
	cout << de->getContainResult(PROCEDURE, 1, ASSIGNMENT, 0).size() << "\n"; //1
	cout << de->getContainResult(PROCEDURE, 1, CALL, 0).size() << "\n"; //0
	cout << de->getContainResult(PROCEDURE, 1, IF, 0).size() << "\n"; //0
	cout << de->getContainResult(PROCEDURE, 4, IF, 0).front().second << "\n"; //42
	
	cout << "============= UNKNOWN, KNOWN ===========\n";
	cout <<de->getContainResult(STATEMENT, 0, STATEMENT, 2).size() << "\n"; // 0
	cout <<de->getContainResult(STATEMENT, 0, STATEMENT, 15).size() << "\n"; // 1
	cout <<de->getContainResult(WHILE, 0, STATEMENT, 2).size() << "\n"; // 0
	cout <<de->getContainResult(IF, 0, STATEMENT, 22).size() << "\n"; // 1
	
	cout <<de->getContainResult(ASSIGNMENT, 0, PLUS, -1).size() << "\n"; // 
	cout <<de->getContainResult(ASSIGNMENT, 0, MINUS, -1).size() << "\n"; // 0
	cout <<de->getContainResult(ASSIGNMENT, 0, MULTIPLY, -1).size() << "\n"; // 1
	cout <<de->getContainResult(ASSIGNMENT, 0, CONSTANT, pkb->getConstantIndex(11)).size() << "\n"; // 2
	cout <<de->getContainResult(ASSIGNMENT, 0, VARIABLE, pkb->getVarIndex("kimchi")).size() << "\n"; //4
	
	cout <<de->getContainResult(WHILE, 0, VARIABLE, pkb->getVarIndex("leek")).size() << "\n"; // 1
	cout <<de->getContainResult(IF, 0, VARIABLE, pkb->getVarIndex("meat")).size() << "\n"; // 1


	cout << "============= UNKNOWN, UNKNOWN ===========\n";
	cout <<de->getContainResult(STATEMENT, 0, STATEMENT,0).size() << "\n"; // 14

	cout <<de->getContainResult(WHILE, 0, WHILE,0).size() << "\n"; // 1
	cout <<de->getContainResult(WHILE, 0, ASSIGNMENT,0).size() << "\n"; // 5
	cout <<de->getContainResult(WHILE, 0, IF,0).size() << "\n"; // 0
	cout <<de->getContainResult(WHILE, 0, CALL,0).size() << "\n"; // 0
	cout <<de->getContainResult(WHILE, 0, STATEMENT,0).size() << "\n"; // 6
	cout << "\n";
	cout << de->getContainResult(IF, 0, WHILE,0).size() << "\n"; // 0
	cout << de->getContainResult(IF, 0, ASSIGNMENT,0).size() << "\n"; // 8
	cout << de->getContainResult(IF, 0, IF,0).size() << "\n"; // 0
	cout << de->getContainResult(IF, 0, CALL,0).size() << "\n"; // 0
	cout << de->getContainResult(IF, 0, STATEMENT,0).size() << "\n"; // 8

	cout << "============= SPECIAL CASE ===========\n";
	cout <<de->getContainResult(PLUS, -1, PLUS,-1).size() << "\n"; // 1
	cout <<de->getContainResult(PLUS, -1, MINUS,-1).size() << "\n"; // 1
	cout <<de->getContainResult(PLUS, -1, VARIABLE,0).size() << "\n"; // 10
	cout <<de->getContainResult(PLUS, -1, CONSTANT,0).size() << "\n"; // 10


	// known known
	/*cout << de->getIsContainResult(PLUS, 0, VARIABLE, pkb->getVarIndex("apple")) << "\n"; // true
	cout << de->getIsContainResult(PLUS, 0, PLUS, 0) << "\n"; //true
	cout << de->getIsContainResult(PLUS, 0, MINUS, 0) << "\n";  //true
	cout << de->getIsContainResult(PLUS, 0, MULTIPLY, 0) << "\n"; //false,
	cout << de->getIsContainResult(STMT_LIST, 0, STATEMENT,1) << "\n"; // true because procedure below is stmt1
	cout << de->getIsContainResult(STMT_LIST, 0, STATEMENT,2) << "\n"; // false
	*/
	/*cout << de->getIsContainResult(STATEMENT, 1, STATEMENT, 2) << "\n"; // false
	cout << de->getIsContainResult(STATEMENT, 29, STATEMENT, 38) << "\n"; //false
	cout << de->getIsContainResult(STATEMENT, 33, STATEMENT, 34) << "\n"; //false
	cout << de->getIsContainResult(STATEMENT, 36, STATEMENT, 37) << "\n"; //true
	cout << de->getIsContainResult(STATEMENT, 36, STATEMENT, 38) << "\n"; //true
	*/
	// Contain("extra", 2)
	// Contain("xylo", 1)
	/*cout << de->getIsContainResult(PROCEDURE, 1, STATEMENT, 1) << "\n"; // true
	cout << de->getIsContainResult(PROCEDURE, 1, STATEMENT, 2) << "\n"; // false
	cout << de->getIsContainResult(PROCEDURE, 1, STMT_LIST, 0) << "\n"; //true
	cout << de->getIsContainResult(PROCEDURE, 1, WHILE, 9) << "\n"; //false
	cout << de->getIsContainResult(PROCEDURE, pkb->getProcIndex("extra"), STATEMENT, 42) << "\n"; //true*/
	//cout << pkb->getStmtListSize() << "\n";

	cout << "End of Program\n";


	//cout << "Assign's Size: " << pkb->getAssignSize() << "\n";
	// Test for Follows
	/*FOLLOWS_LIST result = pkb->getFollows(0,0);
	FOLLOWS_LIST::iterator itr = result.begin();
	while(itr!=result.end()){
		//cout << itr->first << "," << itr->second << "\n";
		itr++;
	}
	NEXT_LIST nxtResult = pkb->getNext(0,0);
	NEXT_LIST::iterator nxtItr = nxtResult.begin();

	while(nxtItr!=nxtResult.end()){
		cout << nxtItr->first << ", " << nxtItr->second << "\n";
		nxtItr++;
	}
	
	MODIFIES_LIST modifiesResult = pkb->getModifies(PROCEDURE,0,0);
	MODIFIES_LIST::iterator itr = modifiesResult.begin();
	while(itr!=modifiesResult.end()){
		cout << "Procedure " << itr->first << " modifies " << pkb->getVarName(itr->second) << "\n";
		itr++;
	}

	USES_LIST usesResult = pkb->getUses(PROCEDURE,0,0);
	USES_LIST::iterator uitr = usesResult.begin();
	while(uitr!=usesResult.end()){
		cout << "Procedure " << uitr->first << " uses " << pkb->getVarName(uitr->second) << "\n";
		uitr++;
	}

	PARENT_LIST result = pkb->getParent(0,0);
	PARENT_LIST::iterator itr = result.begin();
	while(itr!=result.end()){
		cout << itr->first << ", " << itr->second << "\n";
		itr++;
	}*/
}

void ParserTest::testCall(){
	Parser p;
	std::string src;

	src = "procedure test1{\n"
	/*1*/	"a = c;"
	/*2*/	"call test2;"
	/*3*/	"a = d;}"
	/**/	"procedure test2{\n"
	/*4*/	"b = 1;}";

	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == CALL);
}
void ParserTest::testIf()
{
	Parser p;
	std::string src;
	
	// Follows(2, 3), Follows(4, 5)
	//cout << "Start=====\n";
	src = "procedure test1{\n"
	/*1*/	"if b then {"
	/*2*/	"a = c;"
	/*3*/	"a = d;}"
	/**/	"else{"
	/*4*/	"c = d;"
	/*5*/	"e = d;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	CPPUNIT_ASSERT(pkb->isFollows(1, 2) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 3) == false);
	CPPUNIT_ASSERT(pkb->isFollows(2, 3) == true);
	CPPUNIT_ASSERT(pkb->isFollows(4, 5) == true);
	CPPUNIT_ASSERT(pkb->isFollows(2, 4) == false);
	CPPUNIT_ASSERT(pkb->isFollows(2, 5) == false);
	CPPUNIT_ASSERT(pkb->isFollows(2, 6) == false);
	CPPUNIT_ASSERT(pkb->isFollows(3, 4) == false);
	CPPUNIT_ASSERT(pkb->isFollows(4, 6) == false);

	// Test Uses and Modifies for if
	MODIFIES_LIST modifiesList = pkb->getModifies(IF, 1, 0);
	MODIFIES_LIST::iterator modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "e");
	
	USES_LIST useList = pkb->getUses(IF, 1, 0);
	USES_LIST::iterator usesListItr = useList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "d");

	// Follows(2, 3), Follows(3, 4), Follows(7, 8), Follows(1, 9)
	// Parent(1, 2) (1, 3), (1, 4), (4, 5), (4, 6), (1, 7), (1, 8)
	src = "procedure test2{\n"
	/*1*/	"if b then {"
	/*2*/	"a = c;"
	/*3*/	"a = d;"
	/*4*/	"if b then{"
	/*5*/	"a = 1;}"
	/**/	"else{"
	/*6*/	"b = 1;}}"
	/**/	"else{"
	/*7*/	"c = d;"
	/*8*/	"e = d;}"
	/*9*/	"z = y;}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	pkb = p.getPKB();
	CPPUNIT_ASSERT(pkb->isFollows(2, 3) == true);
	CPPUNIT_ASSERT(pkb->isFollows(3, 4) == true);
	CPPUNIT_ASSERT(pkb->isFollows(7, 8) == true);
	CPPUNIT_ASSERT(pkb->isFollows(1, 9) == true);
	CPPUNIT_ASSERT(pkb->isFollows(1, 8) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 2) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 4) == false);

	CPPUNIT_ASSERT(pkb->isParent(1, 2) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 3) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 4) == true);
	CPPUNIT_ASSERT(pkb->isParent(4, 5) == true);
	CPPUNIT_ASSERT(pkb->isParent(4, 6) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 7) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 8) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 9) == false);

	// Test Uses and Modifies for if
	modifiesList = pkb->getModifies(IF, 1, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "e");
	
	modifiesList = pkb->getModifies(IF, 4, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");

	useList = pkb->getUses(IF, 1, 0);
	usesListItr = useList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "d");
	// varindex 7 is variable y
	CPPUNIT_ASSERT(pkb->isUses(IF, 1, 7) == false);

	// Follows(1, 2), Follows(3, 4), Follows(5, 6), Follows(2, 7)
	src = "procedure test3{\n"
		"a = y;"
		"if b then {"
		"a = c;"
		"a = d;}"
		"else{"
		"c = d;"
		"e = d;}"
		"a = 1;}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	pkb = p.getPKB();

	// Test Follows, Parent
	CPPUNIT_ASSERT(pkb->isFollows(1, 2) == true);
	CPPUNIT_ASSERT(pkb->isFollows(3, 4) == true);
	CPPUNIT_ASSERT(pkb->isFollows(5, 6) == true);
	CPPUNIT_ASSERT(pkb->isFollows(2, 7) == true);

	CPPUNIT_ASSERT(pkb->isFollows(1, 3) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 4) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 5) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 6) == false);

	CPPUNIT_ASSERT(pkb->isParent(2, 3) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 4) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 5) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 6) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 7) == false);

	// Test AST
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == IF);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getRightSibling()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()) == ASSIGNMENT);
}

void ParserTest::testMultipleProcedures()
{
	Parser p;
	std::string src;

	src =	"procedure main{\n"
			"	a = a + 2;}\n"
			"procedure text1{\n"
			"	b = a * (2 + c);\n"
			"	c = c - 2;}"
			"procedure text2{\n"
			"	while a{\n"
			"		d = d - 2;}\n"
			"	e = e + 1;\n"
			"	if a then {\n"
			"		h = h * (a * b);\n"
			"		b = b + 1;}\n"
			"	else{\n"
			"		c = b;\n"
			"		while z{\n"
			"			abc = xyz;\n"
			"			abc1 = xyz2 * xyz + h - y;}\n"
			"		a=abc1*2;}}";
			
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	
	// AST Test
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == WHILE);

	// Parent Test to ensure insertion is correct.
	CPPUNIT_ASSERT(!pkb->isParent(1, 9));
	CPPUNIT_ASSERT(pkb->isParent(4, 5));

	// Follow Test
	CPPUNIT_ASSERT(!pkb->isFollows(1, 2));
	CPPUNIT_ASSERT(!pkb->isFollows(3, 4));
	CPPUNIT_ASSERT(pkb->isFollows(4, 6));

}

void ParserTest::testOperators(){
	Parser p;
	std::string src;

	src = "procedure main{\n"
		"a = y;}\n";
	p.setSource(src);
	p.startParse();
	PKB *pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	
	src = "procedure main{\n"
		"a = 1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == CONSTANT);
	

	src = "procedure main{\n"
		"a = y*1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	
	src = "procedure main{\n"
		"a = y+1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);

	src = "procedure main{\n"
		"a = y-1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);

	
	src = "procedure main{\n"
		"a = (y+1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	
	
	src = "procedure main{\n"
		"a = (y-1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = (y*1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	

	src = "procedure main{\n"
		"a = b + c * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	/*CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()))=="b");
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()))=="c");
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()))=="d");
	*/
	src = "procedure main{\n"
		"a = b - c * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	//CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = b * c + d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	/*CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	*/
	
	src = "procedure main{\n"
		"a = b * c - d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = (b + c) * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);


	src = "procedure main{\n"
		"a = b + c + d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == PLUS);
}

// method to test whether parser can parse different source code
void ParserTest::testCorrectSource() {
	Parser p;
	std::string src;
	
	// Purpose: Simplest SIMPLE Source
	src = "procedure test1{\n"
		"a = y+1+x;}\n";
	p.setSource(src);

	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	
	src = "procedure main{\n"
		"while a{\n"
		"b = y + 2;}\n"
		"c = a + 1;}";
	p.setSource(src);

	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	src = "procedure main{\n"
		"while a{\n"
		"b = y + 2;}\n"
		"c = a + 1;}";
	p.setSource(src);
	
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	
	// Purpose: Multiple lines, procedures, long simple expressions.
	// PROGLINE: 8, ContainerIndex: 4, 7
	src = "procedure main{\n"
		"a = y + 1 + 0 + x + b + a;\n" 
		"b = y;\n"
		"b = x + y + 1 + u;\n"
		"while a {\n"
		"x = y; \n"
		"hhhhh = 2;\n"
		"while b { \n"
		"k = 1; } }}\n";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
}

void ParserTest::testGetLastError(){
	Parser p;
	std::string src;	

	// Illegal codes
	// Purpose: Missing ;
	src = "procedure main{\nwhile c{\na=b}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): missing colon");

	// Purpose: Invalid name
	src = "procedure main{\nwhile !@#c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid while var_name");

	
	src = "procedure ma&#(!*in{\nwhile c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid/repeated procedure name");

	// Purpose: Missing {
	src = "procedure main\nwhile c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): missing bracket");

	// Purpose: Invalid Procedure
	
	src = "procure main{\nwhile !@#c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid procedure");
}

void ParserTest::testGetPKB(){
	Parser p;
	std::string src;
	src = "procedure main{\n"
		"a = y + 1 + 0 + x + b + a;\n" 
		"b = y;\n"
		"b = x + y + 1 + u;\n"
		"while a {\n"
		"x = y; \n"
		"hhhhh = 2;\n"
		"while b { \n"
		"k = 1; } }}\n";

	p.setSource(src);
	p.startParse();
	PKB *pkb = p.getPKB();

	// Test Procedure/ProcTable
	CPPUNIT_ASSERT(pkb->getProcedure(1)->getProcName() == "main");
	CPPUNIT_ASSERT_EQUAL(1,pkb->getProcedure(1)->getStartProgLine());
	CPPUNIT_ASSERT_EQUAL(9,pkb->getProcedure(1)->getEndProgLine());

	//Test Parent
	PARENT_LIST parentList = pkb->getParent(0, 5);
	PARENT_LIST::iterator parentItr = parentList.begin();
	CPPUNIT_ASSERT(parentItr->first == 4);

	parentList = pkb->getParent(0, 7);
	parentItr = parentList.begin();
	CPPUNIT_ASSERT(parentItr->first == 4);

	//Test Follow
	CPPUNIT_ASSERT_EQUAL(6, pkb->getFollows(0, 7).begin()->first);
	CPPUNIT_ASSERT_EQUAL(1, pkb->getFollows(0, 2).begin()->first);
	CPPUNIT_ASSERT_EQUAL(3, pkb->getFollows(2, 0).begin()->second);
	CPPUNIT_ASSERT(pkb->getFollows(4, 0).empty() == true);

	//Test VarTable
	VAR_LIST varList = *pkb->getAllVar();
	VAR_LIST::iterator varTableItr = varList.begin();
	CPPUNIT_ASSERT(*(varTableItr++) == "a");
	CPPUNIT_ASSERT(*(varTableItr++) == "y");
	CPPUNIT_ASSERT(*(varTableItr++) == "hhhhh");
	CPPUNIT_ASSERT(*(varTableItr++) == "x");

	//Test Modifies
	MODIFIES_LIST modifiesList = pkb->getModifies(PROCEDURE, 1, 0);
	MODIFIES_LIST::iterator modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "x");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "hhhhh");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "k");

	//Test Modifies While LOOP
	modifiesList = pkb->getModifies(WHILE, 4, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "x");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "hhhhh");

	//Test Uses
	USES_LIST useList = pkb->getUses(PROCEDURE, 1, 0);
	CPPUNIT_ASSERT(useList.size() == 5);

	//Test Uses WHILE LOOP
	useList = pkb->getUses(WHILE, 4, 0);
	CPPUNIT_ASSERT(useList.size() == 3);

	// Test AST
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	
	ast = ast->getFirstDescendant()->getFirstDescendant()->getRightSibling();
	CPPUNIT_ASSERT(pkb->getType(ast) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == WHILE);
	
	ast = ast->getRightSibling()->getRightSibling();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getRightSibling()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == ASSIGNMENT);

	ast = ast->getFirstDescendant()->getRightSibling()->getFirstDescendant();
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == WHILE);

}
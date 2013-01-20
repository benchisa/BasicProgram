#include "ParserTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Parser.h>
#include <../SPA/PKB.h>
using namespace std;

void ParserTest::setUp()
{
}


void ParserTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest ); // Note 4 
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
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "c");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "e");
	
	USES_LIST useList = pkb->getUses(IF, 1, 0);
	USES_LIST::iterator usesListItr = useList.begin();
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "b");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "c");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "d");

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
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "c");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "e");
	
	modifiesList = pkb->getModifies(IF, 4, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "a");

	useList = pkb->getUses(IF, 1, 0);
	usesListItr = useList.begin();
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "b");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "c");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "d");
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

	src = "procedure main{\n"
	/*1*/	"a = y;}\n"
	/**/	"procedure test1{\n"
	/*2*/	"b = x + y;\n"
	/*3*/	"if b then{\n"
	/*4*/	"a = b;}"
	/**/		"else{"
	/*5*/	"a = c;}}"
	/**/	"procedure test2{\n"
	/*6*/	"c = a + b;\n"
	/*7*/	"b = 1;\n"
	/*8*/	"while a{\n"
	/*9*/	"c = 2;\n"
	/*10*/	"a = a - 1 * (c + d);}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	
	// Test for AST
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getData(ast->getRightSibling()) == 2);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == IF);

	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getData(ast->getRightSibling()->getRightSibling()) == 3);

	// Test for Follows
	CPPUNIT_ASSERT(pkb->isFollows(1, 2) == false);
	CPPUNIT_ASSERT(pkb->isFollows(2, 3) == true);
	CPPUNIT_ASSERT(pkb->isFollows(3, 4) == false);
	CPPUNIT_ASSERT(pkb->isFollows(4, 5) == false);
	CPPUNIT_ASSERT(pkb->isFollows(5, 6) == false);
	CPPUNIT_ASSERT(pkb->isFollows(3, 6) == false);
	CPPUNIT_ASSERT(pkb->isFollows(6, 7) == true);
	CPPUNIT_ASSERT(pkb->isFollows(7, 8) == true);
	CPPUNIT_ASSERT(pkb->isFollows(8, 9) == false);
	CPPUNIT_ASSERT(pkb->isFollows(9, 10) == true);
	
	// Test for Parent
	CPPUNIT_ASSERT(pkb->isParent(3, 4) == true);
	CPPUNIT_ASSERT(pkb->isParent(3, 5) == true);
	CPPUNIT_ASSERT(pkb->isParent(3, 6) == false);
	CPPUNIT_ASSERT(pkb->isParent(3, 9) == false);
	CPPUNIT_ASSERT(pkb->isParent(7, 9) == false);
	CPPUNIT_ASSERT(pkb->isParent(8, 9) == true);
	CPPUNIT_ASSERT(pkb->isParent(8, 10) == true);


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
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == CONSTANT);
	

	src = "procedure main{\n"
		"a = y*1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	
	src = "procedure main{\n"
		"a = y+1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);

	src = "procedure main{\n"
		"a = y-1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);

	
	src = "procedure main{\n"
		"a = (y+1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	
	
	src = "procedure main{\n"
		"a = (y-1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = (y*1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	

	src = "procedure main{\n"
		"a = b + c * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(*pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()))=="b");
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(*pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()))=="c");
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(*pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()))=="d");
	
	src = "procedure main{\n"
		"a = b - c * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = b * c + d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	
	
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
	/*src = "procedure main{\nwhile c{\na=b}}";
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
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid procedure name");

	// Purpose: Missing {
	src = "procedure main\nwhile c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): missing bracket");

	// Purpose: Invalid Factor()
	src = "procedure main{\nwhile c{\na=!@@#b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid expression");

	// Purpose: Invalid Procedure
	
	src = "procure main{\nwhile !@#c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid procedure");*/
}

void ParserTest::testGetPKB(){
	/*Parser p;
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
	CPPUNIT_ASSERT_EQUAL(8,pkb->getProcedure(1)->getEndProgLine());

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
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "x");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "hhhhh");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "k");

	//Test Modifies While LOOP
	modifiesList = pkb->getModifies(WHILE, 4, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "x");
	CPPUNIT_ASSERT( *pkb->getVarName((modifiesListItr++->second)) == "hhhhh");

	//Test Uses
	USES_LIST useList = pkb->getUses(PROCEDURE, 1, 0);
	USES_LIST::iterator usesListItr = useList.begin();
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "y");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "x");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "b");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "a");

	//Test Uses WHILE LOOP
	useList = pkb->getUses(WHILE, 4, 0);
	usesListItr = useList.begin();
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "a");
	CPPUNIT_ASSERT( *pkb->getVarName((usesListItr++->second)) == "y");

	// Test AST
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	
	ast = ast->getFirstDescendant()->getFirstDescendant()->getRightSibling();
	CPPUNIT_ASSERT(pkb->getType(ast) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == WHILE);
	
	ast = ast->getRightSibling()->getRightSibling();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getRightSibling()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == ASSIGNMENT);

	ast = ast->getFirstDescendant()->getRightSibling()->getFirstDescendant();
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == WHILE);*/

}
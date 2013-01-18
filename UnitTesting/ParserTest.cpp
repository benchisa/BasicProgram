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
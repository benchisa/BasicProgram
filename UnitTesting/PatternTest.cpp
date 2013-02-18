#include "PatternTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Pattern.h>
#include <../SPA/Parser.h>
#include <../SPA/QueryPreprocessor.h>

void PatternTest::setUp()
{
}


void PatternTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PatternTest ); // Note 4 

void PatternTest::testEvaluatePattern(){
	Parser *p = new Parser();
	PKB *pkb;

	//create root AST
	AST* rootAST;
	AST* firstD;
	//pkb->insertVar("x");
	//CPPUNIT_ASSERT_EQUAL_MESSAGE("It did not pass", pkb->getVarIndex("x"), 2);
	//CPPUNIT_ASSERT_EQUAL_ASSERTION_FAIL_MESSAGE("It did not pass");
	//CPPUNIT_ASSERT(pkb->getVarIndex("x") == 1);
	/*pkb->insertVar("y");
	cout << pkb->getVarIndex("y");*/
	string src = "procedure test1{\n"
	/*2*/	"y = x;\n"
	/*3*/	"}";

	p->setSource(src);
	p->startParse();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("PKB did not parse successfully", 1, p->startParse());
		pkb = p->getPKB();
	QUERYTABLE *qTable = new QUERYTABLE();
	/*rootAST = pkb->createAST(ASSIGNMENT,1,1,-1);
	pkb->setRootAST(rootAST);
	
	firstD = pkb->createAST(VARIABLE,1,1,2);
	pkb->setFirstDescendant(rootAST,firstD);
	pkb->addSibling(firstD,pkb->createAST(VARIABLE,1,1,1));*/

	//p->setQuery("Select a pattern a(y, \"x\")");
	//Select a such that pattern a(y, "x"); 
	/*qTable->insert(pair<INDEX, TYPE>(ASSIGNMENT, QUERYVAR));
	qTable->insert(pair<INDEX, TYPE>(VARIABLE, QUERYVAR));
	qTable->insert(pair<INDEX, TYPE>(-1, ASSIGNMENT));
	qTable->insert(pair<INDEX, TYPE>(2, QUERYVAR));
	qTable->insert(pair<INDEX, TYPE>(1, VARIABLE));*/
	QueryPreprocessor *pre = new QueryPreprocessor(pkb);
	QUERY q = "assign a; Select a such that pattern a(y, \"x\")";
	pre->setQuery(q);
	pre->preProcess();
	//CPPUNIT_ASSERT_EQUAL(true, pre->preProcess());*/
	Pattern *myPattern = new Pattern(pkb);

	RELATION_LIST * result = new RELATION_LIST();
	//result->push_back(pair<STATEMENT_NUM, VAR_INDEX>(1, 3));
	//pre->getParamTable();
	//test ASSIGNMENT
	RELATION_LIST * list = myPattern->evaluatePattern(pre->getQVarTable());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("list does not match", list, result);
	
}
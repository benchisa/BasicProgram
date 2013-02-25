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
	Parser p;
	PKB * pkb = new PKB();

	//create root AST
	AST* rootAST;
	AST* firstD;
	//pkb->insertVar("x");
	//CPPUNIT_ASSERT_EQUAL_MESSAGE("It did not pass", pkb->getVarIndex("x"), 2);
	//CPPUNIT_ASSERT_EQUAL_ASSERTION_FAIL_MESSAGE("It did not pass");
	//CPPUNIT_ASSERT(pkb->getVarIndex("x") == 1);
	/*pkb->insertVar("y");
	cout << pkb->getVarIndex("y");*/
	SOURCE src = "procedure test1{\n" 
	       /*1*/   "y = x+z;}";

	p.setSource(src);
	p.startParse();
	//CPPUNIT_ASSERT_EQUAL_MESSAGE("PKB did not parse successfully", 1, p->startParse());
		pkb = p.getPKB();
	QueryPreprocessor *pre = new QueryPreprocessor(pkb);
	QUERY q = "assign a; variable v; Select a such that pattern a(v, \"x+y\");";
	pre->setQuery(q);
	pre->preProcess();
	//CPPUNIT_ASSERT_EQUAL(true, pre->preProcess());*/
	Pattern *myPattern = new Pattern(pkb);

	RELATION_LIST * result = new RELATION_LIST();
	//result->push_back(pair<STATEMENT_NUM, VAR_INDEX>(1, 3));
	//pre->getParamTable();
	//test ASSIGNMENT
	RELATION_LIST * list = myPattern->evaluatePattern(pre->getQTree(), pre->getQVarTable(), pre->getParamTable());
	delete pkb;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("list does not match", list, result);
	
}
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
	SOURCE src = "procedure test1{\n" 
	       /*1*/   "y = x+y;\n"
		   /*2*/   "z = x+y*w;}";

	p.setSource(src);
	p.startParse();
	//CPPUNIT_ASSERT_EQUAL_MESSAGE("PKB did not parse successfully", 1, p->startParse());
		pkb = p.getPKB();
	QueryPreprocessor *pre = new QueryPreprocessor(pkb);
	QUERY q;
	//q = "assign a; variable v; Select a such that pattern a(v, \"x+y\");";
	//q = "assign a; variable v; Select a such that pattern a(v, _\"x+y\"_)";
	//q = "assign a; variable v; Select a such that pattern a(v, _)";
	//q = "assign a; variable v; Select a such that pattern a(\"y\", _)";
	//q = "assign a; variable v; Select a such that pattern a(\"y\", \"x+y\")";
	//q = "assign a; variable v; Select a such that pattern a(\"y\", _\"x+y\"_)";
	//q = "assign a; variable v; Select a such that pattern a(_, \"x+y\")";
	q = "assign a; variable v; Select a such that pattern a(_, _\"x+y\"_)";
	pre->setQuery(q);
	pre->preProcess();
	Pattern *myPattern = new Pattern(pkb);
	//test ASSIGNMENT
	RELATION_LIST list;
	list = myPattern->evaluatePattern(pre->getQTree(), pre->getQVarTable(), pre->getParamTable());
	RELATION_LIST::iterator it;
	int result, expected;
	expected = 1;
	for(it=list.begin(); it != list.end(); it++) {
		result = (*it).first;
	}
	CPPUNIT_ASSERT_EQUAL_MESSAGE("list does not match", expected , result);

	/*q = "assign a; variable v; Select a such that pattern a(v, _\"x+y\"_);";
	pre->setQuery(q);
	pre->preProcess();
	//test ASSIGNMENT
	list = myPattern->evaluatePattern(pre->getQTree(), pre->getQVarTable(), pre->getParamTable());
	expected = 1;
	for(it=list.begin(); it != list.end(); it++) {
		result = (*it).first;
	}
	CPPUNIT_ASSERT_EQUAL_MESSAGE("list does not match", expected , result);*/
	//delete pkb;
	
}
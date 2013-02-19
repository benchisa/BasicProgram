#include "ParserPKBTest.h"
#include <cppunit/config/SourcePrefix.h>
#include "TestCases.h"
#include "../SPA/Parser.h"
#include "../SPA/PKB.h"

using namespace std;

void ParserPKBTest::setUp()
{
}


void ParserPKBTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( ParserPKBTest );

void ParserPKBTest::testSampleInput1(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource1);
	p.startParse();
	PKB *pkb = p.getPKB();

	// Test Follows Relationship
	CPPUNIT_ASSERT(pkb->isFollows(1, 2));
	CPPUNIT_ASSERT(pkb->isFollows(2, 3));
	CPPUNIT_ASSERT(!pkb->isFollows(1, 3));

	// Test Modifies relationship
	CPPUNIT_ASSERT(pkb->isModifies(ASSIGNMENT, 1, pkb->getVarIndex("a")));
	CPPUNIT_ASSERT(pkb->isModifies(ASSIGNMENT, 2, pkb->getVarIndex("c")));
	CPPUNIT_ASSERT(pkb->isModifies(ASSIGNMENT, 3, pkb->getVarIndex("d")));

	CPPUNIT_ASSERT(!pkb->isModifies(ASSIGNMENT, 2, pkb->getVarIndex("a")));
	CPPUNIT_ASSERT(!pkb->isModifies(ASSIGNMENT, 3, pkb->getVarIndex("x")));

	// Test Uses relationship
	CPPUNIT_ASSERT(pkb->isUses(ASSIGNMENT, 1, pkb->getVarIndex("b")));
	CPPUNIT_ASSERT(pkb->isUses(ASSIGNMENT, 2, pkb->getVarIndex("c")));
	CPPUNIT_ASSERT(pkb->isUses(ASSIGNMENT, 3, pkb->getVarIndex("x")));
	CPPUNIT_ASSERT(pkb->isUses(ASSIGNMENT, 3, pkb->getVarIndex("y")));
	CPPUNIT_ASSERT(pkb->isUses(ASSIGNMENT, 3, pkb->getVarIndex("z")));

	CPPUNIT_ASSERT(!pkb->isUses(ASSIGNMENT, 3, pkb->getVarIndex("a")));
	CPPUNIT_ASSERT(!pkb->isUses(ASSIGNMENT, 3, pkb->getVarIndex("b")));
}

void ParserPKBTest::testSampleInput2(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource2);
	p.startParse();
	PKB *pkb = p.getPKB();

	// Test Parent relationship
	CPPUNIT_ASSERT(!pkb->isParent(2,3));

	CPPUNIT_ASSERT(pkb->isParent(1,2));
	CPPUNIT_ASSERT(pkb->isParent(1,3));

	// Test Follows relationship
	CPPUNIT_ASSERT(pkb->isFollows(2, 3));

	CPPUNIT_ASSERT(!pkb->isFollows(1, 2));

	// Test Modifies for WHILE and PROCEDURE
	CPPUNIT_ASSERT(pkb->isModifies(WHILE, 1, pkb->getVarIndex("c")));
	CPPUNIT_ASSERT(pkb->isModifies(WHILE, 1, pkb->getVarIndex("d")));
	CPPUNIT_ASSERT(pkb->isModifies(PROCEDURE, pkb->getProcIndex("Code2"), pkb->getVarIndex("c")));
	CPPUNIT_ASSERT(pkb->isModifies(PROCEDURE, pkb->getProcIndex("Code2"), pkb->getVarIndex("d")));

	CPPUNIT_ASSERT(!pkb->isModifies(WHILE, 1, pkb->getVarIndex("x")));
	CPPUNIT_ASSERT(!pkb->isModifies(PROCEDURE, pkb->getProcIndex("Code2"), pkb->getVarIndex("x")));
	CPPUNIT_ASSERT(!pkb->isModifies(PROCEDURE, pkb->getProcIndex("Code3"), pkb->getVarIndex("d")));

	// Test Uses for WHILE and PROCEDURE
	CPPUNIT_ASSERT(pkb->isUses(WHILE, 1, pkb->getVarIndex("q")));
	CPPUNIT_ASSERT(pkb->isUses(WHILE, 1, pkb->getVarIndex("c")));
	CPPUNIT_ASSERT(pkb->isUses(WHILE, 1, pkb->getVarIndex("a")));
	CPPUNIT_ASSERT(pkb->isUses(PROCEDURE, pkb->getProcIndex("Code2"), pkb->getVarIndex("q")));
	CPPUNIT_ASSERT(pkb->isUses(PROCEDURE, pkb->getProcIndex("Code2"), pkb->getVarIndex("c")));
	
	CPPUNIT_ASSERT(!pkb->isUses(WHILE, 1, pkb->getVarIndex("d")));
	CPPUNIT_ASSERT(!pkb->isUses(PROCEDURE, pkb->getProcIndex("Code2"), pkb->getVarIndex("d")));
	CPPUNIT_ASSERT(!pkb->isUses(PROCEDURE, pkb->getProcIndex("Code3"), pkb->getVarIndex("c")));
}

void ParserPKBTest::testSampleInput3(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource3);
	p.startParse();
	PKB *pkb = p.getPKB();

	// Test for Follows relationship
	CPPUNIT_ASSERT(pkb->isFollows(1, 4));

	CPPUNIT_ASSERT(!pkb->isFollows(3, 4));
	CPPUNIT_ASSERT(!pkb->isFollows(4, 5));

	// Test for Parent relationship
	CPPUNIT_ASSERT(pkb->isParent(4, 5));
	CPPUNIT_ASSERT(!pkb->isParent(5, 6));

	// Test for Modifies relationship across the while loop
	CPPUNIT_ASSERT(pkb->isModifies(WHILE, 4, pkb->getVarIndex("y")));
	CPPUNIT_ASSERT(!pkb->isModifies(WHILE, 1, pkb->getVarIndex("y")));

	// Test for Uses relationship across the while loop
	CPPUNIT_ASSERT(pkb->isUses(WHILE, 4, pkb->getVarIndex("b")));
	CPPUNIT_ASSERT(!pkb->isUses(WHILE, 1, pkb->getVarIndex("b")));
}

void ParserPKBTest::testSampleInput4(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource4);
	p.startParse();
	PKB *pkb = p.getPKB();

	// Test for Follows
	CPPUNIT_ASSERT(pkb->isFollows(3, 4));
	
	CPPUNIT_ASSERT(!pkb->isFollows(3, 5));
	CPPUNIT_ASSERT(!pkb->isFollows(4, 5));

	// Test for Parent
	CPPUNIT_ASSERT(pkb->isParent(1, 4));

	CPPUNIT_ASSERT(!pkb->isParent(1, 5));
	CPPUNIT_ASSERT(!pkb->isParent(1, 6));

	// Test for Modifies (only for nested while)
	CPPUNIT_ASSERT(pkb->isModifies(WHILE, 1, pkb->getVarIndex("y")));
	CPPUNIT_ASSERT(pkb->isModifies(WHILE, 1, pkb->getVarIndex("b")));
	
	// Test for Uses (only for nest while)
	CPPUNIT_ASSERT(pkb->isUses(WHILE, 1, pkb->getVarIndex("b")));
	CPPUNIT_ASSERT(pkb->isUses(WHILE, 1, pkb->getVarIndex("q")));
}

void ParserPKBTest::testInvalidInput1(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleInvalidSource1);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError() == "Source Code Error(): missing bracket");
}

void ParserPKBTest::testInvalidInput2(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleInvalidSource2);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError() == "Source Code Error(): invalid/repeated procedure name");
}

void ParserPKBTest::testInvalidInput3(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleInvalidSource3);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError() == "Source Code Error(): invalid while var_name");
}

void ParserPKBTest::testInvalidInput4(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleInvalidSource4);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError() == "Source Code Error(): missing colon");
}

void ParserPKBTest::testInvalidInput5(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleInvalidSource5);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError() == "Source Code Error(): invalid procedure");
}
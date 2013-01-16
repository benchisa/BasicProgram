#include "QueryPreprocessorTest.h"
#include <cppunit/config/SourcePrefix.h>
#include "../SPA/QueryPreprocessor.h"
#include "../SPA/PKB.h"
#include "TestCases.h"
#include <iostream>
#include <hash_map>

void QueryPreprocessorTest::setUp()
{
}


void QueryPreprocessorTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryPreprocessorTest ); 


void QueryPreprocessorTest::testCorrectPreProcessing(){
	
	string qr;
	hash_map<TYPE,int>* qVarTable;
	hash_map<TYPE,int>::const_iterator qVarIter;
	//PKB *pkb = SamplePKB::pkb1();
	PKB *pkb = SamplePKB::pkb5();

	
	
	//THE FOLLOWING ARE TRUE TESTS============================
	
	/*
	qpp = new QueryPreprocessor(pkb);
	qr = "while w;Select w  pattern w(_,_);";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==ANY);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "while w;Select w  pattern w(\"a\",_);";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==VARIABLE);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a;Select a  pattern a(_,_\"y\"_);";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==ANY);	
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a;Select a  pattern a(\"d\",_\"y\"_);";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==VARIABLE);	
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a;Select a  pattern a(_,\"a\");";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==ASSIGNMENT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==ANY);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getType()==VARIABLE);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a;Select a  pattern a(\"y\",\"a\");";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==ASSIGNMENT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==VARIABLE);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getType()==VARIABLE);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a; select a pattern a(\"x\",_);";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==VARIABLE);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "while w; assign b; select w such that Parent(w,b);";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==2);
	qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==PARENT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==2);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getAncestor()->getType()==PARENT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getRightSibling()==NULL);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a,b,c;Select c such that Follows(a,b);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==3);
	qTree = qpp->getQTree();
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==3);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==FOLLOWS);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==2);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getAncestor()->getType()==FOLLOWS);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getRightSibling()==NULL);
		
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a,b,c;Select c such that Follows(a,b) pattern a(\"x\",_);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==3);		
	qTree = qpp->getQTree();
	CPPUNIT_ASSERT(qTree->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==SELECT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getData()==3);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getFirstDescendant()->getRightSibling()==NULL);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getType()==FOLLOWS);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getData()==1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==2);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getLeftSibling()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getAncestor()->getType()==FOLLOWS);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getAncestor()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getLeftSibling()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getLeftSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getAncestor()->getType()==PATTERN);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getAncestor()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getData()==1);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==VARIABLE);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==-1);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "stmt a,b,c;Select c such that Follows(a,b)";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==3);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a,b,c;Select c such that Follows*(a,b);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==3);
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a; variable b; Select a such that Modifies(a,\"b\");";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a; variable b; Select a such that Uses(a,\"b\");";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());

	//=====================================END OF TRUE TESTS

	//THE FOLLOWING ARE FALSE TESTS=========================
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a,b; Select a such that Parent(a,b);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());

	qpp = new QueryPreprocessor(pkb);
	qr = "assign a,b; Select a such that Parent*(a,b);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());

	qpp = new QueryPreprocessor(pkb);
	qr = "assign a; Select a such that Modifies(a,1);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());

	qpp = new QueryPreprocessor(pkb);
	qr = "assign a,b; Select b such that Follows(\"a\",b);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());

	qpp = new QueryPreprocessor(pkb);
	qr = "assign a,b; Select b such that Follows(\"a\",\"b\");";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());
		
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a; Select a such that Modifies(a,\"1\");";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a, b; Select a such that Uses(a,b);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());
	
	qpp = new QueryPreprocessor(pkb);
	qr = "assign a; while b; Select a such that Uses(a,b);";
	qpp->setQuery(qr);	
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());
	
	//===================================END OF FALSE TESTS
	*/
}

#include "QueryPreprocessorTest.h"
#include <cppunit/config/SourcePrefix.h>
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
	
	/*
	qpp = new QueryPreprocessor(pkb);
	qr = "constant c;Select boolean with c.value=10;";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	*/
	qpp = new QueryPreprocessor(pkb);
	qr = "constant c;variable v;Select boolean with c.value=v.varName;";
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	/*
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery1;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery2;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery3;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery4;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery5;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery6;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery7;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery8;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery9;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery10;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery11;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery12;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery13;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery14;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery15;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery16;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery17;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery18;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery19;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery20;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());		
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery21;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery22;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery23;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery24;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery25;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery26;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery27;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery28;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery29;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery30;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery31;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery32;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery33;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());		
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery34;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery35;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery36;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery37;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery28;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery39;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery40;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery41;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery42;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery43;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery44;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery45;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery46;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery47;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery48;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery49;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery51;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery52;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery53;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery54;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery55;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery56;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery57;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery58;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	

	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery59;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery60;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(false, qpp->preProcess());
	*/
	//THE FOLLOWING ARE TRUE TESTS============================
	
	
	
	/*
	qpp = new QueryPreprocessor(pkb);
	qr = SampleQueries::sampleQuery12;
	qpp->setQuery(qr);
	CPPUNIT_ASSERT_EQUAL(true, qpp->preProcess());	
	CPPUNIT_ASSERT(qpp->getQVarTable()->size()==1);
	QTREE* qTree = qpp->getQTree();	
	CPPUNIT_ASSERT(qTree->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getData()==0);	
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getType()==RESULT);
	CPPUNIT_ASSERT(qTree->getFirstDescendant()->getAncestor()->getData()==-1);		
	CPPUNIT_ASSERT(qTree->getRightSibling()->getType()==SUCHTHAT);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getData()==1);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getType()==FOLLOWST);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getData()==0);	
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData()==-1);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getLastDescendant()->getType()==ANY);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getLastDescendant()->getData()==-1);	
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getLastDescendant()->getLeftSibling()->getType()==QUERYVAR);
	CPPUNIT_ASSERT(qTree->getRightSibling()->getFirstDescendant()->getLastDescendant()->getLeftSibling()->getData()==0);

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

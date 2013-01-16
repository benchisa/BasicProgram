#include "QueryPreprocessorTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <iostream>

void QueryPreprocessorTest::setUp()
{
	qp = new QueryPreprocessor(pkb);
}


void QueryPreprocessorTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryPreprocessorTest ); 


void QueryPreprocessorTest::testCorrectPreProcessing(){
	
	string qr;
	
	qr = "while w; assign b; select w such that Parent(w,b);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	qr = "while w; assign b; select w such that Parent*(w,b);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	qr = "assign a,b,c;Select c such that Follows(a,b);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	
	qr = "assign a,b,c;Select c such that Follows*(a,b);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	
	qr = "assign a; variable v; Select a such that Modifies(a,v);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	
	qr = "assign a; variable v; Select a such that Uses(a,v);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	
	qr = "assign a; Select a pattern a(_,_);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	
	qr = "assign a; Select a such that Parent(_, a) pattern a(_,_);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());

	qr = "assign a; while w; Select a such that Follows(w, a) pattern a(_,_);";
	qp->setQuery(qr);
	
	CPPUNIT_ASSERT_EQUAL(true, qp->preProcess());
	// test error below
}

void QueryPreprocessorTest::testQTree(){
	string qr;
	
	qr = "while w; assign b; select w such that Parent(w,b);";
	qp->setQuery(qr);
	qp->preProcess();
	QTREE* tree = qp->getQTree();
	CPPUNIT_ASSERT(tree->getFirstDescendant()->getType() == RESULT);
	CPPUNIT_ASSERT(tree->getFirstDescendant()->getFirstDescendant()->getType() == QUERYVAR);
	CPPUNIT_ASSERT(tree->getFirstDescendant()->getFirstDescendant()->getData() == 1);

	
	QTREE* currentNode = tree->getFirstDescendant();
	CPPUNIT_ASSERT(currentNode->getRightSibling()->getType() == SUCHTHAT);
	CPPUNIT_ASSERT(currentNode->getRightSibling()->getFirstDescendant()->getType() == PARENT);
	CPPUNIT_ASSERT(currentNode->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getType() == QUERYVAR);
	CPPUNIT_ASSERT(currentNode->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getData() == 1);
	CPPUNIT_ASSERT(currentNode->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getType() == QUERYVAR);
	CPPUNIT_ASSERT(currentNode->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getData() == 2);
}

void QueryPreprocessorTest::testGetLastError(){
	string qr;
	
	qr = "asasign a select a such that Follows(a, b);";
	qp->setQuery(qr);
	if( qp->preProcess() == 0)
		CPPUNIT_ASSERT(qp->getLastError().compare("Query Error(): invalid entity(s)") == 0);

	
	qr = "assign; a; select a such that Follows(a, b);";
	qp->setQuery(qr);
	if( qp->preProcess() == 0)
		CPPUNIT_ASSERT(qp->getLastError().compare("Query Error(): invalid entity(s)") == 0);

	
	qr = "assign a; select a such that Followsasd(a, b);";
	qp->setQuery(qr);
	if( qp->preProcess() == 0)
		CPPUNIT_ASSERT(qp->getLastError().compare("Query Error(): invalid entity(s)") == 0);

	
	qr = "assign a; select aa such that Follows(a, b);";
	qp->setQuery(qr);
	if( qp->preProcess() == 0)
		CPPUNIT_ASSERT(qp->getLastError().compare("Query Error(): invalid entity(s)") == 0);

	qr = "assign a; select a such that Modify(a, b);";
	qp->setQuery(qr);
	if( qp->preProcess() == 0)
		CPPUNIT_ASSERT(qp->getLastError().compare("Query Error(): invalid entity(s)") == 0);

	
	qr = "assign a; select a such that Modifiesa, b);";
	qp->setQuery(qr);
	if( qp->preProcess() == 0)
		CPPUNIT_ASSERT(qp->getLastError().compare("Query Error(): invalid entity(s)") == 0);
}
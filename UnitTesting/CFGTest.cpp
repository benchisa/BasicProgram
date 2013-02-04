#include "CFGTest.h"
#include "../SPA/CFG.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( CFGTest );

void CFGTest::testCFG(){
	CFG* cfg = new CFG(5);
	cfg->addEdge(1,2);
	cfg->addEdge(2,3);
	cfg->addEdge(3,4);
	cfg->addEdge(4,5);
	cfg->addEdge(5,1);


	CPPUNIT_ASSERT(cfg->isNext(1,2));
	CPPUNIT_ASSERT(!cfg->isNext(3,2));
	CPPUNIT_ASSERT(!cfg->isNext(10,2));
	CPPUNIT_ASSERT(cfg->findAllPaths(1,0).size() == 5);
	CPPUNIT_ASSERT(cfg->findAllPaths(0,0).size() == 5);
	CPPUNIT_ASSERT(cfg->findAllPaths(0,5).size() == 5);
	CPPUNIT_ASSERT(cfg->findAllPaths(1,5).size() == 5);
	CPPUNIT_ASSERT(cfg->findAllPaths(2,5).size() == 5);
	CPPUNIT_ASSERT(cfg->findAllPaths(3,5).size() == 5);
	CPPUNIT_ASSERT(cfg->findAllPaths(5,5).size() == 5);
}
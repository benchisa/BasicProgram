#include "CFGTest.h"
#include "../SPA/CFG.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( CFGTest );

void CFGTest::setUp(){}
void CFGTest::tearDown(){}

void CFGTest::testCFG(){
	CFG *cfg2 = new CFG(6);
	cfg2->addEdge(1,2);
	cfg2->addEdge(2,3);
	cfg2->addEdge(2,4);
	cfg2->addEdge(3,5);
	cfg2->addEdge(4,5);
	cfg2->addEdge(5,1);

	// this is only the progline
	CPPUNIT_ASSERT(cfg2->getAllProgLines(1,0).size() == 5);
	CPPUNIT_ASSERT(cfg2->getAllProgLines(0,6).size() == 0);
	/*list<PROG_LINE> result = cfg2->getAllProgLines(2,3);
	cout << "Result: " << result.size() << "\n";
	list<PROG_LINE>::iterator itr = result.begin();
	while(itr!=result.end()){
		cout << "> " << *itr << "\n";
		itr++;
	}*/
	CPPUNIT_ASSERT(cfg2->getAllProgLines(2,3).size() == 1);

	// this is in Pairs
	CPPUNIT_ASSERT(cfg2->getAllPaths(1,0).size() == 6);
}
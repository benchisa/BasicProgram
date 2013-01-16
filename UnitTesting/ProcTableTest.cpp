#include "ProcTableTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/ProcTable.h>
#include <../SPA/Procedure.h>
void ProcTableTest::setUp()
{
}


void ProcTableTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ProcTableTest ); // Note 4 
//create a new procedure

ProcTable *myProcTable;
Procedure proc1("John",1);
Procedure proc2("Mary",32);
Procedure proc3("John",2);

// method to test the assigning and retrieval of grades
void ProcTableTest::testInsertProc(){
	// verify the insertion and return index is correct
	myProcTable = new ProcTable;

	proc1.setEndProgLine(30);
	proc2.setEndProgLine(45);

	CPPUNIT_ASSERT_EQUAL(1,myProcTable->insertProc(new Procedure(proc1)));
	CPPUNIT_ASSERT_EQUAL(2,myProcTable->insertProc(new Procedure(proc2)));
	//attempt to add a procedure with existing name
	CPPUNIT_ASSERT_EQUAL(1,myProcTable->insertProc(new Procedure(proc1)));
}

void ProcTableTest::testGetProcTableSize(){
	CPPUNIT_ASSERT_EQUAL(2,myProcTable->getProceTableSize());
}

void ProcTableTest::testGetProcedure(){
	//verify that the getProcedure(index) can return correct procedure
	
	/*CPPUNIT_ASSERT((*myProcTable->getProcedure(1)).getProcName()==proc1.getProcName());
	CPPUNIT_ASSERT((*myProcTable->getProcedure(2)).getProcName()==proc2.getProcName());
	//attempt to retrieve a name with invalid index
	CPPUNIT_ASSERT(myProcTable->getProcedure(3)==NULL);*/
	
}

void ProcTableTest::testGetProcIndex(){

	//verify that the getProcIndex(name" can return correct index
	/*CPPUNIT_ASSERT_EQUAL(1,myProcTable->getProcIndex("John"));
	CPPUNIT_ASSERT_EQUAL(2,myProcTable->getProcIndex("Mary"));
	//attempt to retrieve the index with invalid name
	CPPUNIT_ASSERT_EQUAL(NULL,myProcTable->getProcIndex("Jane"));*/
	
}
void ProcTableTest::testIsExists(){

	/*CPPUNIT_ASSERT(myProcTable->isExists("John")==true);
	CPPUNIT_ASSERT(myProcTable->isExists("Mary")==true);
	CPPUNIT_ASSERT(myProcTable->isExists("Jane")==false);*/
	
}
void ProcTableTest::testGetAllProc(){
	
	//Sample name list of procedures
	/*list<string> procList1,procList2;
	procList1.push_back(proc1.getProcName());
	procList1.push_back(proc2.getProcName());

	//get procedure list from program method getAllProc()
	list<Procedure>::iterator itr;
	list<Procedure> list = *myProcTable->getAllProc();
	//convert procedure list ot name list
	for(itr=list.begin();itr!=list.end();itr++){
		procList2.push_back(itr->getProcName());
	}
	//verify two name lists are same
   CPPUNIT_ASSERT(procList1==procList2);*/
   
	
}

void ProcTableTest::testIsInSameProc(){
	/*
	//test 2 stmts in proc1
	CPPUNIT_ASSERT(myProcTable.isInSameProc(1,30)==true);
	//test 2 stmts in proc2
	CPPUNIT_ASSERT(myProcTable.isInSameProc(32,45)==true);
	//boundry stmts: one is end of proc1, the other one is start of proc2
	CPPUNIT_ASSERT(myProcTable.isInSameProc(30,32)==false);
	//boundry stmts: one is valid, the other is invalid
	CPPUNIT_ASSERT(myProcTable.isInSameProc(31,32)==false);
	CPPUNIT_ASSERT(myProcTable.isInSameProc(45,46)==false);
	CPPUNIT_ASSERT(myProcTable.isInSameProc(0,1)==false);
	CPPUNIT_ASSERT(myProcTable.isInSameProc(30,31)==false);
	//invalid stmts
	CPPUNIT_ASSERT(myProcTable.isInSameProc(-2,-1)==false);
	CPPUNIT_ASSERT(myProcTable.isInSameProc(46,47)==false);*/
}
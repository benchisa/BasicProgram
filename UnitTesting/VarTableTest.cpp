#include "VarTableTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/VarTable.h>
#include <iostream>

void VarTableTest::setUp()
{
}


void VarTableTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( VarTableTest ); // Note 4 

// create a new VarTable

VarTable* myVarTable  ;
// initialize a few indexes
int xIndex = 1 ;
int yIndex = 2 ;

// method to test the assigning and retrieval of grades
void VarTableTest::testInsertVar() {

	// verify that the insertion and return index is correct
	myVarTable = new VarTable;
	CPPUNIT_ASSERT_EQUAL(xIndex,myVarTable->insertVar("x"));
	CPPUNIT_ASSERT_EQUAL(yIndex,myVarTable->insertVar("y"));
	//attempt to add an existing variable
	CPPUNIT_ASSERT_EQUAL(2,myVarTable->insertVar("y"));

}
void VarTableTest::testGetVarTableSize(){
	/*
	CPPUNIT_ASSERT_EQUAL(2, myVarTable.getVarTableSize());*/
}

void VarTableTest::testGetVarName(){
	//verify that the getVarName(index) can return correct name
	/*	
	CPPUNIT_ASSERT(*myVarTable.getVarName(1)=="x");
	CPPUNIT_ASSERT(*myVarTable.getVarName(2)=="y");
	//attempt to retrieve a name with invalid index
	CPPUNIT_ASSERT(myVarTable.getVarName(3)==NULL);*/

}
void VarTableTest::testGetVarIndex(){
	/*
	//verify that the getVarIndex(varName) can return correct index
	CPPUNIT_ASSERT_EQUAL(xIndex,myVarTable.getVarIndex("x"));
	CPPUNIT_ASSERT_EQUAL(yIndex,myVarTable.getVarIndex("y"));
	//attempt to retrieve an index with invalid name
	CPPUNIT_ASSERT_EQUAL(NULL,myVarTable.getVarIndex("z"));*/
}

void VarTableTest::testIsExists(){
	/*
	//verify that the isExists(varName) can return correct boolean value
	CPPUNIT_ASSERT(myVarTable.isExists("x")==true);
	CPPUNIT_ASSERT(myVarTable.isExists("y")==true);
	//attempt to check a not exist value "z"
	CPPUNIT_ASSERT(myVarTable.isExists("z")==false);*/


}
void VarTableTest::testGetAllVar(){
	/*list<string> varList;

	varList.push_back("x");
	varList.push_back("y");
	CPPUNIT_ASSERT(*myVarTable.getAllVar()==varList);*/
}

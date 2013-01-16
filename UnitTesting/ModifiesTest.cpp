#include "ModifiesTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Modifies.h>
#include <iostream>
using namespace std;
void ModifiesTest::setUp()
{
}


void ModifiesTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( ModifiesTest );



void ModifiesTest::testInsertModifies() {
	Modifies f;
// verify that the insertion and return index is correct
	CPPUNIT_ASSERT(f.insertModifies(WHILE,1,2));
	CPPUNIT_ASSERT(f.insertModifies(WHILE,1,4));
	CPPUNIT_ASSERT(f.insertModifies(WHILE,2,3));
	CPPUNIT_ASSERT(f.insertModifies(WHILE,1,2)==false);
	
}
void ModifiesTest::testIsModifies() {

// verify that the insertion and return index is correct
	Modifies f;

	f.insertModifies(WHILE,10,11);
	f.insertModifies(WHILE,12,13);
	CPPUNIT_ASSERT(f.isModifies(WHILE,10,11));
	CPPUNIT_ASSERT(f.isModifies(WHILE,12,13));
	CPPUNIT_ASSERT(f.isModifies(WHILE,10,14)==false);




}
void ModifiesTest::testGetModifies()
{  Modifies f;
	
	f.insertModifies(WHILE,100,101);
	f.insertModifies(WHILE,101,102);
	MODIFIES_LIST mlists = f.getModifies(WHILE, 100,NULL);

	MODIFIES_LIST::iterator mitr=mlists.begin();
	CPPUNIT_ASSERT_EQUAL(101, mitr->second);
	

	MODIFIES_LIST list2 = f.getModifies(WHILE,NULL,102);
	MODIFIES_LIST::iterator mitr2=list2.begin();
	CPPUNIT_ASSERT_EQUAL(101,mitr2->first);

	
	MODIFIES_LIST list3 = f.getModifies(WHILE,NULL,NULL);
	MODIFIES_LIST::iterator mitr3=list3.begin();

	int size = list3.size();
	CPPUNIT_ASSERT_EQUAL(2,size);

	CPPUNIT_ASSERT_EQUAL(100,mitr3->first);
	CPPUNIT_ASSERT_EQUAL(101,mitr3->second);
	//cout<<"first:"<<mitr3->first;
	//cout<<"second:"<<mitr3->second;
	mitr3++;
	//cout<<"first:"<<mitr3->first;
	//cout<<"second:"<<mitr3->second;
	CPPUNIT_ASSERT_EQUAL(101,mitr3->first);
	CPPUNIT_ASSERT_EQUAL(102,mitr3->second);

/*
	for(mitr3 = list3.begin();mitr3!=list3.end();mitr3++){
		cout<<"first:"<<mitr3->first;
		cout<<"second:"<<mitr3->second;
	}*/
	
}

#include "UsesTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Uses.h>
#include <iostream>
using namespace std;
void UsesTest::setUp()
{
}


void UsesTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( UsesTest );



void UsesTest::testInsertUses() {
	Uses f;
// verify that the insertion and return index is correct
	CPPUNIT_ASSERT(f.insertUses(WHILE,1,2));
	CPPUNIT_ASSERT(f.insertUses(WHILE,1,4));
	CPPUNIT_ASSERT(f.insertUses(WHILE,2,3));
	CPPUNIT_ASSERT(f.insertUses(WHILE,1,2)==false);

	
}
void UsesTest::testIsUses() {
	Uses f;
// verify that the insertion and return index is correct
	f.insertUses(WHILE,10,11);
	f.insertUses(WHILE,12,13);
	CPPUNIT_ASSERT(f.isUses(WHILE,10,11));
	CPPUNIT_ASSERT(f.isUses(WHILE,12,13));
	CPPUNIT_ASSERT(f.isUses(WHILE,10,14)==false);

}
void UsesTest::testGetUses()
{  Uses f;
	
	f.insertUses(WHILE,100,101);
	f.insertUses(WHILE,100,102);
	USES_LIST mlists = f.getUses(WHILE, 100,NULL);

	USES_LIST::iterator mitr=mlists.begin();
	CPPUNIT_ASSERT_EQUAL(101, mitr->second);
	*mitr++;
	CPPUNIT_ASSERT_EQUAL(102, mitr->second);

	USES_LIST list2 = f.getUses(WHILE,NULL,101);
	MODIFIES_LIST::iterator mitr2=list2.begin();
	CPPUNIT_ASSERT_EQUAL(100,mitr2->first);

	
	MODIFIES_LIST list3 = f.getUses(WHILE,NULL,NULL);
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
	CPPUNIT_ASSERT_EQUAL(100,mitr3->first);
	CPPUNIT_ASSERT_EQUAL(102,mitr3->second);

}

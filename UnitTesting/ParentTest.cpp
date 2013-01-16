#include "ParentTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Parent.h>
#include <iostream>
using namespace std;
void ParentTest::setUp()
{
}


void ParentTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( ParentTest );



void ParentTest::testInsertParent() {
	Parent f;
// verify that the insertion and return index is correct

	CPPUNIT_ASSERT(f.insertParent(1,2)==true);
	CPPUNIT_ASSERT(f.insertParent(1,4)==true);
	CPPUNIT_ASSERT(f.insertParent(2,3)==true);
	CPPUNIT_ASSERT(f.insertParent(1,2)==false);

	
	
}
void ParentTest::testIsParent() {
	Parent f;
// verify that the insertion and return index is correct
	f.insertParent(10,11);
	f.insertParent(12,13);

	CPPUNIT_ASSERT(f.isParent(10,11)==true);
	CPPUNIT_ASSERT(f.isParent(12,13)==true);
	CPPUNIT_ASSERT(f.isParent(10,14)==false);


}
void ParentTest::testGetParent()
{   Parent f;
	f.insertParent(100,101);
	f.insertParent(100,102);
	f.insertParent(102,103);

	PARENT_LIST mlists = f.getParent(100,NULL);

	FOLLOWS_LIST::iterator mitr=mlists.begin();
	CPPUNIT_ASSERT_EQUAL(101, mitr->second);
	mitr++;
	CPPUNIT_ASSERT_EQUAL(102, mitr->second);

	PARENT_LIST list2 = f.getParent(NULL,103);
	FOLLOWS_LIST::iterator mitr2=list2.begin();
	CPPUNIT_ASSERT_EQUAL(102,mitr2->first);

	
	PARENT_LIST list3 = f.getParent(NULL,NULL);
	PARENT_LIST::iterator mitr3=list3.begin();

	int size = list3.size();
	CPPUNIT_ASSERT_EQUAL(3,size);

	CPPUNIT_ASSERT_EQUAL(100,mitr3->first);
	CPPUNIT_ASSERT_EQUAL(101,mitr3->second);
	//cout<<"first:"<<mitr3->first;
	//cout<<"second:"<<mitr3->second;
	mitr3++;
	//cout<<"first:"<<mitr3->first;
	//cout<<"second:"<<mitr3->second;
	CPPUNIT_ASSERT_EQUAL(100,mitr3->first);
	CPPUNIT_ASSERT_EQUAL(102,mitr3->second);
	mitr3++;
	CPPUNIT_ASSERT_EQUAL(102,mitr3->first);
	CPPUNIT_ASSERT_EQUAL(103,mitr3->second);
	
}

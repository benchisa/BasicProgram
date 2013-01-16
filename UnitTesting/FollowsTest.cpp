#include "FollowsTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Follows.h>
#include <iostream>
using namespace std;
void FollowsTest::setUp()
{
}


void FollowsTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( FollowsTest );



void FollowsTest::testInsertFollows() {
	Follows f;
// verify that the insertion and return index is correct

	
	CPPUNIT_ASSERT(f.insertFollows(1,2)==true);
	CPPUNIT_ASSERT(f.insertFollows(2,3)==true);
	CPPUNIT_ASSERT(f.insertFollows(1,2)==false);


	
}
void FollowsTest::testIsFollows() {
	Follows f;
// verify that the insertion and return index is correct
	f.insertFollows(10,11);
	f.insertFollows(12,13);

	
	CPPUNIT_ASSERT(f.isFollows(10,11)==true);
	CPPUNIT_ASSERT(f.isFollows(12,13)==true);
	CPPUNIT_ASSERT(f.isFollows(10,14)==false);


}
void FollowsTest::testGetFollows()
{
    Follows f;	

	f.insertFollows(100,101);
	f.insertFollows(101,102);
	FOLLOWS_LIST mlists = f.getFollows(100,NULL);

	FOLLOWS_LIST::iterator mitr=mlists.begin();
	CPPUNIT_ASSERT_EQUAL(101, mitr->second);
	
	FOLLOWS_LIST list2 = f.getFollows(NULL,102);
	FOLLOWS_LIST::iterator mitr2=list2.begin();
	CPPUNIT_ASSERT_EQUAL(101,mitr2->first);

	
	FOLLOWS_LIST list3 = f.getFollows(NULL,NULL);
	FOLLOWS_LIST::iterator mitr3=list3.begin();

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
	
}

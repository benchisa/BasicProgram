#include "CallTableTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/CallTable.h>
#include <iostream>
using namespace std;

void CallTableTest::setUp()
{

}

void CallTableTest::tearDown()
{

}

void CallTableTest::testInsertCall()
{
  CallTable c;
	CPPUNIT_ASSERT_EQUAL(1,c.insertCall("1","2"));
	CPPUNIT_ASSERT_EQUAL(2,c.insertCall("1","2"));
	CPPUNIT_ASSERT_EQUAL(3,c.insertCall("1","3"));
	CPPUNIT_ASSERT_EQUAL(4,c.insertCall("2","3"));

}

void CallTableTest::testIsCallExist()
{
	CallTable c;
	c.insertCall("1","2");
	c.insertCall("1","3");
	CPPUNIT_ASSERT(c.isExistsCall("1","2")==true);
	CPPUNIT_ASSERT(c.isExistsCall("0","2")==false);
}

void CallTableTest::testGetCall()
{
	CallTable c;
	c.insertCall("1","2");
	c.insertCall("1","3");
	c.insertCall("6","7");
	c.insertCall("8","2");
	/*CALL_LIST cl=c.getCall("1"," ");

	CALL_LIST::iterator c_itr=cl.begin();
	CPPUNIT_ASSERT(c_itr->second=="2");
	c_itr++;
	CPPUNIT_ASSERT(c_itr->second=="3");

	cl=c.getCall(" ","2");
	c_itr=cl.begin();
	CPPUNIT_ASSERT(c_itr->first=="1");
	c_itr++;
	CPPUNIT_ASSERT(c_itr->first=="8");
      */

}	

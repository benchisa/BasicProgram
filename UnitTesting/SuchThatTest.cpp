#include "SuchThatTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/SuchThat.h>
#include <../SPA/Parser.h>
#include <iostream>
using namespace std;

Parser p;
PKB *SuchThat::pkb;

void SuchThatTest::setUp()
{
	string src = "procedure main{\n"
		/*1*/		 "while a{\n"
		/*2*/		 "b = c + 1;\n}"
		/*3*/		 "while b{\n"
		/*4*/		 "thisVar = thisVar + a;\n"
		/*5*/		 "a = x + y;}\n"
		/*6*/		 "y = a;\n"
		/*7*/		 "z = 5;\n}";
	p.setSource(src);
	p.startParse();
	if(p.getLastError() != "")
		cout << p.getLastError() << "\n";
	SuchThat::pkb = p.getPKB();
}


void SuchThatTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( SuchThatTest );


void SuchThatTest::testGetParentResult(){
	// Parent(1, a), return <1, 2>
	PARENT_LIST result1 = SuchThat::getParentResult(ASSIGNMENT, 1, 0); 
	PARENT_LIST::iterator itr = result1.begin();
	
	CPPUNIT_ASSERT(itr->first==1);
	CPPUNIT_ASSERT(itr->second==2);

	// Parent(1, w), return empty set
	PARENT_LIST result2 = SuchThat::getParentResult(WHILE, 1, 0);
	CPPUNIT_ASSERT(result2.empty()==true);
	
	// Parent(w, 2), return <1, 2>
	PARENT_LIST result3 = SuchThat::getParentResult(WHILE, 0, 2);
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 2);

	// Parent(s1, s2), return <1, 2>, <3, 4>, <3, 5>
	PARENT_LIST result4 = SuchThat::getParentResult(STATEMENT, STATEMENT);
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 2);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 4);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 5);

	// Parent(_, s2) == Parent(s1, _) == Parent(s1, s2) == Parent(_, _)
	PARENT_LIST result5 = SuchThat::getParentResult(ANY, STATEMENT);
	CPPUNIT_ASSERT(result5.size() == 3);
	PARENT_LIST result6 = SuchThat::getParentResult(STATEMENT, ANY);
	CPPUNIT_ASSERT(result6.size() == 3);
	PARENT_LIST result11 = SuchThat::getParentResult(STATEMENT, ANY);
	CPPUNIT_ASSERT(result11.size() == 3);
	
	// Parent(_, a), return 3 results
	PARENT_LIST result7 = SuchThat::getParentResult(ANY, ASSIGNMENT);
	CPPUNIT_ASSERT(result7.size() == 3);

	// Parent(_, w), return 0 results
	PARENT_LIST result8 = SuchThat::getParentResult(ANY, WHILE);
	CPPUNIT_ASSERT(result8.size() == 0);

	// Parent(w, _), return 3 results
	PARENT_LIST result9 = SuchThat::getParentResult(WHILE, ANY);
	CPPUNIT_ASSERT(result9.size() == 3);

	// Parent(a, _), return 0
	PARENT_LIST result10 = SuchThat::getParentResult(ASSIGNMENT, ANY);
	CPPUNIT_ASSERT(result10.size() == 0);
	
	// Parent(_, 2), return 1 
	PARENT_LIST result12 = SuchThat::getParentResult(ANY, 0, 2);
	CPPUNIT_ASSERT(result12.size() == 1);

	// Parent(3, _), return 2
	PARENT_LIST result13 = SuchThat::getParentResult(ANY, 3, 0);
	CPPUNIT_ASSERT(result13.size() == 2);
}

void SuchThatTest::testGetParentStarResult()
{
	// Parent(s1, s2), return <1, 2>, <3, 4>, <3, 5>
	PARENT_LIST result1 = SuchThat::getParentStarResult(STATEMENT, STATEMENT);
	PARENT_LIST::iterator itr = result1.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 2);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 4);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 5);

	// Parent(3, a), return <3, 4>, <3,5>
	PARENT_LIST result2 = SuchThat::getParentResult(ASSIGNMENT, 3, 0); 
	 itr = result2.begin();
	
	CPPUNIT_ASSERT(itr->first==3);
	CPPUNIT_ASSERT(itr->second==4);
	itr++;
	CPPUNIT_ASSERT(itr->first==3);
	CPPUNIT_ASSERT(itr->second==5);

}
void SuchThatTest::testGetIsParentResult(){
	// Parent(1, 2),return true
	bool result1 = SuchThat::getIsParentResult(1, 2);
	CPPUNIT_ASSERT(result1 == true);

	// Parent(2, 3) return false
	bool result2 = SuchThat::getIsParentResult(2, 3);
	CPPUNIT_ASSERT(result2 == false);
}

void SuchThatTest::testGetFollowsResult(){
	//Follow(1, a), return empty set
	FOLLOWS_LIST result1 = SuchThat::getFollowsResult(ASSIGNMENT, 1, 0);
	CPPUNIT_ASSERT(result1.empty() == true);

	//Follow(1, w), return <1, 3>
	FOLLOWS_LIST result2 = SuchThat::getFollowsResult(WHILE, 1, 0);
	FOLLOWS_LIST::iterator itr = result2.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);

	//Follow(s1, s2), return <1, 3>, <3, 6>, <4, 5>, <6, 7>
	FOLLOWS_LIST result3 = SuchThat::getFollowsResult(STATEMENT, STATEMENT);
	itr = result3.begin();
	
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);
	itr++;
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == 5);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 6);
	itr++;
	CPPUNIT_ASSERT(itr->first == 6);
	CPPUNIT_ASSERT(itr->second == 7);

	//Follow(w, a), return <3, 6>
	FOLLOWS_LIST result4 = SuchThat::getFollowsResult(WHILE, ASSIGNMENT);
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 6);
	
	//Follow(a, a) return <4, 5>, <6, 7>
	FOLLOWS_LIST result5 = SuchThat::getFollowsResult(ASSIGNMENT, ASSIGNMENT);
	itr = result5.begin();
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == 5);
	itr++;
	CPPUNIT_ASSERT(itr->first == 6);
	CPPUNIT_ASSERT(itr->second == 7);
	
	//Follow(w, w), return <1, 3>
	FOLLOWS_LIST result6 = SuchThat::getFollowsResult(WHILE, WHILE);
	itr = result6.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);

	//Follow(w, 3), return <1, 3>
	FOLLOWS_LIST result7 = SuchThat::getFollowsResult(WHILE, 0, 3);
	itr = result7.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);

	//Follow(a, 6), return empty set
	FOLLOWS_LIST result8 = SuchThat::getFollowsResult(ASSIGNMENT, 0, 6);
	CPPUNIT_ASSERT(result8.empty()==true);

	// Follows(_, s2) == Follows(s1, _) == Follows(s1, s2) == Follows(_, _)
	FOLLOWS_LIST result9 = SuchThat::getFollowsResult(ANY, STATEMENT);
	FOLLOWS_LIST result10 = SuchThat::getFollowsResult(STATEMENT, ANY);
	FOLLOWS_LIST result11 = SuchThat::getFollowsResult(ANY, ANY);
	
	CPPUNIT_ASSERT(result9.size() == 4);
	CPPUNIT_ASSERT(result10.size() == 4);
	CPPUNIT_ASSERT(result11.size() == 4);

	// Follows(_, a), return 3
	FOLLOWS_LIST result12 = SuchThat::getFollowsResult(ANY, ASSIGNMENT);
	CPPUNIT_ASSERT(result12.size() == 3);

	// Follows(a, _), return 2
	FOLLOWS_LIST result13 = SuchThat::getFollowsResult(ASSIGNMENT, ANY);
	CPPUNIT_ASSERT(result13.size() == 2);

	//Follows(_, 7), return 1
	FOLLOWS_LIST result14 = SuchThat::getFollowsResult(ANY, 0, 7);
	CPPUNIT_ASSERT(result14.size() == 1);

	//Follows(4, _), return 1
	FOLLOWS_LIST result15 = SuchThat::getFollowsResult(ANY, 4, 0);
	CPPUNIT_ASSERT(result15.size() == 1);
	
	//Follows(7, _), return 0
	FOLLOWS_LIST result16 = SuchThat::getFollowsResult(ANY, 7, 0);
	CPPUNIT_ASSERT(result16.size() == 0);
	
}
void SuchThatTest::testGetFollowsStarResult()
{
	//Follow(s1, s2), return <1, 3>,<1, 6>, <1,7>,  <3, 6>, <4, 5>
	FOLLOWS_LIST result1 = SuchThat::getFollowsStarResult(STATEMENT, STATEMENT);
	FOLLOWS_LIST::iterator itr = result1.begin();
	
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);
	itr++;
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 6);
	itr++;
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 7);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 6);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 7);
	itr++;
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == 5);

	
	//Follow(w, a), return <1,6>, <1,7>,  <3, 6>, <3,7>
	FOLLOWS_LIST result2 = SuchThat::getFollowsStarResult(WHILE, ASSIGNMENT);
	itr = result2.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 6);
	itr++;
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 7);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 6);
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 7);

	//Follow(a, a) return <4, 5>, <6,7>
	FOLLOWS_LIST result3 = SuchThat::getFollowsStarResult(ASSIGNMENT, ASSIGNMENT);
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == 5);
	itr++;
	CPPUNIT_ASSERT(itr->first == 6);
	CPPUNIT_ASSERT(itr->second == 7);

	//Follow(w, w), return <1, 3>
	FOLLOWS_LIST result4 = SuchThat::getFollowsStarResult(WHILE, WHILE);
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);
	

	//Follow(w, 4), return none
	 FOLLOWS_LIST result5 = SuchThat::getFollowsStarResult(WHILE, 0, 4);
	 CPPUNIT_ASSERT(result5.size()==0);
	
	 //Follow(w, 7), return none
	 FOLLOWS_LIST result6 = SuchThat::getFollowsStarResult(WHILE, 0, 7);
	 itr=result6.begin();
	// cout << itr->first << "\n";
	 CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 7);
	itr++;
	 CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 7);
}

void SuchThatTest::testGetIsFollowsResult(){
	//Follows(1, 3), return true
	bool result1 = SuchThat::getIsFollowsResult(1, 3);
	CPPUNIT_ASSERT(result1==true);

	bool result2 = SuchThat::getIsFollowsResult(4, 5);
	CPPUNIT_ASSERT(result2==true);
	
	//Follows(2, 3), return false
	bool result3 = SuchThat::getIsFollowsResult(2, 3);
	CPPUNIT_ASSERT(result3==false);
}

void SuchThatTest::testGetModifiesResult(){
	//Modifies(1, v), return <1, getVarIndex("b")>
	MODIFIES_LIST result1 = SuchThat::getModifiesResult(STATEMENT, 1, 0);
	MODIFIES_LIST::iterator itr = result1.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));

	// Modifies(w, "b"), Modifies (w, "thisVar")
	//  <3, getVarIndex("thisVar")>
	MODIFIES_LIST result2 = SuchThat::getModifiesResult(WHILE, 0, SuchThat::pkb->getVarIndex("thisVar"));
	itr = result2.begin();
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));

	// <1, getVarIndex("b")>
	MODIFIES_LIST result3 = SuchThat::getModifiesResult(WHILE, 0, SuchThat::pkb->getVarIndex("b"));
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));

	//Modifies(p, "b"), return <getProcIndex("main"), getVarIndex("b")>
	MODIFIES_LIST result4 = SuchThat::getModifiesResult(PROCEDURE, 0, SuchThat::pkb->getVarIndex("b"));
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));

	//Modifies(p, v), Modifies("main", v), both should return same output
	//<1, "b">, <1, "b">, <1, "thisVar"> <1, "a",> <1, "thisVar">, <1, "a">, <1, "y">
	MODIFIES_LIST result5 = SuchThat::getModifiesResult(PROCEDURE, 0, 0);
	itr = result5.begin();
	// will return in sorting order
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("y"));
	
	MODIFIES_LIST result6 = SuchThat::getModifiesResult(PROCEDURE, SuchThat::pkb->getProcIndex("main"), 0);
	itr = result6.begin();
	// will return in sorting order
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == SuchThat::pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("y"));

	//Modifies(a, v), return <2, "b">, <4, "thisVar">, <5, "a">, <6, "y">
	MODIFIES_LIST result7 = SuchThat::getModifiesResult(ASSIGNMENT, 0, 0);
	itr = result7.begin();
	// will return in sorting order
	CPPUNIT_ASSERT(itr->first == 2);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 4 );
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 5 );
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 6 );
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("y"));


	//Modifies(a, "y"), return <6, "y">
	MODIFIES_LIST result8 = SuchThat::getModifiesResult(ASSIGNMENT, 0, SuchThat::pkb->getVarIndex("y"));
	itr = result8.begin();
	CPPUNIT_ASSERT(itr->first == 6 );
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("y"));

	// Modifies(_, _), return non-empty result
	MODIFIES_LIST result9 = SuchThat::getModifiesResult(ANY, 0, 0);
	CPPUNIT_ASSERT(result9.empty() == false);

	
	// Modifies(_, "a"), return non-empty result
	MODIFIES_LIST result10 = SuchThat::getModifiesResult(ANY, 0, SuchThat::pkb->getVarIndex("a"));
	CPPUNIT_ASSERT(result10.empty() == false);

	// Modifies(1, _) return size of 1
	MODIFIES_LIST result11 = SuchThat::getModifiesResult(ANY, 1, 0);
	CPPUNIT_ASSERT(result11.size() == 1);

}

void SuchThatTest::testGetIsModifiesResult(){
	// Modifies(1, "b"), return true
	bool result1 = SuchThat::getIsModifiesResult(STATEMENT, 1, SuchThat::pkb->getVarIndex("b"));
	CPPUNIT_ASSERT(result1 == true);

	// Modifies("main", "b"), return true
	bool result2 = SuchThat::getIsModifiesResult(PROCEDURE, SuchThat::pkb->getProcIndex("main"), SuchThat::pkb->getVarIndex("b"));
	CPPUNIT_ASSERT(result2 == true);

	// Modifies(3, "b"), return false
	bool result3 = SuchThat::getIsModifiesResult(STATEMENT, 3, SuchThat::pkb->getVarIndex("b"));
	CPPUNIT_ASSERT(result3 == false);
}

void SuchThatTest::testGetUsesResult(){
	//Uses(p, v), return <1, "a">, <1, "c">, <1, "thisVar"> <1, "x"> <1, "y">
	USES_LIST result1 = SuchThat::getUsesResult(PROCEDURE, 0, 0);
	USES_LIST::iterator itr = result1.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("c"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("x"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("y"));

	USES_LIST result2 = SuchThat::getUsesResult(PROCEDURE, SuchThat::pkb->getProcIndex("main"), 0);
	itr = result2.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("c"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("x"));
	itr++;
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("y"));

	//Uses(1, v), return <1, "a">, <1, "c">,
	USES_LIST result3 = SuchThat::getUsesResult(STATEMENT, 1, 0);
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("c"));

	//Uses(w, v), return <1, "a">, <1, "c">,<3, "b">, <3, "thisVar">, <3, "a">, <3, "x">, <3, "y">
	USES_LIST result4 = SuchThat::getUsesResult(WHILE, 0, 0);
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("c"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("x"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("y"));

	//Uses(w, "a"), return <1, "a">,  <3, "a">,
	USES_LIST result5 = SuchThat::getUsesResult(WHILE, 0, SuchThat::pkb->getVarIndex("a"));
	itr = result5.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("a"));

	//Uses(a, "thisVar"), return <4, "thisVar">
	USES_LIST result6 = SuchThat::getUsesResult(ASSIGNMENT, 0, SuchThat::pkb->getVarIndex("thisVar"));
	itr = result6.begin();
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == SuchThat::pkb->getVarIndex("thisVar"));

	//Uses(_, _), return not empty
	USES_LIST result7 = SuchThat::getUsesResult(ANY, 0, 0);
	CPPUNIT_ASSERT(result7.empty() == false);

	//Uses(_, "x") return not empty
	USES_LIST result8 = SuchThat::getUsesResult(ANY, 0, SuchThat::pkb->getVarIndex("x"));
	CPPUNIT_ASSERT(result8.empty() == false);

	//Uses(1, _) 
	USES_LIST result9 = SuchThat::getUsesResult(ANY, 1, 0);
	CPPUNIT_ASSERT(result9.size() == 2);
	
	
}

void SuchThatTest::testGetIsUsesResult(){
	// Uses("main", "a"), return true
	bool result1 = SuchThat::getIsUsesResult(PROCEDURE, SuchThat::pkb->getProcIndex("main"), SuchThat::pkb->getVarIndex("a"));
	CPPUNIT_ASSERT(result1 == true);

	// Uses(3, "a") return true
	bool result2 = SuchThat::getIsUsesResult(STATEMENT, 3, SuchThat::pkb->getVarIndex("a"));
	CPPUNIT_ASSERT(result2 == true);

	// Uses(4, "xyz") return false
	bool result3 = SuchThat::getIsUsesResult(STATEMENT, 4, SuchThat::pkb->getVarIndex("xyz"));
	CPPUNIT_ASSERT(result3 == false);
}
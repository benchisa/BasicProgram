#include "DesignExtractorTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/DesignExtractor.h>
#include <../SPA/Parser.h>
#include <iostream>
using namespace std;

Parser p;
PKB *pkb;
DesignExtractor *de;

void DesignExtractorTest::setUp()
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
	pkb = p.getPKB();
	de = new DesignExtractor(pkb);
}


void DesignExtractorTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( DesignExtractorTest );

void DesignExtractorTest::testHelperFunction(){
	CPPUNIT_ASSERT(DesignExtractor::convertExprToPrefix("(a + b - c) * (e * f) - ( g - h*i)") == "- * - + a b c * e f - g * h i");
	CPPUNIT_ASSERT(DesignExtractor::convertExprToPrefix("x + y + z") == "+ + x y z");
	CPPUNIT_ASSERT(DesignExtractor::convertExprToPrefix("(x + y + z)") == "+ + x y z");
	CPPUNIT_ASSERT(DesignExtractor::convertExprToPrefix("((1 * 2) + (3 * 4)) - ((5 - 6) + (7 - 8))") == "- + * 1 2 * 3 4 + - 5 6 - 7 8");
}

void DesignExtractorTest::testGetParentResult(){
	// Parent(1, a), return <1, 2>
	PARENT_LIST result1 = de->getParentResult(ASSIGNMENT, 1, 0); 
	PARENT_LIST::iterator itr = result1.begin();
	
	CPPUNIT_ASSERT(itr->first==1);
	CPPUNIT_ASSERT(itr->second==2);

	// Parent(1, w), return empty set
	PARENT_LIST result2 = de->getParentResult(WHILE, 1, 0);
	CPPUNIT_ASSERT(result2.empty()==true);
	
	// Parent(w, 2), return <1, 2>
	PARENT_LIST result3 = de->getParentResult(WHILE, 0, 2);
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 2);

	// Parent(s1, s2), return <1, 2>, <3, 4>, <3, 5>
	PARENT_LIST result4 = de->getParentResult(STATEMENT, STATEMENT);
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
	PARENT_LIST result5 = de->getParentResult(ANY, STATEMENT);
	CPPUNIT_ASSERT(result5.size() == 3);
	PARENT_LIST result6 = de->getParentResult(STATEMENT, ANY);
	CPPUNIT_ASSERT(result6.size() == 3);
	PARENT_LIST result11 = de->getParentResult(STATEMENT, ANY);
	CPPUNIT_ASSERT(result11.size() == 3);
	
	// Parent(_, a), return 3 results
	PARENT_LIST result7 = de->getParentResult(ANY, ASSIGNMENT);
	CPPUNIT_ASSERT(result7.size() == 3);

	// Parent(_, w), return 0 results
	PARENT_LIST result8 = de->getParentResult(ANY, WHILE);
	CPPUNIT_ASSERT(result8.size() == 0);

	// Parent(w, _), return 3 results
	PARENT_LIST result9 = de->getParentResult(WHILE, ANY);
	CPPUNIT_ASSERT(result9.size() == 3);

	// Parent(a, _), return 0
	PARENT_LIST result10 = de->getParentResult(ASSIGNMENT, ANY);
	CPPUNIT_ASSERT(result10.size() == 0);
	
	// Parent(_, 2), return 1 
	PARENT_LIST result12 = de->getParentResult(ANY, 0, 2);
	CPPUNIT_ASSERT(result12.size() == 1);

	// Parent(3, _), return 2
	PARENT_LIST result13 = de->getParentResult(ANY, 3, 0);
	CPPUNIT_ASSERT(result13.size() == 2);
}

void DesignExtractorTest::testGetParentStarResult()
{
	// Parent(s1, s2), return <1, 2>, <3, 4>, <3, 5>
	PARENT_LIST result1 = de->getParentStarResult(STATEMENT, STATEMENT);
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
	PARENT_LIST result2 = de->getParentResult(ASSIGNMENT, 3, 0); 
	 itr = result2.begin();
	
	CPPUNIT_ASSERT(itr->first==3);
	CPPUNIT_ASSERT(itr->second==4);
	itr++;
	CPPUNIT_ASSERT(itr->first==3);
	CPPUNIT_ASSERT(itr->second==5);

}
void DesignExtractorTest::testGetIsParentResult(){
	// Parent(1, 2),return true
	bool result1 = de->getIsParentResult(1, 2);
	CPPUNIT_ASSERT(result1 == true);

	// Parent(2, 3) return false
	bool result2 = de->getIsParentResult(2, 3);
	CPPUNIT_ASSERT(result2 == false);
}

void DesignExtractorTest::testGetFollowsResult(){
	//Follow(1, a), return empty set
	FOLLOWS_LIST result1 = de->getFollowsResult(ASSIGNMENT, 1, 0);
	CPPUNIT_ASSERT(result1.empty() == true);

	//Follow(1, w), return <1, 3>
	FOLLOWS_LIST result2 = de->getFollowsResult(WHILE, 1, 0);
	FOLLOWS_LIST::iterator itr = result2.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);

	//Follow(s1, s2), return <1, 3>, <3, 6>, <4, 5>, <6, 7>
	FOLLOWS_LIST result3 = de->getFollowsResult(STATEMENT, STATEMENT);
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
	FOLLOWS_LIST result4 = de->getFollowsResult(WHILE, ASSIGNMENT);
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 6);
	
	//Follow(a, a) return <4, 5>, <6, 7>
	FOLLOWS_LIST result5 = de->getFollowsResult(ASSIGNMENT, ASSIGNMENT);
	itr = result5.begin();
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == 5);
	itr++;
	CPPUNIT_ASSERT(itr->first == 6);
	CPPUNIT_ASSERT(itr->second == 7);
	
	//Follow(w, w), return <1, 3>
	FOLLOWS_LIST result6 = de->getFollowsResult(WHILE, WHILE);
	itr = result6.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);

	//Follow(w, 3), return <1, 3>
	FOLLOWS_LIST result7 = de->getFollowsResult(WHILE, 0, 3);
	itr = result7.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);

	//Follow(a, 6), return empty set
	FOLLOWS_LIST result8 = de->getFollowsResult(ASSIGNMENT, 0, 6);
	CPPUNIT_ASSERT(result8.empty()==true);

	// Follows(_, s2) == Follows(s1, _) == Follows(s1, s2) == Follows(_, _)
	FOLLOWS_LIST result9 = de->getFollowsResult(ANY, STATEMENT);
	FOLLOWS_LIST result10 = de->getFollowsResult(STATEMENT, ANY);
	FOLLOWS_LIST result11 = de->getFollowsResult(ANY, ANY);
	
	CPPUNIT_ASSERT(result9.size() == 4);
	CPPUNIT_ASSERT(result10.size() == 4);
	CPPUNIT_ASSERT(result11.size() == 4);

	// Follows(_, a), return 3
	FOLLOWS_LIST result12 = de->getFollowsResult(ANY, ASSIGNMENT);
	CPPUNIT_ASSERT(result12.size() == 3);

	// Follows(a, _), return 2
	FOLLOWS_LIST result13 = de->getFollowsResult(ASSIGNMENT, ANY);
	CPPUNIT_ASSERT(result13.size() == 2);

	//Follows(_, 7), return 1
	FOLLOWS_LIST result14 = de->getFollowsResult(ANY, 0, 7);
	CPPUNIT_ASSERT(result14.size() == 1);

	//Follows(4, _), return 1
	FOLLOWS_LIST result15 = de->getFollowsResult(ANY, 4, 0);
	CPPUNIT_ASSERT(result15.size() == 1);
	
	//Follows(7, _), return 0
	FOLLOWS_LIST result16 = de->getFollowsResult(ANY, 7, 0);
	CPPUNIT_ASSERT(result16.size() == 0);
	
}
void DesignExtractorTest::testGetFollowsStarResult()
{
	//Follow(s1, s2), return <1, 3>,<1, 6>, <1,7>,  <3, 6>, <4, 5>
	FOLLOWS_LIST result1 = de->getFollowsStarResult(STATEMENT, STATEMENT);
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
	FOLLOWS_LIST result2 = de->getFollowsStarResult(WHILE, ASSIGNMENT);
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
	FOLLOWS_LIST result3 = de->getFollowsStarResult(ASSIGNMENT, ASSIGNMENT);
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == 5);
	itr++;
	CPPUNIT_ASSERT(itr->first == 6);
	CPPUNIT_ASSERT(itr->second == 7);

	//Follow(w, w), return <1, 3>
	FOLLOWS_LIST result4 = de->getFollowsStarResult(WHILE, WHILE);
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 3);
	

	//Follow(w, 4), return none
	 FOLLOWS_LIST result5 = de->getFollowsStarResult(WHILE, 0, 4);
	 CPPUNIT_ASSERT(result5.size()==0);
	
	 //Follow(w, 7), return none
	 FOLLOWS_LIST result6 = de->getFollowsStarResult(WHILE, 0, 7);
	 itr=result6.begin();
	// cout << itr->first << "\n";
	 CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == 7);
	itr++;
	 CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == 7);
}

void DesignExtractorTest::testGetIsFollowsResult(){
	//Follows(1, 3), return true
	bool result1 = de->getIsFollowsResult(1, 3);
	CPPUNIT_ASSERT(result1==true);

	bool result2 = de->getIsFollowsResult(4, 5);
	CPPUNIT_ASSERT(result2==true);
	
	//Follows(2, 3), return false
	bool result3 = de->getIsFollowsResult(2, 3);
	CPPUNIT_ASSERT(result3==false);
}

void DesignExtractorTest::testGetModifiesResult(){
	//Modifies(1, v), return <1, getVarIndex("b")>
	MODIFIES_LIST result1 = de->getModifiesResult(STATEMENT, 1, 0);
	MODIFIES_LIST::iterator itr = result1.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));

	// Modifies(w, "b"), Modifies (w, "thisVar")
	//  <3, getVarIndex("thisVar")>
	MODIFIES_LIST result2 = de->getModifiesResult(WHILE, 0, pkb->getVarIndex("thisVar"));
	itr = result2.begin();
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));

	// <1, getVarIndex("b")>
	MODIFIES_LIST result3 = de->getModifiesResult(WHILE, 0, pkb->getVarIndex("b"));
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));

	//Modifies(p, "b"), return <getProcIndex("main"), getVarIndex("b")>
	/*MODIFIES_LIST result4 = de->getModifiesResult(PROCEDURE, 0, pkb->getVarIndex("b"));
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));
	*/

	//Modifies(p, v), Modifies("main", v), both should return same output
	//<1, "b">, <1, "b">, <1, "thisVar"> <1, "a",> <1, "thisVar">, <1, "a">, <1, "y">
	MODIFIES_LIST result5 = de->getModifiesResult(PROCEDURE, 0, 0);
	itr = result5.begin();
	// will return in sorting order
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("y"));
	
	MODIFIES_LIST result6 = de->getModifiesResult(PROCEDURE, pkb->getProcIndex("main"), 0);
	itr = result6.begin();
	// will return in sorting order
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == pkb->getProcIndex("main"));
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("y"));

	//Modifies(a, v), return <2, "b">, <4, "thisVar">, <5, "a">, <6, "y">
	MODIFIES_LIST result7 = de->getModifiesResult(ASSIGNMENT, 0, 0);
	itr = result7.begin();
	// will return in sorting order
	CPPUNIT_ASSERT(itr->first == 2);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 4 );
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 5 );
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 6 );
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("y"));


	//Modifies(a, "y"), return <6, "y">
	MODIFIES_LIST result8 = de->getModifiesResult(ASSIGNMENT, 0, pkb->getVarIndex("y"));
	itr = result8.begin();
	CPPUNIT_ASSERT(itr->first == 6 );
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("y"));

	// Modifies(_, _), return non-empty result
	MODIFIES_LIST result9 = de->getModifiesResult(ANY, 0, 0);
	CPPUNIT_ASSERT(result9.empty() == false);

	// Modifies(1, _) return size of 1
	MODIFIES_LIST result11 = de->getModifiesResult(ANY, 1, 0);
	CPPUNIT_ASSERT(result11.size() == 1);

}

void DesignExtractorTest::testGetIsModifiesResult(){
	// Modifies(1, "b"), return true
	bool result1 = de->getIsModifiesResult(STATEMENT, 1, pkb->getVarIndex("b"));
	CPPUNIT_ASSERT(result1 == true);

	// Modifies("main", "b"), return true
	bool result2 = de->getIsModifiesResult(PROCEDURE, pkb->getProcIndex("main"), pkb->getVarIndex("b"));
	CPPUNIT_ASSERT(result2 == true);

	// Modifies(3, "b"), return false
	bool result3 = de->getIsModifiesResult(STATEMENT, 3, pkb->getVarIndex("b"));
	CPPUNIT_ASSERT(result3 == false);
}

void DesignExtractorTest::testGetUsesResult(){
	
	//Uses(p, v), return <1, "a">, <1, "c">, <1, "thisVar"> <1, "x"> <1, "y">
	USES_LIST result1 = de->getUsesResult(PROCEDURE, 0, 0);
	USES_LIST::iterator itr = result1.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("c"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("x"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("y"));
	
	USES_LIST result2 = de->getUsesResult(PROCEDURE, pkb->getProcIndex("main"), 0);
	itr = result2.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("c"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("x"));
	itr++;
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("y"));


	//Uses(1, v), return <1, "a">, <1, "c">,
	USES_LIST result3 = de->getUsesResult(STATEMENT, 1, 0);
	itr = result3.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("c"));

	//Uses(w, v), return <1, "a">, <1, "c">,<3, "b">, <3, "thisVar">, <3, "a">, <3, "x">, <3, "y">
	USES_LIST result4 = de->getUsesResult(WHILE, 0, 0);
	itr = result4.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("c"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("b"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("x"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("y"));

	//Uses(w, "a"), return <1, "a">,  <3, "a">,
	USES_LIST result5 = de->getUsesResult(WHILE, 0, pkb->getVarIndex("a"));
	itr = result5.begin();
	CPPUNIT_ASSERT(itr->first == 1);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));
	itr++;
	CPPUNIT_ASSERT(itr->first == 3);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("a"));

	//Uses(a, "thisVar"), return <4, "thisVar">
	USES_LIST result6 = de->getUsesResult(ASSIGNMENT, 0, pkb->getVarIndex("thisVar"));
	itr = result6.begin();
	CPPUNIT_ASSERT(itr->first == 4);
	CPPUNIT_ASSERT(itr->second == pkb->getVarIndex("thisVar"));

	//Uses(_, _), return not empty
	USES_LIST result7 = de->getUsesResult(ANY, 0, 0);
	CPPUNIT_ASSERT(result7.empty() == false);

	//Uses(_, "x") return not empty
	USES_LIST result8 = de->getUsesResult(ANY, 0, pkb->getVarIndex("x"));
	CPPUNIT_ASSERT(result8.empty() == false);

	//Uses(1, _) 
	USES_LIST result9 = de->getUsesResult(ANY, 1, 0);
	CPPUNIT_ASSERT(result9.size() == 2);
	
	
}

void DesignExtractorTest::testGetIsUsesResult(){
	// Uses("main", "a"), return true
	bool result1 = de->getIsUsesResult(PROCEDURE, pkb->getProcIndex("main"), pkb->getVarIndex("a"));
	CPPUNIT_ASSERT(result1 == true);

	// Uses(3, "a") return true
	bool result2 = de->getIsUsesResult(STATEMENT, 3, pkb->getVarIndex("a"));
	CPPUNIT_ASSERT(result2 == true);

	// Uses(4, "xyz") return false
	bool result3 = de->getIsUsesResult(STATEMENT, 4, pkb->getVarIndex("xyz"));
	CPPUNIT_ASSERT(result3 == false);
}
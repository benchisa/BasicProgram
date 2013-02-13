#include "NextTest.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/Parser.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( NextTest );
void NextTest::setUp(){
	src = "procedure main{\n"
		/*1*/		 "while a{\n"
		/*2*/		 "	a = a + 1;\n"
		/*3*/		 "	b = b + 1;\n"
		/*4*/		 "	if c then{\n"
		/*5*/		 "		d = d + 1;\n"
		/*6*/		 "		d2 = d2 + 1;\n"
		/*7*/		 "		d2 = d2 + 1;\n"
		/*8*/		 "		d2 = d2 + 1;\n"
		/*9*/		 "		call text2;\n"
		/*10*/		 "		d2 = d2 + 1;\n"
		/*11*/		 "		d2 = d2 + 1;}\n"
		/**/		 "	else{\n"
		/*12*/		 "		e = e + 1;\n"
		/*13*/		 "		e = e + 1;\n"
		/*14*/		 "		e = e + 1;\n"
		/*15*/		 "		while h{\n"
		/*16*/		 "		i = i + 1;}\n"
		/*17*/		 "		e = e + 1;\n"
		/*18*/		 "		f = f + 1;}}}\n"
		/**/	   "procedure text2{\n"
		/*19*/		 "h = h + 1;\n"
		/*20*/		 "b = b + 1;}";
}

void NextTest::tearDown(){}

void NextTest::testIsNext(){
	Parser p;
	p.setSource(src);
	p.startParse();
	PKB *pkb = p.getPKB();
	DesignExtractor *de = new DesignExtractor(pkb);
	de->createCFG();

	//isNextResult, isNextResultStar
	CPPUNIT_ASSERT(de->isNextResult(16,15));
	CPPUNIT_ASSERT(!de->isNextResult(18,19));
	CPPUNIT_ASSERT(!de->isNextResult(100,100));
	CPPUNIT_ASSERT(!de->isNextResult(-3,-2));

	CPPUNIT_ASSERT(de->isNextStarResult(15,15));
	CPPUNIT_ASSERT(de->isNextStarResult(11,1));
	CPPUNIT_ASSERT(!de->isNextStarResult(1000,1000));
	CPPUNIT_ASSERT(!de->isNextStarResult(0,0));
}

void NextTest::testGetNext(){
	Parser p;
	p.setSource(src);
	p.startParse();
	DesignExtractor *de = new DesignExtractor(p.getPKB());
	de->createCFG();

	//getNextResult, getNextResultStarResult
	CPPUNIT_ASSERT(de->getNextResult(0,0).size() == 21);
	CPPUNIT_ASSERT(de->getNextStarResult(0,0).size() == 325);
	CPPUNIT_ASSERT(de->getNextStarResult(19,20).size() == 1); // this case shld not appear, just for checking
	
}
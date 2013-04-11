#include "ParserTest.h"
#include <cppunit/config/SourcePrefix.h>
#include <../SPA/Parser.h>
#include <../SPA/PKB.h>
#include <../SPA/DesignExtractor.h>
using namespace std;

void ParserTest::setUp()
{
}


void ParserTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest ); // Note 4 

void ParserTest::testComplex(){
	Parser p;
	std::string src,src2;

	src = " procedure xylo{\n"
		/*1*/	"     apple=apple + 1 + 3;\n"
		/*2*/	"     banana=apple+1-1;\n"
		/*3*/	"     carrot=apple;\n"
		/*4*/	"     banana=carrot*(egg+fish-apple);\n"
		/*5*/	"     donut=0;\n"
		/*6*/	"     egg=1;\n"
		/*7*/	"     fish=egg;\n"
		/*8*/	"     call yellow;\n"
		/*9*/	"     while apple{\n"
		/*10*/	"          while egg{\n"
		/*11*/	"              innard = 0;\n"
		/*12*/	"               while donut{\n"
		/*13*/	"                  gummy=egg+1;}\n"
		/*14*/	"               while fish{\n"
		/*15*/	"                    gummy=0;\n"
		/*16*/	"                    while innard{\n"
		/*17*/	"                         fish=innard;}}\n"
		/*18*/	"               ham=gummy;}\n"
		/*19*/	"          gummy=apple+9;}\n"
		/*20*/	"   apple=11;\n"
		/*21*/	"   if innard then{\n"
		/*22*/	"        jam=apple;}\n"
		/**/	"	else{\n"
		/*23*/	"        jam=gummy;\n"
		/*24*/	"        if egg then{\n"
		/*25*/	"             gummy=33;}\n"
			"          else{\n"
		/*26*/	"             gummy=99;}}}\n"
		/**/	"procedure yellow{\n"
		/*27*/	"	kimchi = 88;\n"
		/*28*/	"	leek=0;\n"
		/*29*/	"	while leek{\n"
		/*30*/	"      kimchi=kimchi+1-3;}\n"
		/*31*/	"	call zebra;\n"
		/*32*/	"	apple=kimchi;\n"
		/*33*/	"	jam=egg+2;}"
		/**/	"	procedure zebra{\n"
		/*34*/	"	 meat=1;\n"
		/*35*/	"	 banana=meat+10;\n"
		/*36*/	"	 if meat then{\n"
		/*37*/	"		  carrot=banana+22;}\n"
		/**/	"	 else{\n"
		/*38*/	"		  carrot=banana;}\n"
		/*39*/	"	 apple=1;\n"
		/*40*/	"	 kimchi=apple+10;\n"
		/*41*/	"	 call extra;}";

	
	string line;
	ifstream file("simple40.txt");
	if(file.is_open()){
		while(file.good()){
			getline(file, line);
			src2+= line + "\n";
		}

		file.close();
	}
	string src3 = "procedure main{  a = (a*b+e)-(k*h);}";

	p.setSource(src2);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	DesignExtractor *de = new DesignExtractor(pkb);
	de->createCFG();
	cout << "\n";
	
	// Query 60
	cout << "Query 60: \n";
	cout << de->getIsContainResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainResult(MINUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainResult(PLUS, -1, CONSTANT, pkb->getConstantIndex(1)) << "\n";
	cout << de->getIsContainResult(MULTIPLY, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";
	cout << de->getIsContainResult(MINUS, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";

	// Query 61
	cout << "Query 61: \n";
	cout << de->getIsContainStarResult(PLUS, -1, CONSTANT, pkb->getConstantIndex(8)) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainStarResult(MINUS, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";
	cout << de->getIsContainStarResult(MULTIPLY, -1, CONSTANT, pkb->getConstantIndex(8)) << "\n";

	// Query 63
	cout << "Query 63: \n";
	cout << de->getContainResult(PLUS, -1, CONSTANT, 0).size() << "\n"; 
	cout << de->getContainResult(PLUS, -1, VARIABLE, 0).size() << "\n";  
	cout << de->getContainResult(MINUS, -1, CONSTANT, 0).size() << "\n"; 
	cout << de->getContainResult(MINUS, -1, VARIABLE, 0).size() << "\n";
	cout << de->getContainResult(MULTIPLY, -1, CONSTANT, 0).size() << "\n"; 
	cout << de->getContainResult(MULTIPLY, -1, VARIABLE, 0).size() << "\n";


	/*CONTAIN_LIST a =  de->getContainStarResult(PROCEDURE, 0, ASSIGNMENT, 0);
	CONTAIN_LIST::iterator itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain(" << itr->first << ", " << itr->second << ")\n";
	}



	/*
	cout << de->getIsContainResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, MINUS, -1) << "\n";

	
	cout << de->getIsContainResult(MINUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainStarResult(MINUS, -1, PLUS, -1) << "\n";
	/*
	// Query 60
	cout << "Query 60: \n";
	cout << de->getIsContainResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainResult(MINUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainResult(PLUS, -1, CONSTANT, pkb->getConstantIndex(1)) << "\n";
	cout << de->getIsContainResult(MULTIPLY, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";
	cout << de->getIsContainResult(MINUS, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";

	// Query 61
	cout << "Query 61: \n";
	cout << de->getIsContainStarResult(PLUS, -1, CONSTANT, pkb->getConstantIndex(8)) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, MINUS, -1) << "\n";
	cout << de->getIsContainStarResult(PLUS, -1, PLUS, -1) << "\n";
	cout << de->getIsContainStarResult(MINUS, -1, CONSTANT, pkb->getConstantIndex(9)) << "\n";
	cout << de->getIsContainStarResult(MULTIPLY, -1, CONSTANT, pkb->getConstantIndex(8)) << "\n";

	// Query 63
	cout << "Query 63: \n";
	cout << de->getContainResult(PLUS, -1, CONSTANT, 0).size() << "\n"; 
	cout << de->getContainResult(PLUS, -1, VARIABLE, 0).size() << "\n";  
	cout << de->getContainResult(MINUS, -1, CONSTANT, 0).size() << "\n"; 
	cout << de->getContainResult(MINUS, -1, VARIABLE, 0).size() << "\n";
	cout << de->getContainResult(MULTIPLY, -1, CONSTANT, 0).size() << "\n"; 
	cout << de->getContainResult(MULTIPLY, -1, VARIABLE, 0).size() << "\n";

	//Contains(30,sl), Contains("Goneril",sl)
	cout <<"Query 63 (STMT_LIST): \n";
	cout << de->getContainResult(STATEMENT, 30, STMT_LIST, 0).size() << "\n";
	cout << de->getContainResult(STATEMENT, 36, STMT_LIST, 0).size() << "\n";
	cout << de->getContainResult(PROCEDURE, pkb->getProcIndex("Goneril"), STMT_LIST, 0).size() << "\n";
	cout << de->getContainResult(STATEMENT, 99, STMT_LIST, 0).size() << "\n";

	cout << "Query 64: \n";
	CONTAIN_LIST a =  de->getContainStarResult(ASSIGNMENT, 0, CONSTANT, pkb->getConstantIndex(12));
	CONTAIN_LIST::iterator itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain(" << itr->first << ", " << pkb->getConstantValue(itr->second) << ")\n";
	}
	cout << "\n";
	a =  de->getContainStarResult(IF, 0, CALL, 47);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain(" << itr->first << ", " << pkb->getConstantValue(itr->second) << ")\n";
	}
	
	cout << "\n";
	a =  de->getContainStarResult(IF, 0, VARIABLE, pkb->getVarIndex("jam"));
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getVarName(itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(IF, 0, PLUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",PLUS)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(IF, 0, MINUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",MINUS)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(IF, 0, MULTIPLY, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",MULTIPLY)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(IF, 0, CONSTANT, pkb->getConstantIndex(9));
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getConstantValue(itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(WHILE, 0, PLUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",PLUS)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(WHILE, 0, MINUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",MINUS)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(WHILE, 0, MULTIPLY, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",MULTIPLY)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(WHILE, 0, CONSTANT, pkb->getConstantIndex(4));
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getConstantValue(itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(PROCEDURE, 0, PLUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << pkb->getProcedure(itr->first)->getProcName() << ",PLUS)\n";
	}
	cout << "\n";

	
	a =  de->getContainStarResult(PROCEDURE, 0, MINUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << pkb->getProcedure(itr->first)->getProcName() << ",MINUS)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(PROCEDURE, 0, MULTIPLY, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << pkb->getProcedure(itr->first)->getProcName() << ",MULTIPLY)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STATEMENT, 0, VARIABLE, pkb->getVarIndex("mango"));
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << "," << pkb->getVarName(itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STATEMENT, 0, PLUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",PLUS)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STATEMENT, 0, MINUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",MINUS)\n";
	}
	cout << "\n";


	a =  de->getContainStarResult(STATEMENT, 0, MULTIPLY, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ",MULTIPLY)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STATEMENT, 0, CONSTANT, pkb->getConstantIndex(103));
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getConstantValue(itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STMT_LIST, 0, STATEMENT,31);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STMT_LIST, 0, STATEMENT,36);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STMT_LIST, 0, STATEMENT,30);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STMT_LIST, 0, STATEMENT,56);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STMT_LIST, 0, VARIABLE,pkb->getVarIndex("fish"));
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getVarName( itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STMT_LIST, 0, PLUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << "PLUS)\n";
	}
	cout << "\n";

	
	a =  de->getContainStarResult(STMT_LIST, 0, MINUS, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << "MINUS)\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STMT_LIST, 0, MULTIPLY, -1);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << "MULTIPLY)\n";
	}
	cout << "\n";

	
	a =  de->getContainStarResult(STMT_LIST, 0, CONSTANT, pkb->getConstantIndex(13));
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getConstantValue(itr->second) << ")\n";
	}
	cout << "\n";

	
	a =  de->getContainStarResult(STATEMENT, 0, STATEMENT, 56);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second << ")\n";
	}
	cout << "\n";

	
	a =  de->getContainStarResult(STATEMENT, 38, VARIABLE, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getVarName(itr->second) << ")\n";
	}
	cout << "\n";
	
	
	a =  de->getContainStarResult(STATEMENT, 61, IF, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second << ")\n";
	}
	cout << "\n";
	
	a =  de->getContainStarResult(STATEMENT, 48, CALL, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second << ")\n";
	}
	cout << "\n";
	
	a =  de->getContainStarResult(STATEMENT, 22, VARIABLE, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getVarName(itr->second) << ")\n";
	}
	cout << "\n";
	
	a =  de->getContainStarResult(STATEMENT, 7, CONSTANT, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << pkb->getConstantValue(itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STATEMENT, 51, CALL, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " <<itr->second << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STATEMENT, 51, VARIABLE, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " <<pkb->getVarName(itr->second) << ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(PROCEDURE, pkb->getProcIndex("Bob"), VARIABLE, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " <<pkb->getVarName(itr->second) << ")\n";
	}
	cout << "\n";

	
	a =  de->getContainStarResult(STATEMENT,51, STATEMENT, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " <<itr->second << ")\n";
	}
	cout << "\n";

	
	a =  de->getContainStarResult(STATEMENT,51, CALL, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " <<itr->second << ")\n";
	}
	cout << "\n";
	
	
	a =  de->getContainStarResult(STATEMENT, 51, VARIABLE, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " <<pkb->getVarName(itr->second) << ")\n";
	}
	cout << "\n";
	
	
	a =  de->getContainStarResult(STATEMENT, 61, CONSTANT, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " <<pkb->getConstantValue(itr->second) << ")\n";
	}
	cout << "\n";

	
	
	a =  de->getContainStarResult(STATEMENT, 9, STMT_LIST, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second<< ")\n";
	}
	cout << "\n";
	

	a =  de->getContainStarResult(PROCEDURE, pkb->getProcIndex("Bob"), STMT_LIST, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << pkb->getProcedure(itr->first)->getProcName() << ", " << itr->second<< ")\n";
	}
	cout << "\n";

	a =  de->getContainStarResult(STATEMENT, 51, STMT_LIST, 0);
	itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain*(" << itr->first << ", " << itr->second<< ")\n";
	}
	cout << "\n";
	*/
	/*CONTAIN_LIST a =  de->getContainResult(PLUS, -1, VARIABLE, 0);
	CONTAIN_LIST::iterator itr = a.begin();
	for(itr = a.begin(); itr!=a.end(); itr++){
		cout << "Contain(" << itr->first << ", " << pkb->getVarName(itr->second) << ")\n";
	}
	*/
	
	/* check tree*/
	
	/*
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	
	AST_LIST *tmp = pkb->getASTBy(33);
	AST *tmpAST = tmp->front()->getFirstDescendant();



	
	cout << "ASSIGN NODE: " << pkb->getType(tmpAST) << "\n";
	cout << "LHS: " <<  pkb->getVarName(pkb->getData(tmpAST->getFirstDescendant())) << "\n";
	cout << "RHS TYPE: " << pkb->getType(tmpAST->getFirstDescendant()->getRightSibling()) << "\n";
	cout << "RHS's LHS: " << pkb->getType(tmpAST->getFirstDescendant()->getRightSibling()->getFirstDescendant()) << "\n";
	cout << "RHS's LHS's LHS's varName: " << pkb->getVarName(pkb->getData(tmpAST->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant())) << "\n";
	cout << "RHS's LHS's RHS's constant value: " << pkb->getConstantValue(pkb->getData(tmpAST->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling())) << "\n";
	cout << "RHS's RHS's constant value" << pkb->getConstantValue(pkb->getData(tmpAST->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling())) << "\n";


	tmp = pkb->getASTBy(34);
	tmpAST = tmp->front();

	cout << "ASSIGN NODE: " << pkb->getType(tmpAST) << "\n";
	cout << "LHS: " <<  pkb->getVarName(pkb->getData(tmpAST->getFirstDescendant())) << "\n";
	cout << "RHS TYPE: " << pkb->getType(tmpAST->getFirstDescendant()->getRightSibling()) << "\n";
	cout << "RHS's LHS's varName: " << pkb->getVarName(pkb->getData(tmpAST->getFirstDescendant()->getRightSibling()->getFirstDescendant())) << "\n";
	cout << "RHS's RHS's LHS's constantValue: " << pkb->getConstantValue(pkb->getData(tmpAST->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant())) << "\n";
	cout << "RHS's RHS's RHS's constantValue: " << pkb->getConstantValue(pkb->getData(tmpAST->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling())) << "\n";
	cout << "asd: " << de->getIsContainStarResult(PLUS, -1, MULTIPLY, -1) << "\n";
	cout << "asd: " << de->getIsContainStarResult(MINUS, -1, PLUS, -1) << "\n";
	cout << "asd: " << de->getIsContainStarResult(PLUS, -1, PLUS, -1) << "\n";
	cout << "asd: " << de->getIsContainStarResult(MINUS, -1, MULTIPLY, -1) << "\n";
	cout << "asd: " << de->getIsContainStarResult(MINUS, -1, MINUS, -1) << "\n";
	cout << "asd: " << de->getIsContainStarResult(MULTIPLY, -1, MULTIPLY, -1) << "\n";
	cout << "asd: " << de->getIsContainStarResult(VARIABLE, pkb->getVarIndex("lamp"), CONSTANT, pkb->getConstantIndex(999)) << "\n";
	/*CPPUNIT_ASSERT(de->getIsContainResult(STATEMENT, 1, PLUS, -1) == true);
	CPPUNIT_ASSERT(de->getIsContainResult(STATEMENT, 1, MINUS, -1) == false);
	CPPUNIT_ASSERT(de->getIsContainResult(STATEMENT, 1, VARIABLE, pkb->getVarIndex("apple")) == true);
	CPPUNIT_ASSERT(de->getIsContainResult(STATEMENT, 1, VARIABLE, pkb->getConstantIndex(1))== true);
	CPPUNIT_ASSERT(de->getIsContainResult(STATEMENT, 1, STATEMENT, 2) == false);

	CPPUNIT_ASSERT(de->getIsContainResult(PROCEDURE, 1, STATEMENT, 1) == true);
	CPPUNIT_ASSERT(de->getIsContainResult(PROCEDURE, 2, STATEMENT, 27) == true);
	CPPUNIT_ASSERT(de->getIsContainResult(PROCEDURE, 1, STATEMENT, 2) == false);
	
	CPPUNIT_ASSERT(de->getIsContainResult(PLUS, -1, PLUS, -1) == true);
	CPPUNIT_ASSERT(de->getIsContainResult(MINUS, -1, PLUS, -1) == true);
	
	//CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 1, WHILE, 0);
	//CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 1, STMT_LIST, 0);
	//CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 1, IF, 0);
	
	
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, STMT_LIST, 0);
	//CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 0, STMT_LIST, 0);
	//CONTAIN_LIST cList = de->getContainResult(WHILE, 0, STMT_LIST, 0);
	//CONTAIN_LIST cList = de->getContainResult(IF, 0, STMT_LIST, 0);
	//CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, STATEMENT, 0);
	//CONTAIN_LIST cList = de->getContainResult(IF, 0, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(WHILE, 0, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, CONSTANT, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, CONSTANT, 0);
	

	//CONTAIN_LIST cList = de->getContainResult(IF, 0, VARIABLE, pkb->getVarIndex("innard"));
	//CONTAIN_LIST cList = de->getContainResult(WHILE, 0, VARIABLE, pkb->getVarIndex("apple"));
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, VARIABLE, pkb->getVarIndex("apple"));
	//CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, PLUS, -1);
	//CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, MINUS, -1);
	//CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, MULTIPLY, -1);
	//CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, VARIABLE, pkb->getVarIndex("apple"));
	//CONTAIN_LIST cList = de->getContainResult(ASSIGNMENT, 0, CONSTANT, pkb->getConstantIndex(88));
	//CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, STATEMENT, 10);
	//CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, CALL, 8);
	//CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, ASSIGNMENT, 11);
	//CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, IF, 24);
	//CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 0, WHILE, 16);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 0, STMT_LIST, 15);
	//CONTAIN_LIST cList = de->getContainResult(PROCEDURE, 0, STMT_LIST, 1);
	//CONTAIN_LIST cList = de->getContainResult(WHILE, 0, STMT_LIST, 17);
	//CONTAIN_LIST cList = de->getContainResult(IF, 0, STMT_LIST, 23);

	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 1, STATEMENT, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 2, STATEMENT, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 11, STATEMENT, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 11, WHILE, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 11, ASSIGNMENT, 0);
	//CONTAIN_LIST cList = de->getContainResult(STMT_LIST, 11, ASSIGNMENT, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 22, STATEMENT, 0);

	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 1, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 27, CONSTANT, 0);
	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 21, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(IF, 21, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(IF, 21, STMT_LIST, 0);

	//CONTAIN_LIST cList = de->getContainResult(STATEMENT, 10, VARIABLE, 0);
	//CONTAIN_LIST cList = de->getContainResult(WHILE, 10, VARIABLE, 0);
	/*CONTAIN_LIST cList = de->getContainStarResult(STMT_LIST, 0, STMT_LIST, 0);
	CONTAIN_LIST::iterator cListItr = cList.begin();

	cout << "\n";
	while(cListItr!=cList.end()){
		cout << "(" << cListItr->first << "," << cListItr->second << ")\n";
		cListItr++;
	}
	
	cout << "\n=============ContainsStar=============\n";
	cout << "============= UNKNOWN, UNKNOWN ===========\n";
	cout << de->getContainStarResult(STMT_LIST, 0, STMT_LIST, 0).size() << "\n";
	cout << de->getContainStarResult(STMT_LIST, 10, STMT_LIST, 0).size() << "\n";
	cout << de->getContainStarResult(STMT_LIST, 0, STMT_LIST, 13).size() << "\n";

	cout << de->getContainStarResult(STATEMENT, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, ASSIGNMENT, 0).size() <<"\n"; 

	cout << de->getContainStarResult(STATEMENT, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(ASSIGNMENT, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(WHILE, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, ASSIGNMENT, 0).size() <<"\n"; 
	cout << de->getContainStarResult(WHILE, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(IF, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, ASSIGNMENT, 0).size() <<"\n"; 
	cout << de->getContainStarResult(IF, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, MINUS, -1).size() <<"\n\n"; //

	cout << "============= UNKNOWN, KNOWN ===========\n";
	cout << de->getContainStarResult(STATEMENT, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, STATEMENT, 44).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, CONSTANT, pkb->getConstantIndex(1)).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, VARIABLE, pkb->getVarIndex("meat")).size() <<"\n\n"; //

	cout << de->getContainStarResult(PROCEDURE, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, MINUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, VARIABLE, pkb->getVarIndex("b")).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, CONSTANT, pkb->getConstantIndex(11)).size() <<"\n\n"; //
	*/
	/*
	cout << DesignExtractor::convertExprToPrefix("x*y+z*b") << "\n";
	cout << DesignExtractor::convertExprToPrefix("x*y+z+b") << "\n";
	
	cout << de->getIsContainStarResult(MULTIPLY, -1, MULTIPLY, -1) <<"\n";
	
	/*MODIFIES_LIST modifiesResult = de->getFollowsStarResult(STATEMENT,0,24);
	MODIFIES_LIST::iterator itr = modifiesResult.begin();
	while(itr!=modifiesResult.end()){
		cout << "Statement " << itr->first << " uses " << pkb->getVarName(itr->second) << "\n";
		itr++;
	}

	cout << "\n=============ContainsStar=============\n";
	cout << "============= UNKNOWN, UNKNOWN ===========\n";
	//cout << de->getContainStarResult(STATEMENT, 0, STATEMENT, 0).size() <<"\n"; //
	//cout << de->getContainStarResult(STATEMENT, 0, WHILE, 0).size() <<"\n"; //
	//cout << de->getContainStarResult(STATEMENT, 0, IF, 0).size() <<"\n"; //
	//cout << de->getContainStarResult(STATEMENT, 0, ASSIGNMENT, 0).size() <<"\n"; 

	cout << de->getContainStarResult(STATEMENT, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(STATEMENT, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(ASSIGNMENT, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(WHILE, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, ASSIGNMENT, 0).size() <<"\n"; 
	cout << de->getContainStarResult(WHILE, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, MINUS, -1).size() <<"\n\n"; //

	cout << de->getContainStarResult(IF, 0, STATEMENT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, WHILE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, IF, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, ASSIGNMENT, 0).size() <<"\n"; 
	cout << de->getContainStarResult(IF, 0, VARIABLE, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, CONSTANT, 0).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, MINUS, -1).size() <<"\n\n"; //

	cout << "============= UNKNOWN, KNOWN ===========\n";
	cout << de->getContainStarResult(STATEMENT, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(WHILE, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(ASSIGNMENT, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, STATEMENT, 44).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, CONSTANT, pkb->getConstantIndex(1)).size() <<"\n"; //
	cout << de->getContainStarResult(IF, 0, VARIABLE, pkb->getVarIndex("meat")).size() <<"\n\n"; //

	cout << de->getContainStarResult(PROCEDURE, 0, STATEMENT, 13).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, PLUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, MINUS, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, MULTIPLY, -1).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, VARIABLE, pkb->getVarIndex("b")).size() <<"\n"; //
	cout << de->getContainStarResult(PROCEDURE, 0, CONSTANT, pkb->getConstantIndex(11)).size() <<"\n\n"; //
	CONTAIN_LIST result =  de->getContainStarResult(PROCEDURE, 0, CONSTANT, pkb->getConstantIndex(11));
	CONTAIN_LIST::iterator itr = result.begin();

	while(itr!=result.end()){
		cout << "Contain(" << itr->first << ", " << itr->second <<")\n";
		itr++;
	}
	cout << "End of Parser Unit Test\n";
	*/
	//cout << "Assign's Size: " << pkb->getAssignSize() << "\n";
	// Test for Follows
	/*FOLLOWS_LIST result = pkb->getFollows(0,0);
	FOLLOWS_LIST::iterator itr = result.begin();
	while(itr!=result.end()){
		cout << itr->first << "," << itr->second << "\n";
		itr++;
	}
	
/*	MODIFIES_LIST modifiesResult = pkb->getModifies(STATEMENT,0,0);
	MODIFIES_LIST::iterator itr = modifiesResult.begin();
	while(itr!=modifiesResult.end()){
		cout << "Procedure " << itr->first << " modifies " << pkb->getVarName(itr->second) << "\n";
		itr++;
	}

	USES_LIST usesResult = pkb->getUses(STATEMENT,0,0);
	USES_LIST::iterator uitr = usesResult.begin();
	while(uitr!=usesResult.end()){
		cout << "Procedure " << uitr->first << " uses " << pkb->getVarName(uitr->second) << "\n";
		uitr++;
	}

	PARENT_LIST result = pkb->getParent(0,0);
	PARENT_LIST::iterator itr = result.begin();
	while(itr!=result.end()){
		cout << itr->first << ", " << itr->second << "\n";
		itr++;
	}*/
}

void ParserTest::testCall(){
	Parser p;
	std::string src;

	src = "procedure test1{\n"
	/*1*/	"a = c;"
	/*2*/	"call test2;"
	/*3*/	"a = d;}"
	/**/	"procedure test2{\n"
	/*4*/	"b = 1;}";

	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == CALL);
}
void ParserTest::testIf()
{
	Parser p;
	std::string src;
	
	// Follows(2, 3), Follows(4, 5)
	//cout << "Start=====\n";
	src = "procedure test1{\n"
	/*1*/	"if b then {"
	/*2*/	"a = c;"
	/*3*/	"a = d;}"
	/**/	"else{"
	/*4*/	"c = d;"
	/*5*/	"e = d;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	CPPUNIT_ASSERT(pkb->isFollows(1, 2) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 3) == false);
	CPPUNIT_ASSERT(pkb->isFollows(2, 3) == true);
	CPPUNIT_ASSERT(pkb->isFollows(4, 5) == true);
	CPPUNIT_ASSERT(pkb->isFollows(2, 4) == false);
	CPPUNIT_ASSERT(pkb->isFollows(2, 5) == false);
	CPPUNIT_ASSERT(pkb->isFollows(2, 6) == false);
	CPPUNIT_ASSERT(pkb->isFollows(3, 4) == false);
	CPPUNIT_ASSERT(pkb->isFollows(4, 6) == false);

	// Test Uses and Modifies for if
	MODIFIES_LIST modifiesList = pkb->getModifies(IF, 1, 0);
	MODIFIES_LIST::iterator modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "e");
	
	USES_LIST useList = pkb->getUses(IF, 1, 0);
	USES_LIST::iterator usesListItr = useList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "d");

	// Follows(2, 3), Follows(3, 4), Follows(7, 8), Follows(1, 9)
	// Parent(1, 2) (1, 3), (1, 4), (4, 5), (4, 6), (1, 7), (1, 8)
	src = "procedure test2{\n"
	/*1*/	"if b then {"
	/*2*/	"a = c;"
	/*3*/	"a = d;"
	/*4*/	"if b then{"
	/*5*/	"a = 1;}"
	/**/	"else{"
	/*6*/	"b = 1;}}"
	/**/	"else{"
	/*7*/	"c = d;"
	/*8*/	"e = d;}"
	/*9*/	"z = y;}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	pkb = p.getPKB();
	CPPUNIT_ASSERT(pkb->isFollows(2, 3) == true);
	CPPUNIT_ASSERT(pkb->isFollows(3, 4) == true);
	CPPUNIT_ASSERT(pkb->isFollows(7, 8) == true);
	CPPUNIT_ASSERT(pkb->isFollows(1, 9) == true);
	CPPUNIT_ASSERT(pkb->isFollows(1, 8) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 2) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 4) == false);

	CPPUNIT_ASSERT(pkb->isParent(1, 2) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 3) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 4) == true);
	CPPUNIT_ASSERT(pkb->isParent(4, 5) == true);
	CPPUNIT_ASSERT(pkb->isParent(4, 6) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 7) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 8) == true);
	CPPUNIT_ASSERT(pkb->isParent(1, 9) == false);

	// Test Uses and Modifies for if
	modifiesList = pkb->getModifies(IF, 1, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "e");
	
	modifiesList = pkb->getModifies(IF, 4, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");

	useList = pkb->getUses(IF, 1, 0);
	usesListItr = useList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "c");
	CPPUNIT_ASSERT( pkb->getVarName((usesListItr++->second)) == "d");
	// varindex 7 is variable y
	CPPUNIT_ASSERT(pkb->isUses(IF, 1, 7) == false);

	// Follows(1, 2), Follows(3, 4), Follows(5, 6), Follows(2, 7)
	src = "procedure test3{\n"
		"a = y;"
		"if b then {"
		"a = c;"
		"a = d;}"
		"else{"
		"c = d;"
		"e = d;}"
		"a = 1;}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	pkb = p.getPKB();

	// Test Follows, Parent
	CPPUNIT_ASSERT(pkb->isFollows(1, 2) == true);
	CPPUNIT_ASSERT(pkb->isFollows(3, 4) == true);
	CPPUNIT_ASSERT(pkb->isFollows(5, 6) == true);
	CPPUNIT_ASSERT(pkb->isFollows(2, 7) == true);

	CPPUNIT_ASSERT(pkb->isFollows(1, 3) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 4) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 5) == false);
	CPPUNIT_ASSERT(pkb->isFollows(1, 6) == false);

	CPPUNIT_ASSERT(pkb->isParent(2, 3) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 4) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 5) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 6) == true);
	CPPUNIT_ASSERT(pkb->isParent(2, 7) == false);

	// Test AST
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == IF);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getRightSibling()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()) == ASSIGNMENT);
}

void ParserTest::testMultipleProcedures()
{
	Parser p;
	std::string src;

	src =	"procedure main{\n"
			"	a = a + 2;}\n"
			"procedure text1{\n"
			"	b = a * (2 + c);\n"
			"	c = c - 2;}"
			"procedure text2{\n"
			"	while a{\n"
			"		d = d - 2;}\n"
			"	e = e + 1;\n"
			"	if a then {\n"
			"		h = h * (a * b);\n"
			"		b = b + 1;}\n"
			"	else{\n"
			"		c = b;\n"
			"		while z{\n"
			"			abc = xyz;\n"
			"			abc1 = xyz2 * xyz + h - y;}\n"
			"		a=abc1*2;}}";
			
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	PKB *pkb = p.getPKB();
	
	// AST Test
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == PROCEDURE);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == WHILE);

	// Parent Test to ensure insertion is correct.
	CPPUNIT_ASSERT(!pkb->isParent(1, 9));
	CPPUNIT_ASSERT(pkb->isParent(4, 5));

	// Follow Test
	CPPUNIT_ASSERT(!pkb->isFollows(1, 2));
	CPPUNIT_ASSERT(!pkb->isFollows(3, 4));
	CPPUNIT_ASSERT(pkb->isFollows(4, 6));

}

void ParserTest::testOperators(){
	Parser p;
	std::string src;

	src = "procedure main{\n"
		"a = y;}\n";
	p.setSource(src);
	p.startParse();
	PKB *pkb = p.getPKB();
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	
	src = "procedure main{\n"
		"a = 1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == CONSTANT);
	

	src = "procedure main{\n"
		"a = y*1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	
	src = "procedure main{\n"
		"a = y+1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);

	src = "procedure main{\n"
		"a = y-1;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);

	
	src = "procedure main{\n"
		"a = (y+1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	
	
	src = "procedure main{\n"
		"a = (y-1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = (y*1);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	

	src = "procedure main{\n"
		"a = b + c * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()))=="b");
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()))=="c");
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getVarName(pkb->getData(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()))=="d");
	
	src = "procedure main{\n"
		"a = b - c * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = b * c + d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	
	
	src = "procedure main{\n"
		"a = b * c - d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MINUS);
	
	src = "procedure main{\n"
		"a = (b + c) * d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == VARIABLE);

	src = "procedure main{\n"
		"a = (c + d) * (d * e) ;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == MULTIPLY);

	
	src = "procedure main{\n"
		"a = b + c + d;}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == PLUS);

	src = "procedure main{\n"
		"a = (b * c - d) * a + (t + e * f * e * g);}\n";
	p.setSource(src);
	p.startParse();
	pkb = p.getPKB();
	ast = pkb->getRootAST();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == MINUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == PLUS);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == MULTIPLY);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getRightSibling()->getFirstDescendant()->getFirstDescendant()) == MULTIPLY);

	
}

// method to test whether parser can parse different source code
void ParserTest::testCorrectSource() {
	Parser p;
	std::string src;
	
	// Purpose: Simplest SIMPLE Source
	src = "procedure test1{\n"
		"a = y+1+x;}\n";
	p.setSource(src);

	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	
	src = "procedure main{\n"
		"while a{\n"
		"b = y + 2;}\n"
		"c = a + 1;}";
	p.setSource(src);

	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	src = "procedure main{\n"
		"while a{\n"
		"b = y + 2;}\n"
		"c = a + 1;}";
	p.setSource(src);
	
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
	
	// Purpose: Multiple lines, procedures, long simple expressions.
	// PROGLINE: 8, ContainerIndex: 4, 7
	src = "procedure main{\n"
		"a = y + 1 + 0 + x + b + a;\n" 
		"b = y;\n"
		"b = x + y + 1 + u;\n"
		"while a {\n"
		"x = y; \n"
		"hhhhh = 2;\n"
		"while b { \n"
		"k = 1; } }}\n";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(1, p.startParse());
}

void ParserTest::testGetLastError(){
	Parser p;
	std::string src;	

	// Illegal codes
	// Purpose: Missing ;
	src = "procedure main{\nwhile c{\na=b}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): missing colon");

	// Purpose: Invalid name
	src = "procedure main{\nwhile !@#c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid while var_name");

	
	src = "procedure ma&#(!*in{\nwhile c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid/repeated procedure name");

	// Purpose: Missing {
	src = "procedure main\nwhile c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): missing bracket");

	// Purpose: Invalid Procedure
	
	src = "procure main{\nwhile !@#c{\na=b;}}";
	p.setSource(src);
	CPPUNIT_ASSERT_EQUAL(-1, p.startParse());
	CPPUNIT_ASSERT(p.getLastError()=="Source Code Error(): invalid procedure");
}

void ParserTest::testGetPKB(){
	Parser p;
	std::string src;
	src = "procedure main{\n"
		"a = y + 1 + 0 + x + b + a;\n" 
		"b = y;\n"
		"b = x + y + 1 + u;\n"
		"while a {\n"
		"x = y; \n"
		"hhhhh = 2;\n"
		"while b { \n"
		"k = 1; } }}\n";

	p.setSource(src);
	p.startParse();
	PKB *pkb = p.getPKB();

	// Test Procedure/ProcTable
	CPPUNIT_ASSERT(pkb->getProcedure(1)->getProcName() == "main");
	CPPUNIT_ASSERT_EQUAL(1,pkb->getProcedure(1)->getStartProgLine());
	CPPUNIT_ASSERT_EQUAL(9,pkb->getProcedure(1)->getEndProgLine());

	//Test Parent
	PARENT_LIST parentList = pkb->getParent(0, 5);
	PARENT_LIST::iterator parentItr = parentList.begin();
	CPPUNIT_ASSERT(parentItr->first == 4);

	parentList = pkb->getParent(0, 7);
	parentItr = parentList.begin();
	CPPUNIT_ASSERT(parentItr->first == 4);

	//Test Follow
	CPPUNIT_ASSERT_EQUAL(6, pkb->getFollows(0, 7).begin()->first);
	CPPUNIT_ASSERT_EQUAL(1, pkb->getFollows(0, 2).begin()->first);
	CPPUNIT_ASSERT_EQUAL(3, pkb->getFollows(2, 0).begin()->second);
	CPPUNIT_ASSERT(pkb->getFollows(4, 0).empty() == true);

	//Test VarTable
	VAR_LIST varList = *pkb->getAllVar();
	VAR_LIST::iterator varTableItr = varList.begin();
	CPPUNIT_ASSERT(*(varTableItr++) == "a");
	CPPUNIT_ASSERT(*(varTableItr++) == "y");
	CPPUNIT_ASSERT(*(varTableItr++) == "hhhhh");
	CPPUNIT_ASSERT(*(varTableItr++) == "x");

	//Test Modifies
	MODIFIES_LIST modifiesList = pkb->getModifies(PROCEDURE, 1, 0);
	MODIFIES_LIST::iterator modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "a");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "x");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "b");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "hhhhh");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "k");

	//Test Modifies While LOOP
	modifiesList = pkb->getModifies(WHILE, 4, 0);
	modifiesListItr = modifiesList.begin();
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "x");
	CPPUNIT_ASSERT( pkb->getVarName((modifiesListItr++->second)) == "hhhhh");

	//Test Uses
	USES_LIST useList = pkb->getUses(PROCEDURE, 1, 0);
	CPPUNIT_ASSERT(useList.size() == 5);

	//Test Uses WHILE LOOP
	useList = pkb->getUses(WHILE, 4, 0);
	CPPUNIT_ASSERT(useList.size() == 3);

	// Test AST
	AST *ast = pkb->getRootAST();
	CPPUNIT_ASSERT(ast->getRootData() == 1);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()) == VARIABLE);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getFirstDescendant()->getFirstDescendant()->getRightSibling()) == PLUS);
	
	ast = ast->getFirstDescendant()->getFirstDescendant()->getRightSibling();
	CPPUNIT_ASSERT(pkb->getType(ast) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == WHILE);
	
	ast = ast->getRightSibling()->getRightSibling();
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getRightSibling()) == STMT_LIST);
	CPPUNIT_ASSERT(pkb->getType(ast->getFirstDescendant()->getRightSibling()->getFirstDescendant()) == ASSIGNMENT);

	ast = ast->getFirstDescendant()->getRightSibling()->getFirstDescendant();
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()) == ASSIGNMENT);
	CPPUNIT_ASSERT(pkb->getType(ast->getRightSibling()->getRightSibling()) == WHILE);

}
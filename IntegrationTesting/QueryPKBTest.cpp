#include "QueryPKBTest.h"
#include <cppunit/config/SourcePrefix.h>
#include "TestCases.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/QueryPreprocessor.h"
#include "../SPA/QueryFormatter.h"
#include "../SPA/PKB.h"
#include "../SPA/SPAController.h"

using namespace std;
void QueryPKBTest::setUp()
{
}


void  QueryPKBTest::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION(  QueryPKBTest );

void QueryPKBTest::testSampleInput1(){
	SPAController spa;

	spa.enterProgram(SampleSimpleSources::sampleSource5);
	

	FINAL_RESULT result =spa.getQueryResult(SampleQueries::sampleQuery43);
	FINAL_RESULT::iterator itr;

	cout<<"\n Final result is:";
	for(itr = result.begin();itr!=result.end();itr++){
		cout<< *itr <<" "  ;
	}
/*	//create PKB
	PKB *pkb = SamplePKB::pkb5();

	// test for query below
	
	QueryPreprocessor queryProcessor(pkb);
	QueryEvaluator queryEval(pkb);
	QueryFormatter queryFormatter;
	queryFormatter.setPKB(pkb);

	//load sample query
	//queryProcessor.setQuery(SampleQueries::sampleQuery1);
	//queryProcessor.setQuery(SampleQueries::sampleQuery2);
	//queryProcessor.setQuery(SampleQueries::sampleQuery3);

//	queryProcessor.setQuery(SampleQueries::sampleQuery14);

	queryProcessor.setQuery(SampleQueries::sampleQuery33);

	//process & evaluate
	if(queryProcessor.preProcess()==true){
		QTREE * rootQtree;
		rootQtree = queryProcessor.getQTree();

		QUERYTABLE  *queryTable;
		queryTable = queryProcessor.getQVarTable();

	//	cout<<"queryTableSize B: "<<queryTable.size();
		queryEval.evaluate(rootQtree,queryTable);
*/
		//print the result

	
	
	
}

void QueryPKBTest::testSampleInput2(){
	/*SPAController spa;
	spa.enterProgram(SampleSimpleSources::sampleSource4);
	

	FINAL_RESULT result =spa.getQueryResult("Select BOOLEAN such that Uses(2,\"a\");");
	FINAL_RESULT::iterator itr;

	cout<<"\n Final result is:";
	for(itr = result.begin();itr!=result.end();itr++){
		cout<< *itr <<" "  ;
	}*/
	
}
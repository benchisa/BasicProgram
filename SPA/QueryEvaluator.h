#pragma once
#include "Global_Data_Abstraction.h"
#include "QTREE.h"
#include "Pattern.h"
#include <hash_map>
#include "IntermediateResultTable.h"
#include "SuchThatClause.h"


class QueryEvaluator{
public:

	//methods
	QueryEvaluator(PKB* pkb);
	~QueryEvaluator(void);

	bool evaluate(QTREE* qrTree,QUERYTABLE* qrTable,QUERYPARAM* qrParam);
	RAWDATA* getResult();
//	ERROR_MSG getLastError();
	

private:
	QUERYTABLE * qrTable;
	QTREE* qrTree;
	QUERYPARAM * qrParam;
	QTREE* resultNode;
	PKB * pkb;
	TYPE resultType;
	INDEX resultIndex;
	Pattern patProcessor;

    INDEX_LIST * returnList;
	RAWDATA* rawData;
	
	IntermediateResultTable * computeIntermediateResult(QTREE* relationTree);
	IntermediateResultTable * evaluateClause(IntermediateResultTable * resultTable, QTREE* clause);
	bool executeSuchThat(IntermediateResultTable * resultTable,QTREE* suchThatTree);
	bool findResult(QTREE * resultNode,IntermediateResultTable * resultTable);
	RELATION_LIST * appendRelationLists(RELATION_LIST* list1,RELATION_LIST* list2);

	    
};
#pragma once
#include "Global_Data_Abstraction.h"
#include "QTREE.h"
#include <hash_map>
#include "IntermediateResultTable.h"
#include "SuchThatClause.h"
#include "WithClause.h"
#include "Pattern.h"
#include "DesignExtractor.h"
#include "SuchThat.h"
#include <iostream>


class QueryEvaluator{
public:

	//methods
	QueryEvaluator(PKB* pkb,DesignExtractor * de);
	~QueryEvaluator(void);

	bool evaluate(QTREE* qrTree,QUERYTABLE* qrTable,QUERYPARAM* qrParam);
	FINALRAW * getRawResult();
	DATA_LIST * getSelectedVars();
//	ERROR_MSG getLastError();
	

private:
	QUERYTABLE * qrTable;
	QTREE* qrTree;
	QUERYPARAM * qrParam;
	QTREE* resultNode;
	PKB * pkb;
	TYPE resultType;
	INDEX resultIndex;
	FINALRAW * rawData;
	DesignExtractor * extractor;
	IntermediateResultTable * resultTable ;
	DATA_LIST * resultVarList;

	IntermediateResultTable * computeIntermediateResult(QTREE* relationTree);
	IntermediateResultTable * evaluateClause(IntermediateResultTable * resultTable, QTREE* clause);
	bool executeSuchThat(IntermediateResultTable * resultTable,QTREE* suchThatTree);
	bool executeWith(IntermediateResultTable * resultTable,QTREE* withTree);
	bool executePattern(IntermediateResultTable * resultTable,QTREE* withTree);
	bool findResult(QTREE * resultNode,IntermediateResultTable * resultTable);
	void generateRaw(QTREE* resultNode );
	RELATION_LIST * appendRelationLists(RELATION_LIST* list1,RELATION_LIST* list2);
	void replaceValue(QTREE* probRel,QTREE* replaceRel,INDEX_LIST * probedList);
	    
};
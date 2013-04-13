#pragma once
#include "Global_Data_Abstraction.h"
#include "LinkedDataTable.h"
#include "PKB.h"
#include "DesignExtractor.h"
class IntermediateResultTable
{
public:
	IntermediateResultTable(SIZEX tableSize,PKB* pkb,QUERYTABLE* qrTable,DesignExtractor * extractor);
	~IntermediateResultTable(void);
	
	//if joinAttr is 0, both of the two relations are not in the table
	//if joinAttr is 1,
	//if joinAttr is 2,
	bool joinList(JOIN_ATTR qrVarIndex,INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * newList);
	bool isQrVarDiscovered(INDEX qrVarIndex);
	INDEX_LIST* getResultListOf(INDEX qrVarIndex);
	FINALRAW* findResultOf(DATA_LIST resultNodeList);
	DATA_LIST * getStmtListOf(TYPE nodeType);

private:
	typedef list<LinkedDataTable> DATABASE;
	typedef multimap<int,int> GROUP;
	typedef pair<int,int> valuePair;

	SIZEX tableSize;
	//an array to store the qr variables
	int ** qrVarTable;
	PKB * pkb;
	QUERYTABLE * qrTable;

	DATABASE database;
	RAWDATA * joinRaw(RAWDATA * rawData,int tableNum,DATA_LIST * selectedVarList);
	RAWDATA * joinCombinations(RAWDATA* rawData, DATA_LIST * selectedVarList);
	FINALRAW * uniqueRaw(RAWDATA* currentRaw);
	RAWDATA * IntermediateResultTable::simplifyRaw(RAWDATA * currentRaw);
	DesignExtractor * extractor;
};


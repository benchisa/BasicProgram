#pragma once
#include "Global_Data_Abstraction.h"
#include "LinkedDataTable.h"
#include "PKB.h"
#include "DesignExtractor.h"
class IntermediateResultTable
{
public:
	IntermediateResultTable(SIZE tableSize,PKB* pkb,QUERYTABLE* qrTable,DesignExtractor * extractor);
	~IntermediateResultTable(void);
	
	//if joinAttr is 0, both of the two relations are not in the table
	bool joinList(JOIN_ATTR qrVarIndex,INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * newList);
	bool isQrVarDiscovered(INDEX qrVarIndex);
	INDEX_LIST* getResultListOf(INDEX qrVarIndex);
	RAWDATA* findResultOf(DATA_LIST resultNodeList);


private:
	typedef list<LinkedDataTable> DATABASE;
	typedef multimap<int,int> GROUP;
	typedef pair<int,int> valuePair;

	SIZE tableSize;
	//an array to store the qr variables
	int ** qrVarTable;
	PKB * pkb;
	QUERYTABLE * qrTable;

	DATABASE database;
	RAWDATA * joinRaw(RAWDATA * rawData,int tableNum,DATA_LIST * selectedVarList);
	RAWDATA * joinCombinations(RAWDATA* rawData, DATA_LIST * selectedVarList);
	DATA_LIST * getStmtListOf(TYPE nodeType);
	DesignExtractor * extractor;
};


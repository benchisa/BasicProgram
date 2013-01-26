#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"
#include "DesignExtractor.h"

class WithTree
{
public:
	WithTree(PKB * pkb,DesignExtractor* extractor);
	~WithTree(void);
	
	//typedef hash_map<int,string> QUERYVALUE; //to be changed

	RELATION_LIST* evaluateWithTree(QTREE* qrTree,QUERYTABLE* qrTable,QUERYPARAM* qrParam);

private:
	PKB* pkb;
	DesignExtractor* extractor;
	QUERYTABLE* qrTable;
	QUERYPARAM* qrParam;
	QTREE* withTree;
	QTREE* leftVariable;
	QTREE* rightVariable;
	QTREE* rightAttribute;

	void setQueryTable(QUERYTABLE* qrTable);
	void setQueryValue(QUERYPARAM* qrParam);
	RELATION_LIST* evaluateWith(QTREE* withTree);
	
	RELATION_LIST* findLeftVariable();
	RELATION_LIST* findLeftString();
	RELATION_LIST* findLeftInteger();

	RELATION_LIST* findMatchedPairs();

};


#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"
#include "DesignExtractor.h"

class WithTree
{
public:
	WithTree(void);
	~WithTree(void);
	
	typedef hash_map<int,string> QUERYVALUE; //to be changed

	RELATION_LIST* evaluateWithTree(QTREE* qrTree,QUERYTABLE* qrTable,QUERYVALUE* qrValue);

private:
	PKB* pkb;
	QUERYTABLE* qrTable;
	QUERYVALUE* qrValue;
	QTREE* withTree;
	QTREE* leftVariable;
	QTREE* rightVariable;
	QTREE* rightAttribute;

	void setQueryTable(QUERYTABLE* qrTable);
	void setQueryValue(QUERYVALUE* qrValue);
	RELATION_LIST* evaluateWith(QTREE* withTree);
	
	RELATION_LIST* findLeftVariable();
	RELATION_LIST* findLeftString();
	RELATION_LIST* findLeftInteger();

	RELATION_LIST* findMatchedPairs();

};


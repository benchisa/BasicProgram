#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"
class WithTree
{
public:
	WithTree(void);
	~WithTree(void);
	
	typedef QUERYTABLE QUERYVALUE; //to be changed

	RELATION_LIST* evaluateWithTree(QTREE* qrTree,QUERYTABLE* qrTable,QUERYVALUE* qrValue);

private:
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
	RELATION_LIST* findMatchedPairs(QTREE* leftVariable,QTREE* rightVariable);

};


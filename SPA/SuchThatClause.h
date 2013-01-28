#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"

class SuchThatClause
{
public:
	SuchThatClause(PKB* pkb,QUERYTABLE* qrTable);
	~SuchThatClause(void);

	RELATION_LIST* evaluateSuchThatTree(QTREE* suchThatTree);
	
private:
	PKB * pkb;
	QTREE * suchThatTree;
	QUERYTABLE* qrTable;
	RELATION_LIST* evaluateSuchThat();
	void iterateAndStore(RELATION_LIST *result, RELATION_LIST list);
};


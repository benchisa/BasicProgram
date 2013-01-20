#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"

class SuchThatTree
{
public:
	SuchThatTree(PKB* pkb);
	~SuchThatTree(void);

	RELATION_LIST* evaluateSuchThatTree(QTREE* suchThatTree,QUERYTABLE* qrTable);
	
private:
	PKB * pkb;
	QTREE * suchThatTree;
	QUERYTABLE* qrTable;
	RELATION_LIST* evaluateSuchThat();
	void iterateAndStore(RELATION_LIST *result, RELATION_LIST list);
};


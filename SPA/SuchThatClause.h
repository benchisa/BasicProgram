#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"
#include "DesignExtractor.h"
#include "SuchThat.h"

class SuchThatClause
{
public:
	SuchThatClause(PKB* pkb,QUERYTABLE* qrTable,DesignExtractor * extractor);
	~SuchThatClause(void);

	RELATION_LIST* evaluateSuchThatTree(QTREE* suchThatTree);
	
private:
	PKB * pkb;
	DesignExtractor * extractor;
	QTREE * suchThatTree;
	QUERYTABLE* qrTable;
	RELATION_LIST* evaluateSuchThat();
	void iterateAndStore(RELATION_LIST *result, RELATION_LIST list);
	void filterResult(RELATION_LIST * result,RELATION_LIST list,TYPE cond1,TYPE cond2);

};


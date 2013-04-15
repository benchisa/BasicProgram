#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"
#include "DesignExtractor.h"
#include "SuchThat.h"

class SuchThatClause
{
public:
	SuchThatClause(void);
	~SuchThatClause(void);
	
	
	static PKB* pkb;
	static QUERYTABLE* qrTable;
	static DesignExtractor * extractor;
	static RELATION_LIST* evaluateSuchThatTree(QTREE* suchThatTree);
	
private:
	
	static CACHE cache;
	static QTREE * suchThatTree;
	static RELATION_LIST * evaluateSuchThat();
	static void iterateAndStore(RELATION_LIST *result, RELATION_LIST list);
	static void filterResult(RELATION_LIST * result,RELATION_LIST list,TYPE cond1,TYPE cond2);

};


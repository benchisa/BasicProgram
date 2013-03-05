#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "AST.h"
#include "QTREE.h"
#include "PKB.h"
#include "DesignExtractor.h"
#include <iostream>

class Pattern
{

public:
	
	Pattern(PKB* pkb);
	~Pattern(void);
	//static PKB *pkb;
	//void setPKB(PKB* p);
	RELATION_LIST evaluatePattern(QTREE * qtree, QUERYTABLE * qtable, QUERYPARAM * param);
private:
	PKB *p;
	AST *ast;
	VAR_LIST *varList;
	RELATION_LIST rlist;
};


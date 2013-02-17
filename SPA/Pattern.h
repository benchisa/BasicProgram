#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "AST.h"
#include "QTREE.h"
#include "PKB.h"

class Pattern
{

public:
	
	Pattern(PKB* pkb);
	~Pattern(void);
	//static PKB *pkb;
	//void setPKB(PKB* p);
	RELATION_LIST* evaluatePattern(QUERYTABLE * qtable);
private:
	PKB *p;
	AST *ast;
	VAR_LIST *varList;
	RELATION_LIST* list;
};


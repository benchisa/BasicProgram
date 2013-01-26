#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"

class DesignExtractor
{
public:
	DesignExtractor(PKB* pkb);
	~DesignExtractor(void);

	bool isStatementTypeOf(TYPE typeName, STATEMENT_NUM stmtNum);
	STATEMENT_LIST * getAllCallStmts();
	STATEMENT_LIST * getAllAssigns();
	STATEMENT_LIST * getAllWhiles();
	STATEMENT_LIST * getAllIfs();

private:
	PKB* pkb;
};


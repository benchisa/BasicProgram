#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "AST.h"
#include "QTREE.h"

class Pattern
{

public:
	
	Pattern(void);
	~Pattern(void);
	
	void setPKB(PKB* p);
	//type can be while or assign
	bool isPattern(TYPE type,VAR_INDEX index,QTREE* expression);
	STATEMENT_LIST* getPatternStmt(TYPE type,VAR_INDEX index,QTREE* expression);
private:
	STATEMENT_LIST* getPatternAssign(VAR_INDEX index,QTREE* Expression);
	STATEMENT_LIST* getPatternWhile(VAR_INDEX index);
	bool isPatternAssign(VAR_INDEX index,QTREE* expression);
	bool isPatternWhile(VAR_INDEX index);
	bool isSubTree(AST* rootTree, QTREE* subTree);

	PKB *pkb;
};


#pragma once
#include "EvaluateCalls.h"
#include "EvaluateFollows.h"
#include "EvaluateModifies.h"
#include "EvaluateNext.h"
#include "EvaluateParents.h"
#include "EvaluateUses.h"
#include "EvaluateContains.h"
#include "Affects.h"
#include "Helper.h"
#include "CFG.h"
#include "PKB.h"
#include <stack>

class DesignExtractor
{
public:
	DesignExtractor(PKB* pkb);
	~DesignExtractor(void);

	static PREFIXEXPR	convertExprToPrefix(EXPRESSION expr);
	
	bool			isStatementTypeOf(TYPE typeName, STATEMENT_NUM stmtNum);
	DATA_LIST *		getAllCallStmts();
	DATA_LIST *		getAllConstants();
	DATA_LIST *		getStmtListOf(TYPE nodeType);

	// NEXT
	void			createCFG();
	bool			isNextResult(PROG_LINE, PROG_LINE);
	bool			isNextStarResult(PROG_LINE, PROG_LINE);
	NEXT_LIST		getNextResult(PROG_LINE, PROG_LINE);
	NEXT_LIST		getNextStarResult(PROG_LINE, PROG_LINE);

	// CALL
	CALL_LIST		getCallResult(PROC_NAME caller, PROC_NAME callee);
	bool			getIsCallResult(PROC_NAME caller, PROC_NAME callee);
	CALL_LIST		getCallStarResult(PROC_NAME caller, PROC_NAME callee);
	bool			getIsCallStarResult(PROC_NAME caller, PROC_NAME callee);
	
	//from such that, need revamp
	FOLLOWS_LIST	getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	FOLLOWS_LIST	getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	FOLLOWS_LIST	getFollowsResult(TYPE type1, TYPE type2);
	FOLLOWS_LIST	getFollowsStarResult(TYPE type1, TYPE type2);
	bool			getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	bool			getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);

	PARENT_LIST		getParentResult(TYPE type1, TYPE type2);
	PARENT_LIST		getParentStarResult(TYPE type1, TYPE type2);
	PARENT_LIST		getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	PARENT_LIST		getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	bool			getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	bool			getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);

	MODIFIES_LIST	getModifiesResult(TYPE type, int arg1, VAR_INDEX v1);
	bool			getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1);

	USES_LIST		getUsesResult(TYPE type, int arg1, VAR_INDEX v1);
	bool			getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1);

	bool			getIsAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	AFFECT_LIST		getAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);

	bool			getIsContainResult(TYPE, int, TYPE, int);

private:
	PKB *			pkb;
};

#pragma once
#include "Helper.h"
#include "Global_Data_Abstraction.h"
#include <stacks>
class EvaluateCalls
{
public:
	EvaluateCalls(void);
	~EvaluateCalls(void);

	static PKB*				EvaluateCalls::pkb;
	static CALL_LIST		getCallStarResult(PROC_NAME caller, PROC_NAME callee);
	static bool				getIsCallStarResult(PROC_NAME caller, PROC_NAME callee);
	static bool				getIsCallResult(PROC_NAME caller, PROC_NAME callee);
	static MODIFIES_LIST	computeCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex);
	static bool				isCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex);
	static bool				isCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex);
	static USES_LIST		computeCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex);
	static void				insertProcModifiesUses();


private:
	static list<string>		computeCallStar(PROC_NAME caller, PROC_NAME callee);	
};


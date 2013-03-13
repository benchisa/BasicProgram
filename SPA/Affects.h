#pragma once
#include "Global_Data_Abstraction.h"
#include "Helper.h"
#include "EvaluateNext.h"

class Affects
{
public:
	Affects(void);
	~Affects(void);

	static PKB *		pkb;

	static bool			getIsAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static AFFECT_LIST	getAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);

private:
	
	static bool			computeIsAffect(int starting, int ending, int varIndex);

};
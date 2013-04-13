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
	static AFFECT_LIST		getAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static	bool 			getIsAffectStarResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static AFFECT_LIST	getAffectStarResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);

private:
	
	static bool			computeIsAffect(int starting, int ending, int varIndex);
	static bool         		computeIsAffectStar(int starting, int ending); 
	static unsigned __stdcall  computeGetAffect(void* pParam);
	static unsigned __stdcall  computeGetAffectStar(void* pParam);
	struct affectParam
	{
		int starting;
		int ending;
		AFFECT_LIST *result;
		
		
	};


};

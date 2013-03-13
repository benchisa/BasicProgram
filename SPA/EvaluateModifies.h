#pragma once
#include "Global_Data_Abstraction.h"
#include "Helper.h"
#include "EvaluateCalls.h"

class EvaluateModifies
{
public:
	EvaluateModifies(void);
	~EvaluateModifies(void);
	
	static PKB *			pkb;
	static MODIFIES_LIST	getModifiesResult(TYPE type, int arg1, VAR_INDEX v1);
	static bool				getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1);

};


#pragma once
#include "Helper.h"
#include "Global_Data_Abstraction.h"
class EvaluateUses
{
public:
	EvaluateUses(void);
	~EvaluateUses(void);

	static PKB *		pkb;
	static USES_LIST	getUsesResult(TYPE type, int arg1, VAR_INDEX v1);
	static bool			getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1);
};


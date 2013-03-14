#pragma once
#include "Global_Data_Abstraction.h"
#include "Helper.h"

class EvaluateContains
{
public:
	EvaluateContains(void);
	~EvaluateContains(void);

	static PKB *			pkb;
	static bool				getIsContainResult(TYPE, int, TYPE, int);
	static bool				getIsContainStarResult(TYPE, int, TYPE, int);
	static CONTAIN_LIST		getContainResult(TYPE, int, TYPE, int);
	static CONTAIN_LIST		getContainStarResult(TYPE, int, TYPE, int);


private:
	static void				computeContain();
	static void				computeContainStar();
};


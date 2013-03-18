#pragma once
#include "Global_Data_Abstraction.h"
#include "Helper.h"
#include "EvaluateFollows.h"

class EvaluateSibling
{
public:
	EvaluateSibling(void);
	~EvaluateSibling(void);

	static PKB * pkb;

	static bool getIsSibling(TYPE type1,TYPE type2,INDEX index1,INDEX index2);
	static RELATION_LIST getSiblingResult(TYPE type1,TYPE type2,INDEX index1,INDEX index2);

private:
	static RELATION_LIST getOneSibling(TYPE known,TYPE unknown,INDEX knownIndex,int knownPos);
	static RELATION_LIST getSiblingUnknown(TYPE type1,TYPE type2);
	static int getOperatorValue (TYPE operater);
};


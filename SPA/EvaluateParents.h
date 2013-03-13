#pragma once
#include "Helper.h"
#include "Global_Data_Abstraction.h"
class EvaluateParents
{
public:
	EvaluateParents(void);
	~EvaluateParents(void);


	static PKB *pkb;
	static PARENT_LIST EvaluateParents::getParentResult(TYPE type1, TYPE type2);
	static PARENT_LIST EvaluateParents::getParentStarResult(TYPE type1, TYPE type2);
	static PARENT_LIST EvaluateParents::getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static PARENT_LIST EvaluateParents::getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static bool EvaluateParents::getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	static bool EvaluateParents::getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);

private:
	static void EvaluateParents::computeParent(PARENT_LIST &result,PARENT_LIST tmpLst, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static FOLLOWS_LIST EvaluateParents::getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static list<int> EvaluateParents::computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
};


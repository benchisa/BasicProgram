#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "Helper.h"
#include <stack>

class EvaluateFollows
{
public:
	EvaluateFollows(void);
	~EvaluateFollows(void);

	static PKB* pkb;
	static FOLLOWS_LIST EvaluateFollows::getFollowsResult(TYPE type1, TYPE type2);
	static FOLLOWS_LIST EvaluateFollows::getFollowsStarResult(TYPE type1, TYPE type2);
	static FOLLOWS_LIST EvaluateFollows::getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static FOLLOWS_LIST EvaluateFollows::getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static bool EvaluateFollows::getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	static bool EvaluateFollows::getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);


private:
	static FOLLOWS_LIST EvaluateFollows::getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static list<int> computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static void computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	
};


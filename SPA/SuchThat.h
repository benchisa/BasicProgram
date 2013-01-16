#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"

class SuchThat
{
public:
	static PKB *pkb;

	static FOLLOWS_LIST getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static FOLLOWS_LIST getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static FOLLOWS_LIST getFollowsResult(TYPE type1, TYPE type2);
	static FOLLOWS_LIST getFollowsStarResult(TYPE type1, TYPE type2);
	static bool getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	static bool getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);

	static PARENT_LIST getParentResult(TYPE type1, TYPE type2);
	static PARENT_LIST getParentStarResult(TYPE type1, TYPE type2);
	static PARENT_LIST getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static PARENT_LIST getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static bool getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	static bool getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);

	static MODIFIES_LIST getModifiesResult(TYPE type, int arg1, VAR_INDEX v1);
	static bool getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1);

	static USES_LIST getUsesResult(TYPE type, int arg1, VAR_INDEX v1);
	static bool getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1);

private:
	SuchThat(void);
	~SuchThat(void);
private:
	// computational methods
	static void computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &listLst, TYPE type1, TYPE type2);
	static void computeParent(PARENT_LIST &result, PARENT_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static void computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	static PARENT_LIST getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static void computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result);
	static FOLLOWS_LIST getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	static void computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result);
	
	
	static void iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmp, int v1);
	
	
};

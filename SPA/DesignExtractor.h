#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"

class DesignExtractor
{
public:
	DesignExtractor(PKB* pkb);
	~DesignExtractor(void);

	bool isStatementTypeOf(TYPE typeName, STATEMENT_NUM stmtNum);
	STATEMENT_LIST * getAllCallStmts();
	STATEMENT_LIST * getAllAssigns();
	STATEMENT_LIST * getAllWhiles();
	STATEMENT_LIST * getAllIfs();

	//createCFG and store in PKB
	void createCFG();
	bool isNext(PROG_LINE, PROG_LINE);
	bool isNextStar(PROG_LINE, PROG_LINE);
	NEXT_LIST getNext(PROG_LINE, PROG_LINE);
	NEXT_LIST getNextStar(PROG_LINE, PROG_LINE);

	//from such that, need revamp
	FOLLOWS_LIST getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 FOLLOWS_LIST getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 FOLLOWS_LIST getFollowsResult(TYPE type1, TYPE type2);
	 FOLLOWS_LIST getFollowsStarResult(TYPE type1, TYPE type2);
	 bool getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	 bool getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);

	 PARENT_LIST getParentResult(TYPE type1, TYPE type2);
	 PARENT_LIST getParentStarResult(TYPE type1, TYPE type2);
	 PARENT_LIST getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 PARENT_LIST getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 bool getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2);
	 bool getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2);

	 MODIFIES_LIST getModifiesResult(TYPE type, int arg1, VAR_INDEX v1);
	 bool getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1);

	 USES_LIST getUsesResult(TYPE type, int arg1, VAR_INDEX v1);
	 bool getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1);



private:
	PKB* pkb;
	AST* buildCFG(AST *);
	vector<PROG_LINE> cfgWhileKeepers;
	 void computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &listLst, TYPE type1, TYPE type2);
	 void computeParent(PARENT_LIST &result, PARENT_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 void computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 PARENT_LIST getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	 void computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result);
	 FOLLOWS_LIST getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	 void computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result);
	
	
	 void iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmp, int v1);
};


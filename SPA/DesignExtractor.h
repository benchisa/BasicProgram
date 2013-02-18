#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"

class DesignExtractor
{
public:
	DesignExtractor(PKB* pkb);
	~DesignExtractor(void);

	bool isStatementTypeOf(TYPE typeName, STATEMENT_NUM stmtNum);
	DATA_LIST * getAllCallStmts();
	DATA_LIST * getAllAssigns();
	DATA_LIST * getAllWhiles();
	DATA_LIST * getAllIfs();


	//createCFG and store in PKB
	void createCFG();
	bool isNextResult(PROG_LINE, PROG_LINE);
	bool isNextStarResult(PROG_LINE, PROG_LINE);
	NEXT_LIST getNextResult(PROG_LINE, PROG_LINE);
	NEXT_LIST getNextStarResult(PROG_LINE, PROG_LINE);
	
	//call table
	CALL_LIST  getCallResult(PROC_NAME caller, PROC_NAME callee);
	bool getIsCallResult(PROC_NAME caller, PROC_NAME callee);
	CALL_LIST  getCallStarResult(PROC_NAME caller, PROC_NAME callee);
	bool getIsCallStarResult(PROC_NAME caller, PROC_NAME callee);

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
	int maxStmtNum;
	int maxProgNum;
	vector<PROG_LINE> cfgWhileKeepers;
	 void computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &listLst, TYPE type1, TYPE type2);
	 void computeParent(PARENT_LIST &result, PARENT_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 void computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2);
	 PARENT_LIST getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	 void computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result);
	 FOLLOWS_LIST getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	 void computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result);
	
	 MODIFIES_LIST computeCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex);
	 bool isCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex);
	 USES_LIST computeCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex);
	 bool isCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex);

	// void iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmp, int v1);
	 void computeCallStar(PROC_NAME caller, PROC_NAME callee,list<string> &result);
	 void iterateAndStore(RELATION_LIST *result, RELATION_LIST list);
};


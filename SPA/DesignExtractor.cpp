#include "DesignExtractor.h"


DesignExtractor::DesignExtractor(PKB* pkb)
{
	this->pkb = pkb;
	// Point all the sub-components to the same pkb
	Helper::pkb = pkb;
	EvaluateCalls::pkb = pkb;
	EvaluateNext::pkb = pkb;
	EvaluateFollows::pkb = pkb;
	EvaluateUses::pkb = pkb;
	EvaluateContains::pkb = pkb;
	EvaluateParents::pkb = pkb;
	Affects::pkb = pkb;

	EvaluateNext::createCFG();
    EvaluateCalls::insertProcModifiesUses();
}


DesignExtractor::~DesignExtractor(void)
{
	//delete cfg;
}

// ============== HELPER FUNCTIONS ================/
PREFIXEXPR DesignExtractor::convertExprToPrefix(EXPRESSION expr){
	return Helper::convertExprToPrefix(expr);
}

bool DesignExtractor::isStatementTypeOf(TYPE typeName,STATEMENT_NUM stmtNum){
	return Helper::isStatementTypeOf(typeName, stmtNum);
}

DATA_LIST * DesignExtractor::getAllConstants(){
	return Helper::getAllConstants();
}

DATA_LIST * DesignExtractor::getAllCallStmts(){
	return Helper::getAllCallStmts();
}

DATA_LIST * DesignExtractor::getStmtListOf(TYPE nodeType){
	return Helper::getStmtListOf(nodeType);
}

// ============== CALL FUNCTIONS ================/

CALL_LIST DesignExtractor::getCallResult(PROC_NAME caller, PROC_NAME callee){
	return pkb->getCall(caller, callee);
}
bool DesignExtractor::getIsCallResult(PROC_NAME caller, PROC_NAME callee)
{
	return pkb->isExistsCall(caller, callee);
}

CALL_LIST DesignExtractor::getCallStarResult(PROC_NAME caller, PROC_NAME callee){
	return EvaluateCalls::getCallStarResult(caller, callee);
}

bool DesignExtractor::getIsCallStarResult(PROC_NAME caller, PROC_NAME callee){
	return EvaluateCalls::getIsCallStarResult(caller, callee);
}

// CFG and NEXT
void DesignExtractor::createCFG(){
	return EvaluateNext::createCFG();
}

bool DesignExtractor::isNextResult(PROG_LINE p1, PROG_LINE p2){
	return EvaluateNext::isNextResult(p1, p2);
}

// already stored in CFG
NEXT_LIST DesignExtractor::getNextResult(PROG_LINE p1, PROG_LINE p2){
	return EvaluateNext::getNextResult(p1, p2);
}

// on demand
bool DesignExtractor::isNextStarResult(PROG_LINE p1, PROG_LINE p2){
	return EvaluateNext::isNextStarResult(p1, p2);
}

// on demand
NEXT_LIST DesignExtractor::getNextStarResult(PROG_LINE p1, PROG_LINE p2){
	return EvaluateNext::getNextStarResult(p1, p2);
}

FOLLOWS_LIST DesignExtractor::getFollowsResult(TYPE type1, TYPE type2){
	return EvaluateFollows::getFollowsResult(type1, type2);
}

FOLLOWS_LIST DesignExtractor::getFollowsStarResult(TYPE type1, TYPE type2){
	return EvaluateFollows::getFollowsStarResult(type1, type2);
}

FOLLOWS_LIST DesignExtractor::getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	return EvaluateFollows::getFollowsResult(type, s1, s2);
}

FOLLOWS_LIST DesignExtractor::getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	return EvaluateFollows::getFollowsStarResult(type, s1, s2);
}

bool DesignExtractor::getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	return EvaluateFollows::getIsFollowsResult(s1, s2);
}

bool DesignExtractor::getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	return EvaluateFollows::getIsFollowsStarResult(s1, s2);
}



PARENT_LIST DesignExtractor::getParentResult(TYPE type1, TYPE type2){	
	return EvaluateParents::getParentResult(type1, type2);
}

PARENT_LIST DesignExtractor::getParentStarResult(TYPE type1, TYPE type2){	
	return EvaluateParents::getParentStarResult(type1, type2);
}

PARENT_LIST DesignExtractor::getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	return EvaluateParents::getParentResult(type, s1, s2);
}

PARENT_LIST DesignExtractor::getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	return EvaluateParents::getParentStarResult(type, s1, s2);
}

bool DesignExtractor::getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2){	
	return EvaluateParents::getIsParentResult(s1, s2);
}

bool DesignExtractor::getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	return EvaluateParents::getIsParentStarResult(s1, s2);
}

MODIFIES_LIST DesignExtractor::getModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	return EvaluateModifies::getModifiesResult(type, arg1, v1);
}

bool DesignExtractor::getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	return EvaluateModifies::getIsModifiesResult(type, arg1, v1);
}

USES_LIST DesignExtractor::getUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	return EvaluateUses::getUsesResult(type, arg1, v1);
}

bool DesignExtractor::getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	return EvaluateUses::getIsUsesResult(type, arg1, v1);
}

bool DesignExtractor::getIsAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2){
	return Affects::getIsAffectResult(stmt1, stmt2);
}


AFFECT_LIST DesignExtractor::getAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2){
	return Affects::getAffectResult(stmt1, stmt2);
}

bool DesignExtractor::getIsContainResult(TYPE type, int arg1, TYPE type2, int arg2){
	return EvaluateContains::getIsContainResult(type, arg1, type2, arg2);
}

bool DesignExtractor::getIsContainStarResult(TYPE type, int arg1, TYPE type2, int arg2){
	return EvaluateContains::getIsContainStarResult(type, arg1, type2, arg2);
}

CONTAIN_LIST DesignExtractor::getContainResult(TYPE type, int arg1, TYPE type2, int arg2){
	return EvaluateContains::getContainResult(type, arg1, type2, arg2);
}

CONTAIN_LIST DesignExtractor::getContainStarResult(TYPE type, int arg1, TYPE type2, int arg2){
	return EvaluateContains::getContainStarResult(type, arg1, type2, arg2);
}
#include "EvaluateContains.h"


PKB *EvaluateContains::pkb = NULL;

EvaluateContains::EvaluateContains(void)
{
}


EvaluateContains::~EvaluateContains(void)
{
}


// Contain(1,2)
// Contain(PLUS, PLUS/MINUS/TIMES/VARIABLE/CONSTANT) or similar
// Contain("procedureName", assign)
bool EvaluateContains::getIsContainResult(TYPE type, int arg1, TYPE type2, int arg2)
{
	// Contain(1,2)
	// If argument 1 is not WHILE or IF, this is false
	// Else, Check if argument 2 is directly below the WHILE or IF
	if(type == STATEMENT && type2 == STATEMENT){
		if(Helper::getStatementType(arg1) == WHILE || Helper::getStatementType(arg1) == IF){
			DATA_LIST * stmtList =pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();
			while(stmtListItr!=stmtList->end()){
				// make sure we're referring to the correct nest
				if(pkb->getStmtEntry(*stmtListItr).ownerNo == arg1 && *stmtListItr == arg2)
					return true;

				stmtListItr++;
			}
		}

		return false;
	}
	//Contain("extra", 1)
	if(type == PROCEDURE){
		if(type2 == STMT_LIST) // definitely true
			return true;
		if(type2 == STATEMENT){
			DATA_LIST * stmtList =pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();
			while(stmtListItr!=stmtList->end()){
				// make sure we're referring to the correct procedure
				if(pkb->getStmtEntry(*stmtListItr).ownerNo == arg1 && *stmtListItr == arg2)
					return true;

				stmtListItr++;
			}
		}
		return false;
	}


	// Contains(STMTLISTREF, 1)
	if(type == STMT_LIST && type2 == STATEMENT){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			if(*stmtListItr==arg2)
				return true;

			stmtListItr++;
		}

		return false;
	}

	// Contain(PLUS, "apple")
	// Contain(PLUS, "1")
	if((type == PLUS  || type == MINUS || type == MULTIPLY)&& (type2 == VARIABLE ||type2 ==  CONSTANT)){
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			if(Helper::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
				return true;
			}

			assignListItr++;
		}

		return false;
	}

	// Contain(PLUS, PLUS)
	if((type == PLUS  || type == MINUS || type == MULTIPLY)&& (type2 == PLUS ||type2 ==  MINUS || type2== MULTIPLY)){
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			if(Helper::isStatementDescendantTypeDataOf(type, type2, -1, *assignListItr)){
				return true;
			}

			assignListItr++;
		}

		return false;
	}

	// Contain("proc", 1) --> same as stmtlist_ref

}
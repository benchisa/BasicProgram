#include "EvaluateUses.h"
#include "EvaluateCalls.h"

PKB * EvaluateUses::pkb = NULL;

EvaluateUses::EvaluateUses(void)
{
}


EvaluateUses::~EvaluateUses(void)
{
}

USES_LIST EvaluateUses::getUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	USES_LIST result,tmpLst;
	USES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;

	//Uses(w, "b")
	if((type == WHILE  || type == ASSIGNMENT || type == IF || type == PROCEDURE)){
		result = pkb->getUses(type, arg1, v1);
	}
	// uses(c, 2)
	// uses(c, _)
	else if(type == CALL){
		result = EvaluateCalls::computeCallUses(arg1, v1);
	}
	// Uses(1, a)
	else if((type == STATEMENT && arg1 != 0 && v1 == 0) || (type == ANY && arg1 != 0 && v1 == 0)){
		// Find the type first
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();
		if(pkb->isExistCallStmt(arg1)){
			result = EvaluateCalls::computeCallUses(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE || pkb->getType(*astItr) == ASSIGNMENT ||pkb->getType(*astItr) == IF)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->getUses(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == ANY || (type == STATEMENT && arg1 == 0))
	{
		tmpLst = pkb->getUses(WHILE, 0, 0);
		Helper::iterateAndStore(result, tmpLst, v1);

		tmpLst = pkb->getUses(IF, 0, 0);
		Helper::iterateAndStore(result, tmpLst, v1);

		tmpLst = EvaluateCalls::computeCallUses(0,0);
		Helper::iterateAndStore(result, tmpLst, v1);

		tmpLst = pkb->getUses(ASSIGNMENT, 0, 0);
		Helper::iterateAndStore(result, tmpLst, v1);
	}

	//cout << "SIZEX OF RESULT (USES): " << result.size() << "\n";

	return result;
}

bool EvaluateUses::getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	USES_LIST usesLst;
	bool result = false;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;
	/*cout << "asdasd\n";
	cout << "TYPE: " << type << "\n";
	cout << "ARG1: " << arg1 << "\n";
	cout << "v1: " << v1 << "\n";*/

	if(type == STATEMENT && arg1 != 0 && v1 != 0){
		// Find the type of first argument
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();

		if(pkb->isExistCallStmt(arg1)){
			result = EvaluateCalls::isCallUses(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE || pkb->getType(*astItr) == ASSIGNMENT || pkb->getType(*astItr) == IF)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->isUses(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == PROCEDURE) //need to be changed and need to add call
	{
		result = pkb->isUses(PROCEDURE, arg1, v1);
	}
	return result;
}

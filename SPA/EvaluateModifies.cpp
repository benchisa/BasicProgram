#include "EvaluateModifies.h"


PKB * EvaluateModifies::pkb = NULL;

EvaluateModifies::EvaluateModifies(void)
{
}


EvaluateModifies::~EvaluateModifies(void)
{
}

MODIFIES_LIST EvaluateModifies::getModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	MODIFIES_LIST result,tmpLst;
	MODIFIES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;
	//cout << "TYPE: " << type << "\n";

	// Modifies(w, 1)
	// Modifies(w, 2)
	// Modifies(c, 2)
	if((type == WHILE  || type == ASSIGNMENT||type == IF|| type == PROCEDURE)){
		//cout << "v1: " << pkb->getVarName(v1) << "\n";
			result = pkb->getModifies(type, arg1, v1);

	}
	// modifies(c, 2)
	// modifies(c, _)
	else if(type == CALL){
		result = EvaluateCalls::computeCallModifies(arg1, v1);
	}

	// Modifies(1, _), 
	else if((type == STATEMENT && arg1 != 0 && v1 == 0
		|| type == ANY && arg1 != 0 && v1 == 0)){
		// Find the type first
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();

		if(pkb->isExistCallStmt(arg1)){
			result = EvaluateCalls::computeCallModifies(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE || pkb->getType(*astItr) == ASSIGNMENT||pkb->getType(*astItr)==IF)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->getModifies(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == ANY || (type == STATEMENT && arg1 == 0)){
		// Modifies(_,_) or Modifies(s, "x")
		tmpLst = pkb->getModifies(WHILE, 0, 0);
		Helper::iterateAndStore(result, tmpLst, v1);

		tmpLst = EvaluateCalls::computeCallModifies(0,0);
		Helper::iterateAndStore(result, tmpLst, v1);

		tmpLst = pkb->getModifies(ASSIGNMENT, 0, 0);
		Helper::iterateAndStore(result, tmpLst, v1);

		tmpLst = pkb->getModifies(IF, 0, 0);
		Helper::iterateAndStore(result, tmpLst, v1);
	}


	//cout << "SIZE OF RESULT (MODIFIES): " << result.size() << "\n";

	return result;
}

bool EvaluateModifies::getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	MODIFIES_LIST modifiesLst;
	bool result = false;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;

	//WHY dont consider assignment?
	if(type == STATEMENT && arg1 != 0 && v1 != 0){
		// Find the type of first argument
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();

		// check for stmt existence in call table
		/*if(pkb->cal){
		}*/
		if(pkb->isExistCallStmt(arg1)){
			result = EvaluateCalls::isCallModifies(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE||pkb->getType(*astItr) == IF||pkb->getType(*astItr) == ASSIGNMENT)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->isModifies(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == PROCEDURE){ //need to be change
		result = pkb->isModifies(PROCEDURE, arg1, v1);
	}


	return result;
}
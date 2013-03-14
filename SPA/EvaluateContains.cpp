#include "EvaluateContains.h"


PKB *EvaluateContains::pkb = NULL;

EvaluateContains::EvaluateContains(void)
{
}


EvaluateContains::~EvaluateContains(void)
{
}

CONTAIN_LIST EvaluateContains::getContainResult(TYPE type, int arg1, TYPE type2, int arg2){	
	// =============================== Contain(KNOWN, UNKNOWN) =========================//

	// Contain(1, s2), Contain(1, a), Contain(1, w), Contain(1, if)
	CONTAIN_LIST resultList;

	if(type == STATEMENT && arg1!=NULL && arg2 == NULL){
		// First argument cannot be assignment statement
		if(Helper::getStatementType(arg1) != ASSIGNMENT){
			// Go through all the stmt_list, find one that matches arg1, return (arg1, stmtList)
			DATA_LIST * stmtList =pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();
			while(stmtListItr!=stmtList->end()){
				// push everything
				if(pkb->getStmtEntry(*stmtListItr).ownerNo == arg1 && type2 == STATEMENT)
					resultList.push_back(make_pair(arg1, *stmtListItr));
				
				// only certain type
				if(pkb->getStmtEntry(*stmtListItr).ownerNo == arg1 && Helper::getStatementType(*stmtListItr) == type2)
					resultList.push_back(make_pair(arg1, *stmtListItr));

				stmtListItr++;
			}
		}
	}

	

	// Contain("xylo", a), Contain("xylo", s), Contain("xylo", w), Contain("xylo", if)
	if(type == PROCEDURE && arg1!=NULL && arg2 == NULL){
		// return all the first statement below procedure
		if(type2 == STATEMENT)
		{
			DATA_LIST * stmtList =pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();
			while(stmtListItr!=stmtList->end()){
				if(pkb->getStmtEntry(*stmtListItr).ownerNo == arg1)
					resultList.push_back(make_pair(arg1, *stmtListItr));
				stmtListItr++;
			}
		}
		
		if(type2 == ASSIGNMENT || type2 == WHILE || type2 == IF || type2 == CALL){
			DATA_LIST * stmtList =pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();
			while(stmtListItr!=stmtList->end()){
				if(pkb->getStmtEntry(*stmtListItr).ownerNo == arg1 &&
					Helper::getStatementType(*stmtListItr) == type2){
					resultList.push_back(make_pair(arg1, *stmtListItr));
				}
				stmtListItr++;
			}
		}
	}

	// Contain(PLUS/MINUS/VARIABLES/CONSTANT, v/c)
	if(type == PLUS || type == MINUS || type == MULTIPLY || type == VARIABLE || type == CONSTANT){
		// get all assignment, find PLUS, add all its descendant of equal type
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			DATA_LIST dataLst = EvaluateContains::getDataListTypeDescendantOf(type, type2, *assignListItr);
			DATA_LIST::iterator dataItrList = dataLst.begin();
			// push_back all the answer
			while(dataItrList != dataLst.end()){
				resultList.push_back(make_pair(type, *dataItrList));
				dataItrList++;
			}

			assignListItr++;
		}
	}

	// =============================== Contain(UNKNOWN, KNOWN) =========================//
	// Contain(s1, 2), Contain(w, 4), Contain(if, 4)
	if(type2 == STATEMENT && arg2!=NULL && arg1 == NULL){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			if(arg2 == *stmtListItr)
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, arg2));
			stmtListItr++;
		}
	}

	// Contain(a, PLUS/MINUS/MULITPLY)
	if(type == ASSIGNMENT && (type2 == PLUS || type2 == MINUS || type2 == MULTIPLY || type2 == VARIABLE || type2 == CONSTANT)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
				resultList.push_back(make_pair(*assignListItr, type2));
			}
			assignListItr++;
		}
	}

	// Contain(w, VARIABLE)
	if(type == WHILE && arg1 == 0 && type2 == VARIABLE && arg2 != NULL){
		// whileTable
		DATA_LIST * whileList = pkb->getWhiles(arg2);
		DATA_LIST::iterator whileListItr = whileList->begin();
		while(whileListItr!=whileList->end()){
			resultList.push_back(make_pair(*whileListItr, arg2));
			whileListItr++;
		}
	}
	// Contain(if VARIABLE)
	if(type == IF && arg1 == 0 && type2 == VARIABLE && arg2 != NULL){
		DATA_LIST * ifList = pkb->getIfs(arg2);
		DATA_LIST::iterator ifListItr = ifList->begin();
		while(ifListItr!=ifList->end()){
			resultList.push_back(make_pair(*ifListItr, arg2));
			ifListItr++;
		}
	}

	//Contain(PLUS/MINUS/MULTIPLY, "apple")
	if((type == PLUS  || type == MINUS || type == MULTIPLY)&& (type2 == VARIABLE ||type2 ==  CONSTANT)){
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
				resultList.push_back(make_pair(*assignListItr, arg2));
			}

			assignListItr++;
		}
	}
	// =============================== Contain(UNKNOWN, UNKNOWN) =========================//
	// Contain(s1, s2)
	if(type == STATEMENT && arg1 == 0 && type2 == STATEMENT && arg2 == 0){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			if(pkb->getStmtEntry(*stmtListItr).type != PROCEDURE){
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, *stmtListItr));
			}
			stmtListItr++;
		}
	}

	// Contain(w,w), Contain(w, if), Contain(if, if), Contain(if, while)
	// Contain(w,a), Contain(w,s), Contain(if, a), Contain(if, s)
	if((type == WHILE || type == IF) && arg1 == 0 && arg2 == 0)
	{
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			//cout << pkb->getStmtEntry(*stmtListItr).type << ", " << Helper::getStatementType(*stmtListItr) << "\n";
			if(type2 == STATEMENT && (pkb->getStmtEntry(*stmtListItr).type == type)){
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, *stmtListItr));
			}

			if(type == IF && (type2 == STATEMENT || type2 == ASSIGNMENT) && (pkb->getStmtEntry(*stmtListItr).type == THEN || pkb->getStmtEntry(*stmtListItr).type == ELSE))
			{
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, *stmtListItr));
			}

			
			if(pkb->getStmtEntry(*stmtListItr).type == type 
				&& Helper::getStatementType(*stmtListItr) == type2){
					resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, *stmtListItr));
			}
			stmtListItr++;
		}
	}
	
	return resultList;
}

// Contain(1,2)
// Contain(PLUS, PLUS/MINUS/TIMES/VARIABLE/CONSTANT) or similar
// Contain("procedureName", assign)
bool EvaluateContains::getIsContainResult(TYPE type, int arg1, TYPE type2, int arg2)
{
	// Contain(1,2)
	// If argument 1 is not WHILE or IF, this is false
	// Else, Check if argument 2 is directly below the WHILE or IF
	if(type == STATEMENT && type2 == STATEMENT && arg1 != 0 && arg2 != 0){
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

	// Contain(1, "x") -> control variable
	if(arg1 != 0 && arg2!= 0 && type == STATEMENT){
		if((Helper::getStatementType(arg1) == WHILE || Helper::getStatementType(arg1) == IF) && type2 == VARIABLE){
			DATA_LIST * assignList = pkb->getAllAssigns();
			DATA_LIST::iterator assignListItr = assignList->begin();
			while(assignListItr!=assignList->end()){
				if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
					return true;
				}
				assignListItr++;
			}
		}
		// for Contain(1, "x") where 1 refers to assignmentnode
		else if(Helper::getStatementType(arg1) == ASSIGNMENT && (type2 == PLUS  || type2 == MINUS || type2 == MULTIPLY)){
			DATA_LIST * assignList = pkb->getAllAssigns();
			DATA_LIST::iterator assignListItr = assignList->begin();
			while(assignListItr!=assignList->end()){
				if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
					return true;
				}

				assignListItr++;
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
			if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
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
			if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, -1, *assignListItr)){
				return true;
			}

			assignListItr++;
		}

		return false;
	}
}



// **** PRIVATE

// return descendant's  data
DATA_LIST EvaluateContains::getDataListTypeDescendantOf(TYPE typeName, TYPE typeName2, STATEMENT_NUM stmtNum){
	DATA_LIST resultLst;

	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){
		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			// found type 1
			if((*itr)->getRootType()==typeName) {
				// find type 2
				if(pkb->getType((*itr)->getFirstDescendant()) == typeName2){
					resultLst.push_back((*itr)->getFirstDescendant()->getRootData());
				}

				if(pkb->getType((*itr)->getFirstDescendant()->getRightSibling()) == typeName2){
					resultLst.push_back((*itr)->getFirstDescendant()->getRightSibling()->getRootData());
				}
			}
		}
	}

	return resultLst;
}

// return stmt# of ancestor
DATA_LIST EvaluateContains::getDataListTypeAncestorOf(TYPE typeName, TYPE typeName2, STATEMENT_NUM stmtNum){
	DATA_LIST resultLst;

	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){
		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			// found type 1
			if((*itr)->getRootType()==typeName) {
				// find type 2
				cout << "as: " << stmtNum << "\n";
				if(pkb->getType((*itr)->getFirstDescendant()) == typeName2){
					resultLst.push_back(stmtNum);
				}

				if(pkb->getType((*itr)->getFirstDescendant()->getRightSibling()) == typeName2){
					resultLst.push_back(stmtNum);
				}
			}
		}
	}

	return resultLst;
}


// only for PLUS, MINUS, TIMES
bool EvaluateContains::isStatementDescendantTypeDataOf(TYPE typeName, TYPE typeName2, int data, STATEMENT_NUM stmtNum){
	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){
		//cout << typeName << ", " << typeName2 << ", " << data << ", " << stmtNum << "\n";
		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName){
				// check descendant
				AST * descendant = (*itr)->getFirstDescendant();
				if(pkb->getType(descendant) == typeName2 && pkb->getData(descendant) == data){
					return true;
				}
				else if(pkb->getType(descendant->getRightSibling()) == typeName2 && pkb->getData(descendant->getRightSibling()) == data){{
					return true;
				}
				}
			}
		}
	}
	
	return false;
}
#include "EvaluateContains.h"


PKB *EvaluateContains::pkb = NULL;

EvaluateContains::EvaluateContains(void)
{
}


EvaluateContains::~EvaluateContains(void)
{
}
CONTAIN_LIST EvaluateContains::getContainStarResult(TYPE type, int arg1, TYPE type2, int arg2){
	// =============================== Contain*(KNOWN, UNKNOWN)/Contain*(UNKNOWN, KNOWN) =========================//
	CONTAIN_LIST resultList;
	
	//============================================= start OF STATEMENT =======================================/

	// Contain*(1, s2), Contain*(1, a), Contain*(1, w), Contain*(1, if), Contain*(1, call)
	// Contain*(s, 2), Contains*(a, 2), Contains*(2, 2), Contains*(if, 2), 
	if((type == STATEMENT && arg1 != 0 && (type2 == STATEMENT || type2 == WHILE || type2 == IF || type2 == ASSIGNMENT || type2 == CALL)) ||
		(type2 == STATEMENT && arg2 != 0 && (type == STATEMENT || type == WHILE || type == IF || type == ASSIGNMENT))){
		PARENT_LIST parentList = EvaluateParents::getParentStarResult(type, type2);
		PARENT_LIST::iterator parentListItr = parentList.begin();
		while(parentListItr != parentList.end()){
			if(arg1!= 0 && parentListItr->first == arg1)
				resultList.push_back(make_pair(arg1, parentListItr->second));
			else if(arg2!= 0 && parentListItr->second == arg2)
				resultList.push_back(make_pair(parentListItr->first, arg2));
			parentListItr++;
		}
	}

	else if((type == STATEMENT && Helper::getStatementType(arg1) == ASSIGNMENT && arg1 != 0 && arg2 == 0 && (type2 == VARIABLE || type2 == CONSTANT))){
		string prefix = pkb->getAssignEntry(arg1).prefixTree;
		
		if(type2 == CONSTANT){
			DATA_LIST allConstants = EvaluateContains::getPrefixTreeConstants(prefix, 0);
			DATA_LIST::iterator cItr = allConstants.begin();
			while(cItr != allConstants.end()){
				resultList.push_back(make_pair(arg1, *cItr));
				cItr++;
			}
		}

		else if(type2 == VARIABLE){
			DATA_LIST allVariables = EvaluateContains::getPrefixTreeVarIndexes(prefix, 0);
			DATA_LIST::iterator vItr = allVariables.begin();
			while(vItr != allVariables.end()){
				resultList.push_back(make_pair(arg1, *vItr));
				vItr++;
			}
		}
		else{
			DATA_LIST allOperators = EvaluateContains::getPrefixTreeOperators(prefix, type2, 0);
			DATA_LIST::iterator allOperatorsItr = allOperators.begin();
			while(allOperatorsItr != allOperators.end()){
				resultList.push_back(make_pair(arg1, *allOperatorsItr));
				allOperatorsItr++;
			}
		}
	}

	// Contain*(1, v), Contain*(1, c): if 1 is while or if
	else if(type == STATEMENT && (Helper::getStatementType(arg1) == WHILE || Helper::getStatementType(arg1) == IF) && arg1 != 0 && arg2 == 0  && (type2 == VARIABLE || type2 == CONSTANT || type2 == PLUS || type2 == MINUS || type2 == MULTIPLY)){
		PARENT_LIST parentList;

		// Get Parent* (1, x)
		if(Helper::getStatementType(arg1) == WHILE)
			parentList = EvaluateParents::getParentStarResult(WHILE, arg1, 0);
		if(Helper::getStatementType(arg1) == IF)
			parentList = EvaluateParents::getParentStarResult(IF, arg1, 0);
		PARENT_LIST::iterator parentListItr = parentList.begin();

		while(parentListItr != parentList.end()){
			if(parentListItr->first = arg1){
				// check and add contorl variable
				if(Helper::getStatementType(arg1) == WHILE &&type2 == VARIABLE){
					resultList.push_back(make_pair(arg1, pkb->getWhileCtrVar(arg1)));
				}
				
				else if(Helper::getStatementType(arg1) == IF &&type2 == VARIABLE){
					
					resultList.push_back(make_pair(arg1, pkb->getIfCtrVar(arg1)));
				}
				
				// check child
				if(Helper::getStatementType(parentListItr->second) == WHILE){
					if(type2 == VARIABLE){
						resultList.push_back(make_pair(arg1, pkb->getWhileCtrVar(parentListItr->second)));
					}
				}
				else if(Helper::getStatementType(parentListItr->second) == IF){
					
					if(type2 == VARIABLE){
						resultList.push_back(make_pair(arg1, pkb->getIfCtrVar(parentListItr->second)));
					}
				}
				else{
					string prefix = pkb->getAssignEntry(parentListItr->second).prefixTree;
					
					if(type2 == CONSTANT){
						DATA_LIST allConstants = EvaluateContains::getPrefixTreeConstants(prefix,0);
						DATA_LIST::iterator cItr = allConstants.begin();
						while(cItr != allConstants.end()){
							resultList.push_back(make_pair(arg1, *cItr));
							cItr++;
						}
					}

					else if(type2 == VARIABLE){
						DATA_LIST allVariables = EvaluateContains::getPrefixTreeVarIndexes(prefix,0);
						DATA_LIST::iterator vItr = allVariables.begin();
						while(vItr != allVariables.end()){
							resultList.push_back(make_pair(arg1, *vItr));
							vItr++;
						}
					}
					else{
						DATA_LIST allOperators = EvaluateContains::getPrefixTreeOperators(prefix, type2, 0);
						DATA_LIST::iterator allOperatorsItr = allOperators.begin();
						while(allOperatorsItr != allOperators.end()){
							
							resultList.push_back(make_pair(arg1, *allOperatorsItr));
							allOperatorsItr++;
						}
					}
				}
				
			}
			parentListItr++;
		}
		
		
	}
	//============================================= END OF STATEMENT =======================================/
	// Contains("procName", *)
	else if((type == PROCEDURE && arg1 != 0))
	{
		int start, end;
		// restrict the check
		if(arg1 != 0){
			start = pkb->getProcedure(arg1)->getStartProgLine();
			end = pkb->getProcedure(arg1)->getEndProgLine();
		}
		
		// Contain*("procName", s/a/call/w/if)
		if(type2 == STATEMENT || type2 == ASSIGNMENT || type2 == CALL || type2 == WHILE || type2 == IF){
			for(int i = start; i <= end; i++){
				if(type2 == STATEMENT){
					// push all
					resultList.push_back(make_pair(arg1, i));
				}
				else if(Helper::getStatementType(i) == type2){
					resultList.push_back(make_pair(arg1, i));
				}
			}
		}
		else if(type2 == VARIABLE || type2 == CONSTANT || type2 == PLUS || type2 == MINUS || type2 == MULTIPLY){
			// Contain*("procName", v/c),
			DATA_LIST *assignList = pkb->getAllAssigns();
			DATA_LIST::iterator assignListItr = assignList->begin();

			while(assignListItr!=assignList->end()){
				if(*assignListItr >=start && *assignListItr <= end){
					string prefix = pkb->getAssignEntry(*assignListItr).prefixTree;
				
					if(type2 == CONSTANT){
						DATA_LIST allConstants = EvaluateContains::getPrefixTreeConstants(prefix,0);
						DATA_LIST::iterator cItr = allConstants.begin();
						while(cItr != allConstants.end()){
							resultList.push_back(make_pair(arg1, *cItr));
							cItr++;
						}
					}

					else if(type2 == VARIABLE){
						DATA_LIST allVariables = EvaluateContains::getPrefixTreeVarIndexes(prefix,0);
						DATA_LIST::iterator vItr = allVariables.begin();
						while(vItr != allVariables.end()){
							resultList.push_back(make_pair(arg1, *vItr));
							vItr++;
						}
					}
					else{
						DATA_LIST allOperators = EvaluateContains::getPrefixTreeOperators(prefix, type2, 0);
						DATA_LIST::iterator allOperatorsItr = allOperators.begin();
						while(allOperatorsItr != allOperators.end()){
							resultList.push_back(make_pair(arg1, *allOperatorsItr));
							allOperatorsItr++;
						}
					}
				}
				assignListItr++;
			}
		}

		
	}

	// Contain("x", ...)
	else if((type == VARIABLE || type == CONSTANT || type == PLUS || type == MINUS || type == MULTIPLY) && arg1 != 0 && (arg2 == -1 || arg2 == 0)){

		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		
		while(assignListItr!=assignList->end()){
			int operPos = EvaluateContains::findPrefixTreeMatch(type, *assignListItr,arg1,0);

			// found "x" in this statement, i wan all the thing behind him
			if(operPos != -1){
				string prefix = pkb->getAssignEntry(*assignListItr).prefixTree;

				if(type2 == CONSTANT){
					// start findin constants after "x"
					DATA_LIST allConstants = EvaluateContains::getPrefixTreeConstants(prefix, operPos+1);
					DATA_LIST::iterator cItr = allConstants.begin();
					while(cItr != allConstants.end()){
						resultList.push_back(make_pair(arg1, *cItr));
						cItr++;
					}
				}

				else if(type2 == VARIABLE){
					// start findin constants after "x"
					DATA_LIST allVariables = EvaluateContains::getPrefixTreeVarIndexes(prefix, operPos+1);
					DATA_LIST::iterator vItr = allVariables.begin();
					while(vItr != allVariables.end()){
						resultList.push_back(make_pair(arg1, *vItr));
						vItr++;
					}
				}

				else{
					DATA_LIST allOperators = EvaluateContains::getPrefixTreeOperators(prefix, type2,operPos+1);
					if(allOperators.size()!=0){
						resultList.push_back(make_pair(arg1, type2));
					}
				}
			}
			assignListItr++;
		}
	}
	
	// =============================== Contain*(UNKNOWN, KNOWN) =========================//

	// Contain*(s, "v"), Contain*(s, "1"), 
	//Contain*(a, "v"), Contain*(a, "1"), 
	//Contain*(w, "v"), Contain*(w, "1"),
	//Contain*(if, "v"), Contain*(if, "1")
	else if(type!=PROCEDURE && arg1 == 0 && arg2 !=0 && (type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS ||  type2 == MINUS || type2 == MULTIPLY)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			int operPos = EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,arg2,0);
			if(operPos!=-1){
				if(type==STATEMENT || type == ASSIGNMENT){
					resultList.push_back(make_pair(*assignListItr, arg2));
				}
				if(type==WHILE){
					PARENT_LIST parentLst = EvaluateParents::getParentStarResult(WHILE, 0, *assignListItr);
					PARENT_LIST::iterator parentLstitr = parentLst.begin();
					while(parentLstitr!=parentLst.end()){
						resultList.push_back(make_pair(parentLstitr->first, arg2));
						parentLstitr++;
					}
				}
				if(type==IF){
					PARENT_LIST parentLst = EvaluateParents::getParentStarResult(IF, 0, *assignListItr);
					PARENT_LIST::iterator parentLstitr = parentLst.begin();
					while(parentLstitr!=parentLst.end()){
						resultList.push_back(make_pair(parentLstitr->first, arg2));
						parentLstitr++;
					}
				}
			}
			assignListItr++;
		}
	}

	else if(type == PROCEDURE && arg1 == 0 && (arg2 != 0 || arg2==-1)){
		PROC_LIST *procList = pkb->getAllProc();
		PROC_LIST::iterator procListItr = procList->begin();
		
		if(type2 == STATEMENT){
			while(procListItr!=procList->end()){
				int start = procListItr->getStartProgLine();
				int end = procListItr->getEndProgLine();
				if(arg2 >= start && arg2 <= end){
					resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()),arg2));
					break;
				}
				procListItr++;
			}
		}

		// O(n2)
		else if(type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS ||  type2 == MINUS || type2 == MULTIPLY){
			DATA_LIST *assignList = pkb->getAllAssigns();
			DATA_LIST::iterator assignListItr = assignList->begin();

			while(assignListItr!=assignList->end()){
				int operPos = EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,arg2,0);
				if(operPos!=-1){
					// find which procedure it belong to
					while(procListItr!=procList->end()){
						int start = procListItr->getStartProgLine();
						int end = procListItr->getEndProgLine();
						if(*assignListItr >= start && *assignListItr <= end){
							resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()),*assignListItr));
							break;
						}
						procListItr++;
					}
				}
				assignListItr++;
			}
		}
	}
	

	// =============================== Contain*(UNKNOWN, UNKNOWN) =========================//
	else if((type!=PROCEDURE && type2!=PROCEDURE) && arg1 == 0 && arg2 == 0){
		if(type == STMT_LIST && type2 == STMT_LIST){


			PARENT_LIST parentList = EvaluateParents::getParentStarResult(WHILE, WHILE);
			PARENT_LIST::iterator parentListItr = parentList.begin();

			while(parentListItr != parentList.end()){
				resultList.push_back(make_pair(parentListItr->first, parentListItr->second));
				parentListItr++;
			}

			parentList = EvaluateParents::getParentStarResult(WHILE, IF);
			parentListItr = parentList.begin();

			while(parentListItr != parentList.end()){
				resultList.push_back(make_pair(parentListItr->first, parentListItr->second));
				parentListItr++;
			}

			parentList = EvaluateParents::getParentStarResult(IF, IF);
			parentListItr = parentList.begin();

			while(parentListItr != parentList.end()){
				resultList.push_back(make_pair(parentListItr->first, parentListItr->second));
				parentListItr++;
			}

			parentList = EvaluateParents::getParentStarResult(IF, WHILE);
			parentListItr = parentList.begin();

			while(parentListItr != parentList.end()){
				resultList.push_back(make_pair(parentListItr->first, parentListItr->second));
				parentListItr++;
			}
		}
		// Contain*(s1, v)
		// Contain*(s1, c)
		// Contain*(if, v)
		// Contain*(if, c)
		// Contain*(w, v)
		// Contain*(w, c)
		// Contain*(a, v)
		// Contain*(a, c)
		else if(type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS ||  type2 == MINUS || type2 == MULTIPLY){
			DATA_LIST *assignList = pkb->getAllAssigns();
			DATA_LIST::iterator assignListItr = assignList->begin();
			// control variable
			if(type == WHILE && type2 == VARIABLE){
				DATA_LIST *ctrlList = pkb->getAllWhiles();
				DATA_LIST::iterator ctrlItr = ctrlList->begin();
				while(ctrlItr!=ctrlList->end()){
					resultList.push_back(make_pair(*ctrlItr, pkb->getWhileCtrVar(*ctrlItr)));
					ctrlItr++;
				}
			}

			if(type == IF && type2 == VARIABLE){
				DATA_LIST *ctrlList = pkb->getAllIfs();
				DATA_LIST::iterator ctrlItr = ctrlList->begin();
				while(ctrlItr!=ctrlList->end()){
					resultList.push_back(make_pair(*ctrlItr, pkb->getIfCtrVar(*ctrlItr)));
					ctrlItr++;
				}
			}

			// RHS
			while(assignListItr!=assignList->end()){
				string prefix = pkb->getAssignEntry(*assignListItr).prefixTree;
				VAR_INDEX varIndex =  pkb->getAssignEntry(*assignListItr).varIndex;
				
				if(type2 == CONSTANT){
					DATA_LIST allConstant = EvaluateContains::getPrefixTreeConstants(prefix, 0);
					DATA_LIST::iterator allConstantItr = allConstant.begin();
					while(allConstantItr != allConstant.end()){
						resultList.push_back(make_pair(*assignListItr, *allConstantItr));
						allConstantItr++;
					}
				}

				else if(type2 == VARIABLE){
					// control variable
					resultList.push_back(make_pair(*assignListItr, varIndex));
					
					DATA_LIST allVariable = EvaluateContains::getPrefixTreeVarIndexes(prefix, 0);
					DATA_LIST::iterator allVariableItr = allVariable.begin();
					while(allVariableItr != allVariable.end()){
						resultList.push_back(make_pair(*assignListItr, *allVariableItr));
						allVariableItr++;
					}
				}

				else{
					DATA_LIST allOperators = EvaluateContains::getPrefixTreeOperators(prefix, type2, 0);
					DATA_LIST::iterator allOperatorsItr = allOperators.begin();
					while(allOperatorsItr != allOperators.end()){
						resultList.push_back(make_pair(*assignListItr, *allOperatorsItr));
						allOperatorsItr++;
					}
				}
				
			assignListItr++;
			}
		}
		else{
			PARENT_LIST parentList = EvaluateParents::getParentStarResult(type, type2);
			PARENT_LIST::iterator parentListItr = parentList.begin();

			while(parentListItr != parentList.end()){
				resultList.push_back(make_pair(parentListItr->first, parentListItr->second));
				parentListItr++;
			}
		}
	}
	
	else if(type == PROCEDURE && arg1 == 0 && arg2 == 0){
		PROC_LIST *procList = pkb->getAllProc();
		PROC_LIST::iterator procListItr = procList->begin();
		
		if(type2 == ASSIGNMENT || type2 == WHILE || type2 == IF || type2 == CALL){
				while(procListItr!=procList->end()){
					int start = procListItr->getStartProgLine();
					int end = procListItr->getEndProgLine();
					for(int i = start; i <= end; i++){
						if(type2 == STATEMENT){
							// push all
							resultList.push_back(make_pair(arg1, i));
						}
						else if(Helper::getStatementType(i) == type2){
							resultList.push_back(make_pair(arg1, i));
						}
					}
					procListItr++;
				}
		}
	}

	else if((type == PLUS || type == MINUS || type == MULTIPLY) && (type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS || type2 == MINUS || type2 == MULTIPLY)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			// for this to be true, PLUS must appear before variable/constant in prefix
			int operPos;
			operPos = EvaluateContains::findPrefixTreeMatch(type, *assignListItr, arg1, 0);
			string prefix = pkb->getAssignEntry(*assignListItr).prefixTree;
			if(type2 == CONSTANT){
				DATA_LIST allConstant = EvaluateContains::getPrefixTreeConstants(prefix, operPos);
				DATA_LIST::iterator allConstantItr = allConstant.begin();
				while(allConstantItr != allConstant.end()){
					resultList.push_back(make_pair(*assignListItr, *allConstantItr));
					allConstantItr++;
				}
			}

			else if(type2 == VARIABLE){
				DATA_LIST allVariable = EvaluateContains::getPrefixTreeVarIndexes(prefix, operPos);
				DATA_LIST::iterator allVariableItr = allVariable.begin();
				while(allVariableItr != allVariable.end()){
					resultList.push_back(make_pair(*assignListItr, *allVariableItr));
					allVariableItr++;
				}
			}
			else{
 				DATA_LIST allOperators = EvaluateContains::getPrefixTreeOperators(prefix, type2, 0);
				DATA_LIST::iterator allOperatorsItr = allOperators.begin();
				while(allOperatorsItr != allOperators.end()){
					resultList.push_back(make_pair(*assignListItr, *allOperatorsItr));
					allOperatorsItr++;
				}
			}


			assignListItr++;
		}
	}
	resultList.sort();
	resultList.unique();
	return resultList;
}

bool EvaluateContains::getIsContainStarResult(TYPE type, int arg1, TYPE type2, int arg2){
	if(arg1 == 0 || arg2 == 0)
		return false;

	// Contain*(1, 2)
	if(type == STATEMENT && type2 == STATEMENT){
		return EvaluateParents::getIsParentStarResult(arg1, arg2);
	}

	// If arg1 is an assignment, just check prefix tree. 
	// Contain*(1 "x"), Contains*(1, "1"), Contains*(1, PLUS),  Contains*(1, MINUS),  Contains*(1, MULTIPLY)
	if(type == STATEMENT && Helper::getStatementType(arg1) == ASSIGNMENT){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			if(EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,  arg2, 0) == -1){
				return true;
			}
			assignListItr++;
		}
	}

	// If arg1 is WHILE or IF, Parents*(WHILE/IF, s). If s is an assignment, check prefix in assignTable
	if(type == STATEMENT && (Helper::getStatementType(arg1) == WHILE || Helper::getStatementType(arg1) == IF)){
		PARENT_LIST parentList = EvaluateParents::getParentStarResult(WHILE, arg1, 0);
		PARENT_LIST::iterator parentListItr = parentList.begin();
		
		while(parentListItr != parentList.end()){
			if(Helper::getStatementType(parentListItr->second) == ASSIGNMENT){
				if(EvaluateContains::findPrefixTreeMatch(type2, parentListItr->second,  arg2, 0)){
					return true;
				}
			}

			parentListItr++;
		}
	}

	// Containts(STMT_LIST, ...), STMT_LIST = all program lines
	// O(n)
	if(type == STMT_LIST){
		int start = 1;
		int end = pkb->getMaxStatementNum();
		
		if(type2 == STATEMENT){
			if(arg2 >= start && arg2 <= end)
				return true;
		}

		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			if(*assignListItr >=start && *assignListItr <= end && EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,  arg2, 0) != -1){
				return true;
			}
			assignListItr++;
		}
	}


	// Get Start and End line of the procedure, match with assign, check prefix
	// Contain*("procName", "x")
	// Contain*("procName", "1")
	if(type == PROCEDURE){

		int start = pkb->getProcedure(arg1)->getStartProgLine();
		int end = pkb->getProcedure(arg1)->getEndProgLine();
		
		if(type2 == STATEMENT){
			if(arg2 >= start && arg2 <= end)
				return true;
		}

		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			if(*assignListItr >=start && *assignListItr <= end && EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,  arg2, 0) != -1){
				return true;
			}
			assignListItr++;
		}
	}

	// Contain*(PLUS, "apple") 
	// Contain*(MINUS, "1")
	if((type == PLUS || type == MINUS || type == MULTIPLY || type == VARIABLE || type == CONSTANT) && (type2 == PLUS || type2 == MINUS || type2 == MULTIPLY || type2 == VARIABLE || type2 == CONSTANT)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			// for this to be true, PLUS must appear before variable/constant in prefix
			int operPos;
			operPos = EvaluateContains::findPrefixTreeMatch(type, *assignListItr, arg2, 0);
			
			// for Contain*(PLUS, PLUS)
			int pos = EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,  arg2, 0);
			vector<int> operandPos;

			// must chekc precedence here.
			// find all positions behind
			while(pos != -1){
				operandPos.push_back(pos);
				pos = EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,  arg2, pos+1);
			}

			vector<int>::iterator vItr = operandPos.begin();
			while(vItr!=operandPos.end()){
				if(operPos != -1 && operPos < *vItr){
					// check precednece
					if(type2 != MULTIPLY && pkb->getAssignEntry(*assignListItr).prefixTree.substr(*vItr, (*vItr)+1) != "*" ){
						return true;
					}
				}
				vItr++;
			}
			assignListItr++;
		}
	}

	return false;
}

CONTAIN_LIST EvaluateContains::getContainResult(TYPE type, int arg1, TYPE type2, int arg2){	
	// =============================== Contain(KNOWN, UNKNOWN) =========================//
	CONTAIN_LIST resultList;

	// Contain(1, s2), Contain(1, a), Contain(1, w), Contain(1, if), 1 can be stmt# or stmtList
	if(type == STATEMENT && (type2 != VARIABLE || type2 != CONSTANT)&& arg1!=NULL && arg2 == NULL && !Helper::isStatementTypeOf(ASSIGNMENT, arg1)){
		AST_LIST *nodes = pkb->getASTBy(arg1);
		AST *startNode = nodes->front();

		// If 1 is stmtList
		if(startNode->getRootType() == STMT_LIST){
			startNode = startNode->getFirstDescendant();
			cout << startNode->getRootData() << "\n";
			while(startNode!=NULL){
				// If type2 is statement, add everything
				if(type2 == STATEMENT || startNode->getRootType() == type2){
					resultList.push_back(make_pair(arg1, startNode->getRootData()));
				}
				startNode = startNode->getRightSibling();
			}
		}
		// if node is WHILE or IF, we're looking for STMT_LIST
		else if(startNode->getRootType() == WHILE || startNode->getRootType() == IF){
			startNode = startNode->getFirstDescendant()->getRightSibling()->getFirstDescendant();
			if(startNode!=NULL){
				if(type2 == STATEMENT || startNode->getRootType() == type2){
					resultList.push_back(make_pair(arg1, startNode->getRootData()));
				}
				// Special Case because of THEN, ELSE
				if(type2 == IF){
					resultList.push_back(make_pair(arg1, startNode->getRightSibling()->getRootData()));
				}
			}
		}
	}
	// Contain(1, variable), Contain(1, constant) where 1 is assignment
	else if(type == STATEMENT && ( type2 == VARIABLE ||  type2 == CONSTANT) && arg1!=NULL && arg2 == NULL && Helper::isStatementTypeOf(ASSIGNMENT, arg1)){
		AST_LIST *nodes = pkb->getASTBy(arg1);
		AST *startNode = nodes->front()->getFirstDescendant();
		while(startNode != NULL){
			if(startNode->getRootType() == type2){
				// (arg1, varIndex/constantIndex)
				resultList.push_back(make_pair(arg1, startNode->getRootData()));
			}
			startNode = startNode->getRightSibling();
		}
	}

	// Contain("xylo", s)
	// For procedure to contain some unknown type, must be valid type PROCEDURE in StmtListTable
	else if(type == PROCEDURE && type2 == STATEMENT && arg1!=NULL && arg2 == NULL){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			// find the matching ProcIndex
			if(pkb->getStmtEntry(*stmtListItr).type == type && pkb->getStmtEntry(*stmtListItr).ownerNo == arg1)
				resultList.push_back(make_pair(arg1, *stmtListItr));
			stmtListItr++;
		}
	}
	//Contain("xylo", while), Contain("xylo", if), Contain("xylo", call), Contain("xylo", assignment)
	else if(type == PROCEDURE && (type2 == ASSIGNMENT || type2 == WHILE || type2 == IF || type2 == CALL) && arg1!=NULL && arg2 == NULL){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			if(pkb->getStmtEntry(*stmtListItr).type == type && pkb->getStmtEntry(*stmtListItr).ownerNo == arg1 && Helper::isStatementTypeOf(type2, *stmtListItr)){
				resultList.push_back(make_pair(arg1, *stmtListItr));
			}
			stmtListItr++;
		}
	}

	// =============================== Contain(UNKNOWN, KNOWN) =========================//
	// Contain(s, 4), Contain(w, 4), Contain(if, 4), where 4 is stmt_list
	// Contain(proc, 4), where 4 is stmt_list
	else if((type == STATEMENT || type == PROCEDURE || type == IF || type == WHILE) && type2 == STATEMENT && Helper::isStatementTypeOf(STMT_LIST, arg2) && arg2 != NULL && arg1 == NULL){
		AST_LIST *nodeLst = pkb->getASTBy(arg2);
		AST *startNode = nodeLst->front();
		// push everything or specific type, no procedure here
		if((type == STATEMENT || startNode->getAncestor()->getRootType() == type)){
			resultList.push_back(make_pair(startNode->getAncestor()->getRootData(), arg2));
		}
	}
	// Contain(stmtlist, 4), where 4 is assignment, if, while or call
	else if((type == STATEMENT) && type2 == STATEMENT && !Helper::isStatementTypeOf(STMT_LIST, arg2) && arg2!=NULL && arg1 == NULL){
		AST_LIST *nodeLst = pkb->getASTBy(arg2);
		AST *startNode = nodeLst->front();
		// only stmt_list is possible here
		if(startNode->getRootType() == STMT_LIST){
			resultList.push_back(make_pair(startNode->getAncestor()->getRootData(), arg2));
		}
	}

	// Contain(a, "v"/"c")
	// Whether the variable or constant is directly under the assignNode
	// Get all asignments from assignTable, check its descendant for type2 and arg2
	else if(type == ASSIGNMENT && arg1 == NULL && arg2 != NULL&& (type2 == VARIABLE || type2 == CONSTANT)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
				resultList.push_back(make_pair(*assignListItr, arg2));
			}
			assignListItr++;
		}
	}
	
	// Contain(a, PLUS/MINUS/MULTIPLY)
	// Go to assignTable, find whether if any assignNode has PLUS, MINUS, MuLTIPLY
	else if(type == ASSIGNMENT && arg1 == 0 && ( type2 == PLUS ||  type2 == MINUS ||  type2 == MULTIPLY)){
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, -1, *assignListItr)){
				resultList.push_back(make_pair(*assignListItr, type));
			}
			assignListItr++;
		}
	}
	// Contain(w, "v")
	// Get all while stmts that match arg2 from whileTable
	else if(type == WHILE && arg1 == 0 && type2 == VARIABLE && arg2 != NULL){
		// whileTable
		DATA_LIST * whileList = pkb->getWhiles(arg2);
		DATA_LIST::iterator whileListItr = whileList->begin();
		while(whileListItr!=whileList->end()){
			resultList.push_back(make_pair(*whileListItr, arg2));
			whileListItr++;
		}
	}
	// Contain(if, "v")
	// Get all if stmts that match arg2 from ifTable
	else if(type == IF && arg1 == 0 && type2 == VARIABLE && arg2 != NULL){
		DATA_LIST * ifList = pkb->getIfs(arg2);
		DATA_LIST::iterator ifListItr = ifList->begin();
		while(ifListItr!=ifList->end()){
			resultList.push_back(make_pair(*ifListItr, arg2));
			ifListItr++;
		}
	}

	// =============================== Contain(UNKNOWN, UNKNOWN) =========================//
	// Contain(statement, stmt_list)
	// Contain(w, stmt_list), Contain(if, stmt_list)
	else if(((type == STATEMENT || type == WHILE || type == IF) && arg1 == 0 && type2 == STMT_LIST) && arg2 == 0){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			if(pkb->getStmtEntry(*stmtListItr).type == type){
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, *stmtListItr));
			}
			stmtListItr++;
		}
	}
	// Contain(stmt_list, any type)
	else if(type == STMT_LIST && (type2 == STATEMENT || type2 == WHILE || type2 == IF || type2 == CALL || type2 == ASSIGNMENT) && arg1 == 0 && arg2 == 0){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			AST_LIST *astList = pkb->getASTBy(*stmtListItr);
			AST *startNode = astList->front()->getFirstDescendant();

			while(startNode != NULL){
				if((type2 == STATEMENT || startNode->getRootType() == type2)){
					resultList.push_back(make_pair(*stmtListItr, startNode->getRootData()));
				}
				startNode = startNode->getRightSibling();
			}


			stmtListItr++;
		}
	}
	
	// Contain(w, v)
	else if(type == WHILE && type2 == VARIABLE  && arg1 == 0 && arg2 == 0){
		DATA_LIST * whileList = pkb->getAllWhiles();
		DATA_LIST::iterator whileListItr = whileList->begin();

		while(whileListItr != whileList->end()){
			resultList.push_back(make_pair(*whileListItr, pkb->getWhileCtrVar(*whileListItr)));
			whileListItr++;
		}
	}

	// Contain(if, v)
	else if(type == IF && type2 == VARIABLE && arg1 == 0 && arg2 == 0){
		DATA_LIST * ifList = pkb->getAllIfs();
		DATA_LIST::iterator ifListItr =  ifList->begin();

		while(ifListItr != ifList->end()){
			resultList.push_back(make_pair(*ifListItr, pkb->getIfCtrVar(*ifListItr)));
			ifListItr++;
		}
	}


	// Contain(procedure, stmt_list)
	// Find all the first PROCEDURE stmtList in table
	else if(type == PROCEDURE && type2 == STMT_LIST && arg1==0 && arg2==0){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();

		while(stmtListItr!=stmtList->end()){
			if(pkb->getStmtEntry(*stmtListItr).type == type){
				// return (ProcIndex, Stmt#)
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo,*stmtListItr));
			}
			stmtListItr++;
		}
	}

	// Contain(a, v), Contain(a, c) [The only O(n2)]
	else if(type == ASSIGNMENT && (type2 == VARIABLE || type2 == CONSTANT) && arg1 == 0 && arg2 == 0){
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		
		while(assignListItr!=assignList->end()){
			DATA_LIST arg2List = EvaluateContains::getDataListTypeDescendantOf(type, type2, *assignListItr);
			DATA_LIST::iterator arg2ListItr = arg2List.begin();
			
			while(arg2ListItr!=arg2List.end()){
				resultList.push_back(make_pair(*assignListItr,*arg2ListItr));
				arg2ListItr++;
			}

			assignListItr++;
		}
	}
	return resultList;
}

bool EvaluateContains::getIsContainResult(TYPE type, int arg1, TYPE type2, int arg2)
{
	// Contain(1,2)
	if(type == STATEMENT && type2 == STATEMENT && arg1 != 0 && arg2 != 0){
		// Contain(STMT_LIST, whatever's below)
		if(Helper::isStatementTypeOf(STMT_LIST, arg1)){
			AST_LIST *aList = pkb->getASTBy(arg1);
			AST *startNode = aList->front()->getFirstDescendant();

			while(startNode != NULL){
				if(startNode->getRootData() == arg2)
					return true;
				else
					startNode = startNode->getRightSibling();
			}
		}
		// Assignment has no child of type statement
		else if(Helper::isStatementTypeOf(ASSIGNMENT, arg1)){
			return false;
		}
		// WHILE/IF, from StmtListTable, look whether any stmt# belongs to arg1
		else if(Helper::isStatementTypeOf(WHILE, arg1) || Helper::isStatementTypeOf(IF, arg1)){
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

	// Contain(1, "x") -> control variable, where 1 is either WHILE or IF
	if(type == STATEMENT && type2 == VARIABLE && arg1 != NULL && arg2 != NULL
		&& (Helper::isStatementTypeOf(WHILE, arg1) || Helper::isStatementTypeOf(IF, arg1))){

		if(EvaluateContains::isStatementDescendantTypeDataOf(WHILE, type2, arg2, arg1)){
			return true;
		}
		else if(EvaluateContains::isStatementDescendantTypeDataOf(IF, type2, arg2, arg1)){
			return true;
		}
	}

	// for Contain(1, v/c/PLUS/MINUS/MULTIPLY) where 1 is Assignment Statement	
	if(type == STATEMENT && arg1 != NULL && arg2 != NULL &&
		Helper::isStatementTypeOf(ASSIGNMENT, arg1) && (type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS  || type2 == MINUS || type2 == MULTIPLY)){
			if(EvaluateContains::isStatementDescendantTypeDataOf(ASSIGNMENT, type2, arg2, arg1)){
				return true;
			}
	}

	//Contain("extra", 1)
	if(type == PROCEDURE && type2 == STATEMENT && arg1 != NULL && arg2 != NULL){
		// If the stmt_list has the same stmt# as the procedure's first stmt#
		if(Helper::isStatementTypeOf(STMT_LIST, arg2) && pkb->getProcedure(arg1)->getStartProgLine() == arg2){
			return true;
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
			//cout << pkb->getAssignEntry(*assignListItr).prefixTree << "\n";
			
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
		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
			
		// search if RHS and LHS has type2 and data
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName){
				// check descendant
				AST * descendant = (*itr)->getFirstDescendant();
				
				if(pkb->getType(descendant) == typeName2 && pkb->getData(descendant) == data){
					return true;
				}
				if(pkb->getType(descendant->getRightSibling()) == typeName2 && pkb->getData(descendant->getRightSibling()) == data){
					return true;
				}
				
			}
		}
	}
	
	return false;
}

 int EvaluateContains::findPrefixTreeMatch(TYPE type2, STATEMENT_NUM stmt, int data, int offset){
	 if(type2 == VARIABLE && pkb->getAssignEntry(stmt).prefixTree.find(pkb->getVarName(data),offset) != pkb->getAssignEntry(stmt).prefixTree.npos){
		return pkb->getAssignEntry(stmt).prefixTree.find(pkb->getVarName(data),offset);
	}
	 
	if(type2 == CONSTANT && pkb->getAssignEntry(stmt).prefixTree.find(static_cast<ostringstream*>( &(ostringstream() << pkb->getConstantValue(data)) )->str(),offset) != pkb->getAssignEntry(stmt).prefixTree.npos){
		return pkb->getAssignEntry(stmt).prefixTree.find(static_cast<ostringstream*>( &(ostringstream() << pkb->getConstantValue(data)) )->str(),offset);
	}

	if(type2 == PLUS && pkb->getAssignEntry(stmt).prefixTree.find("+",offset) != pkb->getAssignEntry(stmt).prefixTree.npos){
		return pkb->getAssignEntry(stmt).prefixTree.find("+",offset);
	}

	if(type2 == MINUS && pkb->getAssignEntry(stmt).prefixTree.find("-",offset)!= pkb->getAssignEntry(stmt).prefixTree.npos){
		return pkb->getAssignEntry(stmt).prefixTree.find("-",offset);
	}

	if(type2 == MULTIPLY && pkb->getAssignEntry(stmt).prefixTree.find("*",offset) != pkb->getAssignEntry(stmt).prefixTree.npos){
		return pkb->getAssignEntry(stmt).prefixTree.find("*",offset);
	}

	return -1;
}

DATA_LIST	EvaluateContains::getPrefixTreeVarIndexes(string prefix, int offset){
	DATA_LIST resultList;
	
	string token,text(prefix.substr(offset, prefix.length()));
    istringstream iss(text);
	while ( getline(iss, token, ' ') )
	{
		if(pkb->getVarIndex(token) != 0){
			resultList.push_back(pkb->getVarIndex(token));
		}
	}

	return resultList;
}

DATA_LIST	EvaluateContains::getPrefixTreeConstants(string prefix, int offset){
	DATA_LIST resultList;
	

	string token, text(prefix.substr(offset, prefix.length()));
	istringstream iss(text);
	while ( getline(iss, token, ' ') )
	{
		if((atoi(token.c_str()) != 0 || token == "0") && pkb->getConstantIndex(atoi(token.c_str())) != -1){
			resultList.push_back(pkb->getConstantIndex(atoi(token.c_str())));
		}
	}
	return resultList;
}

DATA_LIST	EvaluateContains::getPrefixTreeOperators(string prefix, TYPE type, int offset){
	DATA_LIST resultList;
	
	string token, text(prefix.substr(offset, prefix.length()));
	string matchType;
    istringstream iss(text);

	if(type == PLUS)
		matchType = "+";
	if(type == MINUS)
		matchType = "-";
	if(type == MULTIPLY)
		matchType = "*";


	while ( getline(iss, token, ' ') )
	{
		if(token == matchType){
			resultList.push_back(type);
		}
	}

	return resultList;
}
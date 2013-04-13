#include "EvaluateContains.h"
/* INDEXES

Contains*(KNOWN, UNKNOWN)
0001: Contains*(STMT#, STMT_LIST)
0002: Contains*(STMT#, STMT#/WHILE/IF/ASSIGNMENT/CALL)
0003: Contains*(STMT#, VARIABLE/CONSTANT) where STMT# is ASSIGNMENT
0004: Contains*(STMT#, VARIABLE/CONSTANT) where STMT# is IF or WHILE
0005: Contains*(PROCEDURE, STMT_LIST)
0006: Contains*(PROCEDURE, STMT#/WHILE/IF/ASSIGNMENT/CALL)
0007: Contains*(PROCEDURE, VARIABLE/CONSTANT)
0008: Contains*(VARIABLE/CONSTANT/PLUS/MINUS/MULTIPLY, VARIABLE/CONSTANT)
0023: Contains*(PLUS/MINUS/MULTIPLY, CONSTANT/VARIABLE/PLUS/MINUS/MULTIPLY)

	
Contains*(UNKNOWN, KNOWN)
0009: Contains*(STMT#/WHILE/IF/ASSIGNMENT/CALL, STMT#)
0010: Contains*(STMT_LIST, STMT#/STMT_LIST)
0011: Contains*(STMT#/CALL/IF/ASSIGNMENT/WHILE, CONSTANT/VARIABLE/PLUS/MINUS/MULTIPLY/MINUS)
0012: Contains*(PROCEDURE, STMT#)		
0013: Contains*(PROCEDURE, CONSTANT/VARIABLE/PLUS/MINUS/MULTIPLY)

Contains*(UNKNOWN, UNKNOWN)
0014: Contain*(STMT_LIST, Stmt#/WHILE/IF/CALL/ASSIGNMENT)
0015: Contain*(STMT_LIST, STMT_LIST)
0016: Contains*(STMT_LIST, VARIABLE/CONSTANT)
0017: Contains*(STMT/ASSIGNMENT/WHILE/IF, CONSTANT/VARIABLE)
0018: Contains*(STMT#, STMT#)
0019: Contains*(PROCEDURE, STATEMENT)
0020: Contains*(PROCEDURE, VARIABLE)
0021: Contains*(PROCEDURE, CONSTANT)
0022: Contains*(PROCEDURE, ASSIGNMENT/WHILE/IF/CALL)

Contains*(KNOWN, KNOWN)
0023: Contain*(STMT#, STMT#)
0024: Contains*(STMT#, VARIABLE/CONSTANT/PLUS/MINUS/MULITPLY) where STMT# is ASSIGNMENT
0025: Contains*(STMT#, VARIABLE/CONSTANT/PLUS/MINUS/MULITPLY) where STMT# is WHILE/IF
0026: Contains*(STMT#/WHILE/IF, STMT_LIST)
0027: Contains*(STMT_LIST, STMT#)
0028: Contains*(STMT_LIST, CONSTANT/VARIABLE)
0029: Contains*(PROCEDURE, STATEMENT)
0030: Contains*(PROCEDURE, PLUS/MULTIPLY/MINUS/VARIABLE/CONSTANT)
0031: Contains*(PLUS/MINUS/MULTIPLY/VARIABLE/CONSTANT, PLUS/MINUS/CONSTANT/VARIABLE/MULITPLY)

Contains(KNOWN, UNKNOWN)
0032: Contains(STMT#, STMT_LIST) where STMT# must be IF or WHILE
0033: Contains(STMT#, VARIABLE/CONSTANT)
0034: Contains(PROCEDURE, STMT_LIST)
0035: Contains(PLUS/MINUS/MULTIPLY, VARIABLE/CONSTANT)

Contains(UNKNOWN, KNOWN)
0036: Contains(STMT#/PROCEDURE/IF/WHILE, STMT#)
[remove] 0037: Contains(STMT#/PROCEDURE/IF/WHILE, STMT_LIST)
0038: Contains(STMT_LIST, STMT#/ASSIGNMENT/WHILE/IF/CALL)
0039: Contains(STMT#, VARIABLE)
0040: Contains(STMT#, CONSTANT/PLUS/MINUS/MULTIPLY)
0041: Contains(ASSIGNMENT, VARIABLE/CONSTANT/PLUS/MINUS/MULTIPLY)
0042: Contains(WHILE, VARIABLE)
0043: Contains(IF, VARIABLE)

Contains(UNKNOWN, UNKNOWN)
0044: Contains(STMT#/WHILE/IF, STMT_LIST)
0045: Contains(STMT_LIST, ASSIGNMENT/WHILE/IF/CALL/ASSIGNMENT)
0046: Contains(IF, VARIABLE)
0047: Contains(WHILE, VARIABLE)
0048: Contains(STMT#, VARIABLE)
0049: Contains(STMT#, CONSTANT)
0050: Contains(ASSIGNMENT, VARIABLE/CONSTANT)
0051: Contains(PROCEDURE, STMT_LIST)

Contains(KNOWN, KNOWN)
0052: Contains(STMT#, STMT#)
[removed] 0053: Contains(STMT_LIST, STMT#/ASSIGNMENT/WHILE/IF/CALL)
[removed] 0054: Contains(STMT#/ASSIGNMENT/WHILE/IF/CALL, STMT_LIST)
0055: Contains(STMT#, VARIABLE) where STATEMENT is WHILE or IF
0056: Contains(STMT#, CONSTANT/VARIABLE/PLUS/MINUS/MULTIPLY), where STATEMENT is ASSIGNMENT
0057: Contains(PROCEDURE, STMT#)
0058: Contains(PLUS/MINUS/MULTIPLY, VARIABLE/CONSTANT/PLUS/MINUS/MULTIPLY)

*/

PKB *EvaluateContains::pkb = NULL;

EvaluateContains::EvaluateContains(void)
{
}

EvaluateContains::~EvaluateContains(void)
{
}

CONTAIN_LIST EvaluateContains::getContainStarResult(TYPE type, int arg1, TYPE type2, int arg2){
	
	CONTAIN_LIST resultList;
	// 0001: Contains*(STMT#, STMT_LIST)
	if((type == STATEMENT) && type2 == STMT_LIST && arg1 != NULL && arg2 == NULL){
		PARENT_LIST pList = EvaluateParents::getParentStarResult(STATEMENT, arg1, 0);
		PARENT_LIST::iterator pListItr = pList.begin();

		while(pListItr!=pList.end()){
			if(type == STATEMENT && Helper::isStatementTypeOf(STMT_LIST, pListItr->second)){
				resultList.push_back(*pListItr);
			}
			else if(type == STMT_LIST && Helper::isStatementTypeOf(STMT_LIST, pListItr->first) && Helper::isStatementTypeOf(STMT_LIST, pListItr->second)){
				resultList.push_back(*pListItr);
			}
			pListItr++;
		}
	}

	// 0002: Contains*(STMT#, STMT#/WHILE/IF/ASSIGNMENT/CALL)
	// 0009: Contains*(STMT#/WHILE/IF/ASSIGNMENT/CALL, STMT#)
	else if((type == STATEMENT && arg1 != 0 && (type2 == STATEMENT || type2 == WHILE || type2 == IF || type2 == ASSIGNMENT || type2 == CALL)) || (type2 == STATEMENT && arg2 != 0 && (type == STATEMENT || type == WHILE || type == IF || type == ASSIGNMENT))){
		PARENT_LIST parentList;
		parentList = EvaluateParents::getParentStarResult(type, type2);
		
		PARENT_LIST::iterator parentListItr = parentList.begin();
		while(parentListItr != parentList.end()){
			if(arg1!= 0 && parentListItr->first == arg1)
				resultList.push_back(make_pair(arg1, parentListItr->second));
			else if(arg2!= 0 && parentListItr->second == arg2)
				resultList.push_back(make_pair(parentListItr->first, arg2));
			parentListItr++;
		}
	}

	// 0003: Contains*(STMT#, VARIABLE/CONSTANT) where STMT# is ASSIGNMENT
	else if((type == STATEMENT && Helper::isStatementTypeOf(ASSIGNMENT, arg1) && arg1 != 0 && arg2 == 0 && (type2 == VARIABLE || type2 == CONSTANT))){
		
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
			// get the LHS
			resultList.push_back(make_pair(arg1, pkb->getAssignEntry(arg1).varIndex));

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

	// 0004: Contains*(STMT#, VARIABLE/CONSTANT) where STMT# is IF or WHILE
	else if((type == STATEMENT) && (Helper::isStatementTypeOf(WHILE, arg1) || Helper::isStatementTypeOf(IF, arg1) )&& arg1 != 0 && arg2 == 0  && (type2 == VARIABLE || type2 == CONSTANT || type2 == PLUS || type2 == MINUS || type2 == MULTIPLY)){
		PARENT_LIST parentList;
	
		// Get Parent* (1, x)
		parentList = EvaluateParents::getParentStarResult(STATEMENT, arg1, 0);
		
		PARENT_LIST::iterator parentListItr = parentList.begin();

		while(parentListItr != parentList.end()){
			if(parentListItr->first = arg1){
				// check and add contorl variable
				if(Helper::isStatementTypeOf(WHILE, arg1) &&type2 == VARIABLE){
					resultList.push_back(make_pair(arg1, pkb->getWhileCtrVar(arg1)));
				}
				
				else if(Helper::isStatementTypeOf(IF, arg1) &&type2 == VARIABLE){
					resultList.push_back(make_pair(arg1, pkb->getIfCtrVar(arg1)));
				}
				
				// check child
				if(Helper::isStatementTypeOf(WHILE, parentListItr->second)){
					if(type2 == VARIABLE){
						resultList.push_back(make_pair(arg1, pkb->getWhileCtrVar(parentListItr->second)));
					}
				}
				else if(Helper::isStatementTypeOf(IF, parentListItr->second)){
					
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
						resultList.push_back(make_pair(arg1,pkb->getAssignEntry(parentListItr->second).varIndex));
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

	// Contains*(PROCEDURE, STMT/CALL/IF/ASSIGNMENT/WHILE/IF)
	else if((type == PROCEDURE && arg1 != 0))
	{
		int start, end;
		if(arg1 != 0){
			start = pkb->getProcedure(arg1)->getStartProgLine();
			end = pkb->getProcedure(arg1)->getEndProgLine();
		}
		
		// 0005: Contain*(PROCEDURE, STMT_LIST)
		if(type2 == STMT_LIST){
			DATA_LIST *stmtList = pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();

			while(stmtListItr!=stmtList->end()){
				if(*stmtListItr >=start && *stmtListItr <= end){
					resultList.push_back(make_pair(arg1, *stmtListItr));
				}
				stmtListItr++;
			}
		}

		// 0006: Contains*(PROCEDURE, STMT#/WHILE/IF/ASSIGNMENT/CALL)
		else if(type2 == STATEMENT || type2 == ASSIGNMENT || type2 == CALL || type2 == WHILE || type2 == IF){
			for(int i = start; i <= end; i++){
				if(type2 == STATEMENT){
					// push all
					resultList.push_back(make_pair(arg1, i));
				}
				else if(Helper::isStatementTypeOf(type2, i)){
					resultList.push_back(make_pair(arg1, i));
				}
			}
		}

		// 0007: Contains*(PROCEDURE, VARIABLE/CONSTANT/PLUS/MINUS/MULTIPLY)
		else if(type2 == VARIABLE || type2 == CONSTANT || type2 == PLUS || type2 == MINUS || type2 == MULTIPLY){
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
						resultList.push_back(make_pair(arg1, pkb->getAssignEntry(*assignListItr).varIndex));

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

	// 0008: Contains*(VARIABLE/CONSTANT/PLUS/MINUS/MULTIPLY, VARIABLE/CONSTANT)
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
	
	// 0010: Contains*(STMT_LIST, STMT#/STMT_LIST)
	else if(type == STMT_LIST && ( type2 == STATEMENT || type2 == STMT_LIST) && arg1 == NULL && arg2 != NULL){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			//add for procedure
			if(pkb->getStmtEntry(*stmtListItr).type == PROCEDURE){
							
				int start = pkb->getProcedure(pkb->getStmtEntry(*stmtListItr).ownerNo)->getStartProgLine();
				int end = pkb->getProcedure(pkb->getStmtEntry(*stmtListItr).ownerNo)->getEndProgLine();

				if(arg2 >= start && arg2 <= end){
					resultList.push_back(make_pair(*stmtListItr, arg2));
				}
			}
			if(*stmtListItr <= arg2 && pkb->getStmtEntry(*stmtListItr).type == THEN &&  EvaluateParents::getIsParentStarResult( pkb->getStmtEntry(*stmtListItr).ownerNo, arg2))
			{
				AST *tmp = pkb->getASTBy(*stmtListItr)->front()->getRightSibling(); // else node
				if(arg2 < pkb->getStatementNum(tmp)){
					resultList.push_back(make_pair(*stmtListItr, arg2));
				}
			}
			else if(*stmtListItr <= arg2 && pkb->getStmtEntry(*stmtListItr).type == ELSE &&  EvaluateParents::getIsParentStarResult( pkb->getStmtEntry(*stmtListItr).ownerNo, arg2))
			{
				AST *tmp = pkb->getASTBy(*stmtListItr)->front()->getLeftSibling(); // else node
				if(arg2 >= pkb->getStatementNum(tmp)){
					resultList.push_back(make_pair(*stmtListItr, arg2));
				}
			}
			else if(*stmtListItr <= arg2 && (pkb->getStmtEntry(*stmtListItr).type != ELSE || pkb->getStmtEntry(*stmtListItr).type != THEN) && EvaluateParents::getIsParentStarResult( pkb->getStmtEntry(*stmtListItr).ownerNo, arg2)){
				resultList.push_back(make_pair(*stmtListItr, arg2));
			}

			stmtListItr++;
		}
	}

	// 0011: Contains*(STMT#/CALL/IF/ASSIGNMENT/WHILE, CONSTANT/VARIABLE/PLUS/MINUS/MULTIPLY/MINUS)
	else if(type!=PROCEDURE && arg1 == 0 && arg2 !=0 && (type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS ||  type2 == MINUS || type2 == MULTIPLY)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		// find assignment statement
		while(assignListItr!=assignList->end()){
			int operPos = EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,arg2,0);
			if((type == STATEMENT || type == ASSIGNMENT ) && type2== VARIABLE && pkb->getAssignEntry(*assignListItr).varIndex == arg2){
				resultList.push_back(make_pair(*assignListItr, arg2));
			}

			if(operPos!=-1){
				if(type==STATEMENT || type == ASSIGNMENT){
					resultList.push_back(make_pair(*assignListItr, arg2));

				}
				if(type == WHILE || type == IF || type == STATEMENT){

					PARENT_LIST parentLst = EvaluateParents::getParentStarResult(type, 0, *assignListItr);
					PARENT_LIST::iterator parentLstitr = parentLst.begin();
					while(parentLstitr!=parentLst.end()){
						resultList.push_back(make_pair(parentLstitr->first, arg2));
						parentLstitr++;
					}
				
				}

				if(type == STMT_LIST){
					DATA_LIST * stmtList =pkb->getAllStmtLst();
					DATA_LIST::iterator stmtListItr = stmtList->begin();
					while(stmtListItr!=stmtList->end()){
						//add for procedure
						if(pkb->getStmtEntry(*stmtListItr).type == PROCEDURE){
							
							int start = pkb->getProcedure(pkb->getStmtEntry(*stmtListItr).ownerNo)->getStartProgLine();
							int end = pkb->getProcedure(pkb->getStmtEntry(*stmtListItr).ownerNo)->getEndProgLine();

							if( *assignListItr >= start &&  *assignListItr <= end){
								resultList.push_back(make_pair(*stmtListItr, arg2));
							}
						}
						if(EvaluateParents::getIsParentStarResult( pkb->getStmtEntry(*stmtListItr).ownerNo, *assignListItr)){
							if( pkb->getStmtEntry(*stmtListItr).type == THEN){
								// is assignListItr within this?
								AST *elseNode = pkb->getASTBy(*stmtListItr)->front()->getRightSibling();
								if(*assignListItr < pkb->getStatementNum(elseNode)){
									resultList.push_back(make_pair(*stmtListItr, arg2));
								}
							}
							else if( pkb->getStmtEntry(*stmtListItr).type == ELSE){
								resultList.push_back(make_pair(*stmtListItr, arg2));
							}
							// WHILE
							else{
								resultList.push_back(make_pair(*stmtListItr, arg2));
							}
						}
						else if(pkb->getStmtEntry(*stmtListItr).type == ELSE){
							
						}

						stmtListItr++;
					}
				}
			}
			assignListItr++;
		}

		// find all control variables
		if(type2 == VARIABLE){
			DATA_LIST *ctrlList = pkb->getAllWhiles();
			DATA_LIST::iterator ctrlItr = ctrlList->begin();
			while(ctrlItr!=ctrlList->end()){
				if(pkb->getWhileCtrVar(*ctrlItr) == arg2){
					if(type == STATEMENT)
						resultList.push_back(make_pair(*ctrlItr, arg2));

					PARENT_LIST parentLst = EvaluateParents::getParentStarResult(type, 0, *ctrlItr);
					PARENT_LIST::iterator parentLstitr = parentLst.begin();
					while(parentLstitr!=parentLst.end()){
						resultList.push_back(make_pair(parentLstitr->first, arg2));
						parentLstitr++;
					}
				}
				ctrlItr++;
			}

			// find all if variables
			ctrlList = pkb->getAllIfs();
			ctrlItr = ctrlList->begin();
			while(ctrlItr!=ctrlList->end()){
				if(pkb->getIfCtrVar(*ctrlItr) == arg2){
					if(type == STATEMENT)
						resultList.push_back(make_pair(*ctrlItr, arg2));

					PARENT_LIST parentLst = EvaluateParents::getParentStarResult(type, 0, *ctrlItr);
					PARENT_LIST::iterator parentLstitr = parentLst.begin();
					while(parentLstitr!=parentLst.end()){
						resultList.push_back(make_pair(parentLstitr->first, arg2));
						parentLstitr++;
					}
				}
				ctrlItr++;
			}
		}
	}

	else if(type == PROCEDURE && arg1 == 0 && (arg2 != 0 || arg2==-1)){
		PROC_LIST *procList = pkb->getAllProc();
		PROC_LIST::iterator procListItr = procList->begin();
	
		// 0012: Contains*(PROCEDURE, STMT#)		
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

		// 0013: Contains*(PROCEDURE, CONSTANT/VARIABLE/PLUS/MINUS/MULTIPLY)
		else if(type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS ||  type2 == MINUS || type2 == MULTIPLY){
			DATA_LIST *assignList = pkb->getAllAssigns();
			DATA_LIST::iterator assignListItr = assignList->begin();

			while(assignListItr!=assignList->end()){
				int operPos = EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,arg2,0);
				if(operPos!=-1){
					// find which procedure it belong to
					procListItr = procList->begin();
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
			
		resultList.sort();
		resultList.unique();
		}
	}
	
	// Contains*(STMT#/CALL/IF/ASSIGNMENT/WHILE, STMT#/CALL/IF/ASSIGNMENT/WHILE)
	else if((type!=PROCEDURE && type2!=PROCEDURE) && arg1 == 0 && arg2 == 0){
		
		// 0014: Contains*(STMT_LIST, Stmt#/WHILE/IF/CALL/ASSIGNMENT)
		if(type == STMT_LIST && (type2 == STATEMENT || type2 == WHILE || type2 == IF || type2 == CALL || type2 == ASSIGNMENT)){
			DATA_LIST *stmtList = pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();

			while(stmtListItr != stmtList->end()){
				if(pkb->getStmtEntry(*stmtListItr).type == PROCEDURE){
					Procedure *p = pkb->getProcedure(pkb->getStmtEntry(*stmtListItr).ownerNo);
					int start = p->getStartProgLine();
					int end = p->getEndProgLine();

					for(int i = start; i<=end; i++){
						if(type2 == STATEMENT || Helper::isStatementTypeOf(type2, i)){
							resultList.push_back(make_pair(*stmtListItr, i));
						}
					}
				}
				else{
					PARENT_LIST pList = EvaluateParents::getParentStarResult(type2, pkb->getStmtEntry( *stmtListItr).ownerNo, 0);
					PARENT_LIST::iterator pListItr = pList.begin();
					
					while(pListItr!=pList.end()){
						if(pkb->getStmtEntry(*stmtListItr).type == THEN)
						{
							AST *tmp = pkb->getASTBy(*stmtListItr)->front()->getRightSibling(); // else node
							if(pListItr->second < pkb->getStatementNum(tmp) ){
								resultList.push_back(make_pair(*stmtListItr, pListItr->second));
							}
						}
						else if(pkb->getStmtEntry(*stmtListItr).type == ELSE)
						{
							AST *tmp = pkb->getASTBy(*stmtListItr)->front()->getLeftSibling(); // else node
							if(pListItr->second >= pkb->getStatementNum(tmp)){
								resultList.push_back(make_pair(*stmtListItr, pListItr->second));
							}
						}
						else if((pkb->getStmtEntry(*stmtListItr).type != ELSE || pkb->getStmtEntry(*stmtListItr).type != THEN)){
							resultList.push_back(make_pair(*stmtListItr, pListItr->second));
						}
						pListItr++;
					}
				}
				// others
				stmtListItr++;
			}
		}
		
		// 0015: Contains*(STMT_LIST, STMT_LIST)
		else if(type == STMT_LIST && type2 == STMT_LIST && arg1 == NULL && arg2 == NULL){
			// all stmtlists are stmtlist of procedure's
			PROC_LIST * procList = pkb->getAllProc();
			PROC_LIST::iterator procListItr = procList->begin();

			while(procListItr!=procList->end()){
				int start = procListItr->getStartProgLine();
				int end = procListItr->getEndProgLine();
				DATA_LIST * stmtList = pkb->getAllStmtLst();
				DATA_LIST::iterator stmtListItr = stmtList->begin();
				while(stmtListItr!=stmtList->end()){
					if(*stmtListItr>= start && *stmtListItr <= end){
						resultList.push_back(make_pair(start, *stmtListItr));
					}
					stmtListItr++;
				}
				procListItr++;
			}

			// check all other stmtlists by using parent
			PARENT_LIST pList = EvaluateParents::getParentStarResult(STATEMENT, STATEMENT);
			PARENT_LIST::iterator pListItr = pList.begin();

			while(pListItr!=pList.end()){
				if(Helper::isStatementTypeOf(STMT_LIST, pListItr->first) && Helper::isStatementTypeOf(STMT_LIST, pListItr->second)){
					resultList.push_back(*pListItr);
				}
				pListItr++;
			}

		}

		// 0016: Contains*(STMT_LIST, VARIABLE/CONSTANT)
		else if(type == STMT_LIST &&( type2 == VARIABLE || type2 == CONSTANT)){
			DATA_LIST * stmtList =pkb->getAllStmtLst();
			DATA_LIST::iterator stmtListItr = stmtList->begin();
			while(stmtListItr!=stmtList->end()){

				// if this is PROCEDURE, add all variables and constant
				if(pkb->getStmtEntry(*stmtListItr).type == PROCEDURE){
					Procedure *p = pkb->getProcedure(pkb->getStmtEntry(*stmtListItr).ownerNo);
					int start = p->getStartProgLine();
					int end = p->getEndProgLine();

					for(int i = start; i <=end; i++){
						EvaluateContains::getTypeInsertAll(&resultList, type, type2, i, *stmtListItr);
					}
				}
				else if(pkb->getStmtEntry(*stmtListItr).type == THEN){
						int start = *stmtListItr;
						int end = pkb->getStatementNum(pkb->getASTBy(*stmtListItr)->front()->getRightSibling())-1;
						for(int i = start; i <=end; i++){
							EvaluateContains::getTypeInsertAll(&resultList,type, type2, i, *stmtListItr);
						}
				}
				else if(pkb->getStmtEntry(*stmtListItr).type == ELSE){
					PARENT_LIST parentList = EvaluateParents::getParentStarResult(STATEMENT, pkb->getStmtEntry(*stmtListItr).ownerNo, 0);
					PARENT_LIST::iterator parentListItr = parentList.begin();

					while(parentListItr != parentList.end()){
						if(parentListItr->second >= *stmtListItr){
							EvaluateContains::getTypeInsertAll(&resultList,type, type2, parentListItr->second, *stmtListItr);
						}
						parentListItr++;
					}
				}
				// WHILE's STMT_LIST
				else{
					PARENT_LIST parentList = EvaluateParents::getParentStarResult(STATEMENT, pkb->getStmtEntry(*stmtListItr).ownerNo, 0);
					PARENT_LIST::iterator parentListItr = parentList.begin();

					while(parentListItr != parentList.end()){
						EvaluateContains::getTypeInsertAll(&resultList, type, type2, parentListItr->second, *stmtListItr);
						parentListItr++;
					}
				}
					
					
				stmtListItr++;
			}
				
		}
		
		// 0017: Contains*(STMT/ASSIGNMENT/WHILE/IF, CONSTANT/VARIABLE)
		else if((type == STATEMENT || type == ASSIGNMENT || type == WHILE || type == IF) && type2 == CONSTANT || type2 == VARIABLE){
			int start = 1;
			int end = pkb->getMaxStatementNum();
			
			for(int i = start; i <= end; i++){
				EvaluateContains::getTypeInsertAll(&resultList, type, type2, i, i);
			}
		}

		// 0018: Contains*(STMT#, STMT#)
		else{
			PARENT_LIST parentList = EvaluateParents::getParentStarResult(type, type2);
			PARENT_LIST::iterator parentListItr = parentList.begin();

			while(parentListItr != parentList.end()){
				resultList.push_back(make_pair(parentListItr->first, parentListItr->second));
				parentListItr++;
			}
		}
	}
	
	// Contains*(PROCEDURE, STMT#/CALL/IF/ASSIGNMENT/WHILE/VARIABLE/CONSTANT)
	else if(type == PROCEDURE && arg1 == 0 && arg2 == 0){
		PROC_LIST *procList = pkb->getAllProc();
		PROC_LIST::iterator procListItr = procList->begin();
		
		while(procListItr!=procList->end()){
			int start = procListItr->getStartProgLine();
			int end = procListItr->getEndProgLine();
			for(int i = start; i <= end; i++){

				// 0019: Contains*(PROCEDURE, STATEMENT)
				if(type2 == STATEMENT){
					// push all
					resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()), i));
				}

				// 0020: Contains*(PROCEDURE, VARIABLE)
				else if(type2 == VARIABLE){
						if(Helper::isStatementTypeOf(WHILE, i))
							resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()), pkb->getWhileCtrVar(i)));
						
						if(Helper::isStatementTypeOf(IF, i))
							resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()), pkb->getIfCtrVar(i)));
							
						if(Helper::isStatementTypeOf(ASSIGNMENT, i)){
							string prefix = pkb->getAssignEntry(i).prefixTree;
							DATA_LIST allVariable = EvaluateContains::getPrefixTreeVarIndexes(prefix, 0);
							DATA_LIST::iterator allVariableItr = allVariable.begin();
							while(allVariableItr != allVariable.end()){
								resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()), *allVariableItr));
								allVariableItr++;
							}
						}
				}

				// 0021: Contains*(PROCEDURE, CONSTANT)
				else if(type2 == CONSTANT){
					if(Helper::isStatementTypeOf(ASSIGNMENT, i)){
						string prefix = pkb->getAssignEntry(i).prefixTree;
						DATA_LIST allConstant = EvaluateContains::getPrefixTreeConstants(prefix, 0);
						DATA_LIST::iterator allConstantItr = allConstant.begin();
						while(allConstantItr != allConstant.end()){
							resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()), *allConstantItr));
							allConstantItr++;
						}
					}
				}

				// 0022: Contains*(PROCEDURE, ASSIGNMENT/WHILE/IF/CALL)
				else if(Helper::isStatementTypeOf(type2, i)){
					resultList.push_back(make_pair(pkb->getProcIndex(procListItr->getProcName()), i));
				}
				
			}
			procListItr++;
		}
	}

	// 0023: Contains*(PLUS/MINUS/MULTIPLY, CONSTANT/VARIABLE)
	else if((type == PLUS || type == MINUS || type == MULTIPLY) && (type2 == CONSTANT || type2 == VARIABLE)){
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
	
	// 0023: Contain*(STMT#, STMT#)
	if(type == STATEMENT && type2 == STATEMENT){
		return EvaluateParents::getIsParentStarResult(arg1, arg2);
	}
	
	// 0024: Contains*(STMT#, VARIABLE/CONSTANT/PLUS/MINUS/MULITPLY)
	else if(type == STATEMENT && Helper::isStatementTypeOf(ASSIGNMENT, arg1) && (type2 == VARIABLE || type2 == PLUS || type2 == MINUS ||type2 == MULTIPLY||type2 == CONSTANT)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			if(EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,  arg2, 0) == -1){
				return true;
			}
			assignListItr++;
		}
	}

	// 0025: Contains*(STMT#, VARIABLE/CONSTANT/PLUS/MINUS/MULITPLY) where STMT# is WHILE/IF
	else if((type == STATEMENT) && (Helper::isStatementTypeOf(WHILE, arg1) || Helper::isStatementTypeOf(IF, arg1))){
		PARENT_LIST parentList;
		TYPE iType;
		if(Helper::isStatementTypeOf(WHILE, arg1)){
			iType = WHILE;
		}
		else{
			iType = IF;
		}
		
		// only for direct control variable
		if(type2 == VARIABLE && EvaluateContains::getIsContainResult(iType, arg1, VARIABLE, arg2)){
			return true;
		}

		// for all the rest
		parentList = EvaluateParents::getParentStarResult(STATEMENT, arg1, 0);
		PARENT_LIST::iterator parentListItr = parentList.begin();
		while(parentListItr != parentList.end()){
			// if 2nd argument is assignment
			if(Helper::isStatementTypeOf(ASSIGNMENT, parentListItr->second) && EvaluateContains::findPrefixTreeMatch(type2, parentListItr->second, arg2, 0)){
				return true;
			}
			// if 2nd argument is while or if
			// check control variable
			else if(type2 == VARIABLE && (Helper::isStatementTypeOf(WHILE, parentListItr->second) || Helper::isStatementTypeOf(IF, parentListItr->second)) && EvaluateContains::getIsContainResult(STATEMENT, parentListItr->second, VARIABLE, arg2)){
				return true;
			}
			parentListItr++;
		}
		
	
	}
	// 0026: Contains*(STMT#/WHILE/IF, STMT_LIST)
	else if((type == STATEMENT || type == WHILE || type == IF) && type2 == STMT_LIST && arg1 != 0 && arg2 != 0){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			// find the matching ProcIndex
			//if(pkb->getStmtEntry(*stmtListItr).type == type && pkb->getStmtEntry(*stmtListItr).ownerNo == arg1)
			if((type == STATEMENT || pkb->getStmtEntry(*stmtListItr).type == type) && *stmtListItr == arg2 && EvaluateParents::getIsParentStarResult(arg1, arg2))
				return true;
			
			stmtListItr++;
		}
	}
	// 0027: Contains*(STMT_LIST, STMT#)
	else if(type == STMT_LIST && (pkb->getStmtEntry(arg1).type != PROCEDURE) && type2 == STATEMENT){
		int data = pkb->getStmtEntry(arg1).ownerNo;
		PARENT_LIST parentList = EvaluateParents::getParentStarResult(STATEMENT, data, 0);
		PARENT_LIST::iterator parentListItr = parentList.begin();
		
		while(parentListItr != parentList.end()){
			if(parentListItr->second == arg2)
				return true;

			parentListItr++;
		}
	}
	// 0028: Contain*(STMT_LIST, CONSTANT/VARIABLE)
	else if(type == STMT_LIST && (pkb->getStmtEntry(arg1).type != PROCEDURE) && (type2 == VARIABLE || type2 == CONSTANT)){
		int data = pkb->getStmtEntry(arg1).ownerNo;
		PARENT_LIST parentList = EvaluateParents::getParentStarResult(STATEMENT, data, 0);
		PARENT_LIST::iterator parentListItr = parentList.begin();
		
		while(parentListItr != parentList.end()){
			if(Helper::isStatementTypeOf(WHILE,parentListItr->second) || Helper::isStatementTypeOf(IF,parentListItr->second)){
				if(pkb->getWhileCtrVar(parentListItr->second) == arg2)
					return true;
				if(pkb->getIfCtrVar(parentListItr->second) == arg2)
					return true;
			}

			if(Helper::isStatementTypeOf(ASSIGNMENT,parentListItr->second)){
				if(EvaluateContains::findPrefixTreeMatch(type2, parentListItr->second,  arg2, 0)){
					return true;
				}
			}
			parentListItr++;
		}
	}
	
	// Contains*(PROCEDURE, STATEMENT/PLUS/MULTIPLY/MINUS/VARIABLE/CONSTANT)
	else if(type == PROCEDURE){

		int start = pkb->getProcedure(arg1)->getStartProgLine();
		int end = pkb->getProcedure(arg1)->getEndProgLine();
		
		// 0029: Contains*(PROCEDURE, STATEMENT)	
		if(type2 == STATEMENT){
			if(arg2 >= start && arg2 <= end)
				return true;
		}

		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		// 0030: Contains*(PROCEDURE, PLUS/MULTIPLY/MINUS/VARIABLE/CONSTANT)
		while(assignListItr!=assignList->end()){
			if(*assignListItr >=start && *assignListItr <= end && EvaluateContains::findPrefixTreeMatch(type2, *assignListItr,  arg2, 0) != -1){
				return true;
			}
			assignListItr++;
		}
	}

	// 0031: Contains*(PLUS/MINUS/MULTIPLY/VARIABLE/CONSTANT, PLUS/MINUS/CONSTANT/VARIABLE/MULITPLY)
	else if((type == PLUS || type == MINUS || type == MULTIPLY || type == VARIABLE || type == CONSTANT) && (type2 == PLUS || type2 == MINUS || type2 == MULTIPLY || type2 == VARIABLE || type2 == CONSTANT)){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			// for this to be true, PLUS must appear before variable/constant in prefix
			int check,check2;
			
			check = EvaluateContains::findPrefixTreeMatch(type, *assignListItr, arg1, 0);
			if(check != -1){
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
					if(operPos == 0 && operPos < *vItr){
						return true;
					}

					else if(operPos != -1 && operPos < *vItr){
						// check precedence
						if(type == type2 || (type2 == MULTIPLY && (type == PLUS || type == MINUS))){
							if(*vItr-operPos == 2){
								return true;
							}
							else{
								operPos = *vItr;
							}
						}
						else{
							return true;
						}
					}
					vItr++;
				}
			}

			
			assignListItr++;
		}
	}

	return false;
}

CONTAIN_LIST EvaluateContains::getContainResult(TYPE type, int arg1, TYPE type2, int arg2){	
	CONTAIN_LIST resultList;

	// 0032: Contains(STMT#, STMT_LIST)
	if(type == STATEMENT && (type2 != VARIABLE && type2 != CONSTANT)&& arg1!=NULL && arg2 == NULL){
		AST_LIST *nodes = pkb->getASTBy(arg1);
		AST *startNode = nodes->front();
		bool ifSwitch = 0;

		if(startNode->getRootType() == STMT_LIST){
			startNode = startNode->getFirstDescendant();
		}

		if(startNode->getRootType() == IF){
			ifSwitch = 1;
		}

		if(type2 == STMT_LIST && (startNode->getRootType() == WHILE || startNode->getRootType() == IF)){
			startNode = startNode->getFirstDescendant()->getRightSibling();
			if(startNode!=NULL){
				resultList.push_back(make_pair(arg1, startNode->getRootStatementNum()));

				// IF has then
				if(ifSwitch){
					startNode = startNode->getRightSibling();
					resultList.push_back(make_pair(arg1, startNode->getRootStatementNum()));
				}
			}
		}
	}

	// 0033: Contains(STMT#, VARIABLE/CONSTANT)
	else if((type == STATEMENT) && ( type2 == VARIABLE ||  type2 == CONSTANT) && arg1!=NULL && arg2 == NULL){
		AST_LIST *nodes = pkb->getASTBy(arg1);
		AST * startNode;
		if( Helper::isStatementTypeOf(STMT_LIST, arg1))
			startNode = nodes->front()->getFirstDescendant();
		else
			startNode = nodes->front();
		
		if(startNode->getRootType() == ASSIGNMENT || startNode->getRootType() == WHILE || startNode->getRootType() == IF){
			if(startNode->getFirstDescendant()->getRootType() == type2 ){
				// (arg1, varIndex/constantIndex)
				resultList.push_back(make_pair(arg1, startNode->getFirstDescendant()->getRootData()));
			}
			if(startNode->getRootType() == ASSIGNMENT && startNode->getFirstDescendant()->getRightSibling()->getRootType() == type2){
				resultList.push_back(make_pair(arg1, startNode->getFirstDescendant()->getRootData()));
			}
		}
	}

	// 0034: Contains(PROCEDURE, STMT_LIST)
	else if(type == PROCEDURE && type2 == STMT_LIST && arg1!=NULL && arg2 == NULL){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			// find the matching ProcIndex
			if(pkb->getStmtEntry(*stmtListItr).type == type && pkb->getStmtEntry(*stmtListItr).ownerNo == arg1)
				resultList.push_back(make_pair(arg1, *stmtListItr));
			stmtListItr++;
		}
	}
	// 0035: Contains(PLUS/MINUS/MULTIPLY, VARIABLE/CONSTANT)
	else if((type == PLUS || type == MINUS || type == MULTIPLY) && arg1 == -1 && arg2 == NULL && (type2 == VARIABLE || type2 == CONSTANT)){
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			EvaluateContains::isStatementDescendantTypeOf(&resultList, type, type2, arg2, *assignListItr);
			assignListItr++;
		}
		resultList.sort();
		resultList.unique();
	}
	
	// 0036: Contains(STMT#/PROCEDURE/IF/WHILE, STMT#)
	else if((type == STATEMENT || type == PROCEDURE || type == IF || type == WHILE) && type2 == STATEMENT && arg2 != NULL && arg1 == NULL){
		AST_LIST *nodeLst = pkb->getASTBy(arg2);
		AST *startNode = nodeLst->front();
		// push everything or specific type, no procedure here
		if((type == STATEMENT || startNode->getRootType() == type || startNode->getAncestor()->getRootType() == type)){
			resultList.push_back(make_pair(startNode->getAncestor()->getRootStatementNum(), arg2));
		}
	}
	// 0038: Contains(STMT_LIST, STMT#/ASSIGNMENT/WHILE/IF/CALL)
	else if(type == STMT_LIST && (type2 == STATEMENT || type2 == ASSIGNMENT || type2 == WHILE || type2 == IF || type2 == CALL) && arg1 == NULL && arg2 != NULL){
		AST_LIST *nodeLst = pkb->getASTBy(arg2);
		AST *startNode = nodeLst->front();
		// same level
		if(startNode->getRootType() == type && startNode->getRootStatementNum() == arg2){
			resultList.push_back(make_pair(startNode->getRootStatementNum(), arg2));
		}
		// diff
		if(startNode->getAncestor()->getRootType() == type){
			resultList.push_back(make_pair(startNode->getAncestor()->getRootStatementNum(), arg2));
		}
	}
	// 0039: Contains(STMT#, VARIABLE)
	else if(type == STATEMENT && type2 == VARIABLE && arg1 == NULL && arg2 != NULL){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			if(EvaluateContains::isStatementDescendantTypeDataOf(ASSIGNMENT, type2, arg2, *assignListItr)){
				resultList.push_back(make_pair(*assignListItr, arg2));
			}
			assignListItr++;
		}

		DATA_LIST * whileList = pkb->getWhiles(arg2);
		DATA_LIST::iterator whileListItr = whileList->begin();
		while(whileListItr!=whileList->end()){
			resultList.push_back(make_pair(*whileListItr, arg2));
			whileListItr++;
		}

		DATA_LIST * ifList = pkb->getIfs(arg2);
		DATA_LIST::iterator ifListItr = ifList->begin();
		while(ifListItr!=ifList->end()){
			resultList.push_back(make_pair(*ifListItr, arg2));
			ifListItr++;
		}
	}
	// 0040: Contains(STMT#, CONSTANT/PLUS/MINUS/MULTIPLY)
	// 0041: Contains(ASSIGNMENT, VARIABLE/CONSTANT/PLUS/MINUS/MULTIPLY)
	else if(((type == STATEMENT && (type2 == CONSTANT || type2 == PLUS || type2 == MINUS || type2 == MULTIPLY)) || (type == ASSIGNMENT && (type2 == VARIABLE || type2 == CONSTANT || type2 == PLUS || type2 == MINUS || type2 == MULTIPLY))) && arg1 == NULL && arg2 != NULL){
		DATA_LIST *assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			if(EvaluateContains::isStatementDescendantTypeDataOf(ASSIGNMENT, type2, arg2, *assignListItr)){
				resultList.push_back(make_pair(*assignListItr, arg2));
			}

			if(EvaluateContains::isStatementDescendantTypeDataOf(ASSIGNMENT, type2, -1, *assignListItr)){
				resultList.push_back(make_pair(*assignListItr, type2));
			}
			assignListItr++;
		}
	}
	
	// 0042: Contains(WHILE, VARIABLE)
	else if(type == WHILE && arg1 == 0 && type2 == VARIABLE && arg2 != NULL){
		// whileTable
		DATA_LIST * whileList = pkb->getWhiles(arg2);
		DATA_LIST::iterator whileListItr = whileList->begin();
		while(whileListItr!=whileList->end()){
			resultList.push_back(make_pair(*whileListItr, arg2));
			whileListItr++;
		}
	}
	// 0043: Contains(IF, VARIABLE)
	else if(type == IF && arg1 == 0 && type2 == VARIABLE && arg2 != NULL){
		DATA_LIST * ifList = pkb->getIfs(arg2);
		DATA_LIST::iterator ifListItr = ifList->begin();
		while(ifListItr!=ifList->end()){
			resultList.push_back(make_pair(*ifListItr, arg2));
			ifListItr++;
		}
	}

	// 0044: Contains(STMT#/WHILE/IF, STMT_LIST)
	else if((type == STATEMENT || type == WHILE || type == IF) && arg1 == 0 && type2 == STMT_LIST && arg2 == 0){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			if((type == STATEMENT || pkb->getStmtEntry(*stmtListItr).type == type) && pkb->getStmtEntry(*stmtListItr).type != PROCEDURE){
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, *stmtListItr));
			}
			else if(type == IF && (pkb->getStmtEntry(*stmtListItr).type == THEN || pkb->getStmtEntry(*stmtListItr).type == ELSE)){
				resultList.push_back(make_pair(pkb->getStmtEntry(*stmtListItr).ownerNo, *stmtListItr));
			}
			stmtListItr++;
		}
	}

	// 0045: Contains(STMT_LIST, ASSIGNMENT/WHILE/IF/CALL/ASSIGNMENT)
	else if(type == STMT_LIST && (type2 == STATEMENT || type2 == WHILE || type2 == IF || type2 == CALL || type2 == ASSIGNMENT) && arg1 == 0 && arg2 == 0){
		DATA_LIST * stmtList =pkb->getAllStmtLst();
		DATA_LIST::iterator stmtListItr = stmtList->begin();
		while(stmtListItr!=stmtList->end()){
			AST_LIST *astList = pkb->getASTBy(*stmtListItr);
			AST *startNode = astList->front()->getFirstDescendant();

			while(startNode != NULL){
				if((type2 == STATEMENT || startNode->getRootType() == type2)){
					resultList.push_back(make_pair(*stmtListItr, startNode->getRootStatementNum()));
				}
				startNode = startNode->getRightSibling();
			}


			stmtListItr++;
		}
	}
	// 0046: Contains(IF, VARIABLE)
	else if((type == IF) && type2 == VARIABLE  && arg1 == 0 && arg2 == 0){
		DATA_LIST * ifList = pkb->getAllIfs();
		DATA_LIST::iterator ifListItr =  ifList->begin();
		
		while(ifListItr != ifList->end()){
			resultList.push_back(make_pair(*ifListItr, pkb->getIfCtrVar(*ifListItr)));
			ifListItr++;
		}
	}
	// 0047: Contains(WHILE, VARIABLE)
	else if((type == WHILE) && type2 == VARIABLE  && arg1 == 0 && arg2 == 0){
		DATA_LIST * whileList = pkb->getAllWhiles();
		DATA_LIST::iterator whileListItr = whileList->begin();
		
		while(whileListItr != whileList->end()){
			resultList.push_back(make_pair(*whileListItr, pkb->getWhileCtrVar(*whileListItr)));
			whileListItr++;
		}

	}
	// 0048: Contains(STMT#, VARIABLE)
	else if((type == STATEMENT) && type2 == VARIABLE  && arg1 == 0 && arg2 == 0){
		// Get all control variables
		DATA_LIST * whileList = pkb->getAllWhiles();
		DATA_LIST::iterator whileListItr = whileList->begin();
		
		while(whileListItr != whileList->end()){
			resultList.push_back(make_pair(*whileListItr, pkb->getWhileCtrVar(*whileListItr)));
			whileListItr++;
		}

		DATA_LIST * ifList = pkb->getAllIfs();
		DATA_LIST::iterator ifListItr =  ifList->begin();
		
		while(ifListItr != ifList->end()){
			resultList.push_back(make_pair(*ifListItr, pkb->getIfCtrVar(*ifListItr)));
			ifListItr++;
		}

		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();

		while(assignListItr!=assignList->end()){
			resultList.push_back(make_pair(*assignListItr, pkb->getAssignEntry(*assignListItr).varIndex));
			DATA_LIST arg2List = EvaluateContains::getDataListTypeDescendantOf(type, type2, *assignListItr);
			DATA_LIST::iterator arg2ListItr = arg2List.begin();
			
			while(arg2ListItr!=arg2List.end()){
				resultList.push_back(make_pair(*assignListItr,*arg2ListItr));
				arg2ListItr++;
			}

			assignListItr++;
		}
	}
	// 0049: Contains(STMT#, CONSTANT)
	// 0050: Contains(ASSIGNMENT, VARIABLE/CONSTANT)
	else if((type == STATEMENT && type2 == CONSTANT) || (type == ASSIGNMENT && (type2 == VARIABLE || type2 == CONSTANT)) && arg1 == 0 && arg2 == 0){
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		
		while(assignListItr!=assignList->end()){
			DATA_LIST arg2List = EvaluateContains::getDataListTypeDescendantOf(ASSIGNMENT, type2, *assignListItr);
			DATA_LIST::iterator arg2ListItr = arg2List.begin();
			
			while(arg2ListItr!=arg2List.end()){
				resultList.push_back(make_pair(*assignListItr,*arg2ListItr));
				arg2ListItr++;
			}

			assignListItr++;
		}
	}
	// 0051: Contains(PROCEDURE, STMT_LIST)
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

	resultList.sort();
	resultList.unique();
	return resultList;
}

bool EvaluateContains::getIsContainResult(TYPE type, int arg1, TYPE type2, int arg2)
{
	// 0052: Contains(STMT#, STMT#)
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

	// 0055: Contains(STMT#, VARIABLE)
	else if(type == STATEMENT && type2 == VARIABLE && arg1 != NULL && arg2 != NULL
		&& (Helper::isStatementTypeOf(WHILE, arg1) || Helper::isStatementTypeOf(IF, arg1))){

		if(EvaluateContains::isStatementDescendantTypeDataOf(WHILE, type2, arg2, arg1)){
			return true;
		}
		else if(EvaluateContains::isStatementDescendantTypeDataOf(IF, type2, arg2, arg1)){
			return true;
		}
	}

	// 0056: Contains(STMT#, CONSTANT/VARIABLE/PLUS/MINUS/MULTIPLY), where STATEMENT is ASSIGNMENT
	else if(type == STATEMENT && arg1 != NULL && arg2 != NULL &&
		Helper::isStatementTypeOf(ASSIGNMENT, arg1) && (type2 == CONSTANT || type2 == VARIABLE || type2 == PLUS  || type2 == MINUS || type2 == MULTIPLY)){
			if(EvaluateContains::isStatementDescendantTypeDataOf(ASSIGNMENT, type2, arg2, arg1)){
				return true;
			}
	}

	// 0057: Contains(PROCEDURE, STMT#)
	else if((type == PROCEDURE && type2 == STATEMENT)  && arg1 != NULL && arg2 != NULL){
		// If the stmt_list has the same stmt# as the procedure's first stmt#
		if(Helper::isStatementTypeOf(STMT_LIST, arg2) && pkb->getProcedure(arg1)->getStartProgLine() == arg2){
			return true;
		}
		return false;
	}

	// 0058: Contains(PLUS/MINUS/MULTIPLY, VARIABLE/CONSTANT/PLUS/MINUS/MULTIPLY)
	else if((type == PLUS  || type == MINUS || type == MULTIPLY)&& (type2 == VARIABLE ||type2 ==  CONSTANT || type2== PLUS || type2 == MINUS || type2 == MULTIPLY)){
		
		DATA_LIST * assignList = pkb->getAllAssigns();
		DATA_LIST::iterator assignListItr = assignList->begin();
		while(assignListItr!=assignList->end()){
			if((type2 == VARIABLE || type2 == CONSTANT) && EvaluateContains::isStatementDescendantTypeDataOf(type, type2, arg2, *assignListItr)){
				return true;
			}
			else if(EvaluateContains::isStatementDescendantTypeDataOf(type, type2, -1, *assignListItr)){
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
		if(typeName == STATEMENT)
			typeName = ASSIGNMENT;
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
				//cout << "as: " << stmtNum << "\n";
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

void EvaluateContains::isStatementDescendantTypeOf(CONTAIN_LIST* dList, TYPE typeName, TYPE typeName2, int data, STATEMENT_NUM stmtNum){
	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){
		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
			
		// search if RHS and LHS has type2 and data
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName){
				// check descendant
				AST * descendant = (*itr)->getFirstDescendant();
				
				if(pkb->getType(descendant) == typeName2){
					if(typeName == PROCEDURE){
						dList->push_back(make_pair(data, pkb->getData(descendant)));
					}
					// add data to answer
					else
						dList->push_back(make_pair(typeName, pkb->getData(descendant)));

				}
				if(pkb->getType(descendant->getRightSibling()) == typeName2){
					// add data to answer
					if(typeName == PROCEDURE){
						dList->push_back(make_pair(data, pkb->getData(descendant->getRightSibling())));
					}
					else
						dList->push_back(make_pair(typeName, pkb->getData(descendant->getRightSibling())));
				}
			}
		}
	}
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
				
				if(pkb->getType(descendant) == typeName2){
					if(typeName2 == CONSTANT && pkb->getConstantValue( pkb->getData(descendant)) == data){
						return true;
					}
					else if(typeName2 != CONSTANT && pkb->getData(descendant) == data){
						return true;
					}
				}
				if(pkb->getType(descendant->getRightSibling()) == typeName2){
					if(typeName2 == CONSTANT && pkb->getConstantValue( pkb->getData(descendant->getRightSibling())) == data){
						return true;
					}
					else if(typeName2 != CONSTANT && pkb->getData(descendant->getRightSibling()) == data){
						return true;
					}
				}
			}
		}
	}
	
	return false;
}

 int EvaluateContains::findPrefixTreeMatch(TYPE type2, STATEMENT_NUM stmt, int data, int offset){
	// cout << pkb->getAssignEntry(stmt).prefixTree << "\n";
	 if(type2 == VARIABLE && pkb->getAssignEntry(stmt).prefixTree.find(pkb->getVarName(data),offset) != pkb->getAssignEntry(stmt).prefixTree.npos){
		int o =  pkb->getAssignEntry(stmt).prefixTree.find(pkb->getVarName(data),offset);
		// only 1 variable
		if( o == 0 && pkb->getAssignEntry(stmt).prefixTree.length() == pkb->getVarName(data).length()){
			return o;
		}
		// last variable
		else if(o == pkb->getAssignEntry(stmt).prefixTree.length()-pkb->getVarName(data).length() && pkb->getAssignEntry(stmt).prefixTree.find(" ",o-1) == o-1)
		{
			return o;
		}
		// first variable
		else if(o == 0 && pkb->getAssignEntry(stmt).prefixTree.find(" ",o+ pkb->getVarName(data).length()) == o+ pkb->getVarName(data).length())
		{
			return o;
		}
		// in the middle
		else if(pkb->getAssignEntry(stmt).prefixTree.find(" ",o+ pkb->getVarName(data).length()) == o+ pkb->getVarName(data).length() &&  pkb->getAssignEntry(stmt).prefixTree.find(" ",o-1) == o-1)
		{
			return o;
		}
	}
	 
	if(type2 == CONSTANT && pkb->getAssignEntry(stmt).prefixTree.find(static_cast<ostringstream*>( &(ostringstream() << data) )->str(),offset) != pkb->getAssignEntry(stmt).prefixTree.npos){
		int o = pkb->getAssignEntry(stmt).prefixTree.find(static_cast<ostringstream*>( &(ostringstream() << data) )->str(),offset);
		int length = static_cast<ostringstream*>( &(ostringstream() << data) )->str().length();
		// 1 number only
		
		// only 1 digit
		if( o == 0 && pkb->getAssignEntry(stmt).prefixTree.length() == length){
			return o;
		}
		// last digit
		else if(o == pkb->getAssignEntry(stmt).prefixTree.length()-length && pkb->getAssignEntry(stmt).prefixTree.find(" ",o-1) == o-1)
		{
			return o;
		}
		// first digit
		else if(o == 0 && pkb->getAssignEntry(stmt).prefixTree.find(" ",o+length) == o+length)
		{
			return o;
		}
		else if(pkb->getAssignEntry(stmt).prefixTree.find(" ",o+length) == o+length &&  pkb->getAssignEntry(stmt).prefixTree.find(" ",o-1) == o-1)
		{
			return o;
		}
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

void EvaluateContains::getParentInsertAll(CONTAIN_LIST* dList, TYPE type, int data, STATEMENT_NUM i){
	PARENT_LIST pList = EvaluateParents::getParentStarResult(type, 0, i);
	PARENT_LIST::iterator pListItr = pList.begin();
	while(pListItr!=pList.end()){
		if(Helper::isStatementTypeOf(IF, pListItr->first)){
				
			AST *elseNode, *thenNode;
					
			if(Helper::isStatementTypeOf(STMT_LIST, pListItr->first)){
				thenNode = pkb->getASTBy(pListItr->first)->front()->getFirstDescendant()->getFirstDescendant()->getRightSibling();
				elseNode = pkb->getASTBy(pListItr->first)->front()->getFirstDescendant()->getFirstDescendant()->getRightSibling()->getRightSibling();
			}
			else{
				thenNode= pkb->getASTBy(pListItr->first)->front()->getFirstDescendant()->getRightSibling();
				elseNode= pkb->getASTBy(pListItr->first)->front()->getFirstDescendant()->getRightSibling()->getRightSibling();
			}
			int thenStmt = pkb->getStatementNum(thenNode);
			int elseStmt = pkb->getStatementNum(elseNode);

			// belongs to ELSE
			if(i >= elseStmt && type == STMT_LIST){
				dList->push_back(make_pair(elseStmt,  data));
			}
			// belongs to THEN
			else if(type == STMT_LIST){
				dList->push_back(make_pair(thenStmt,  data));
			}
			else if(type == STATEMENT || type == IF){
				dList->push_back(make_pair(pListItr->first, data));
			}
		}
		else{
			dList->push_back(make_pair(pListItr->first, data));
		}
		pListItr++;
	}
}

// Insert (i, type2's data)
void EvaluateContains::getTypeInsertAll(CONTAIN_LIST* dList, TYPE type, TYPE type2, STATEMENT_NUM i, STATEMENT_NUM rootI){
	
	if(Helper::isStatementTypeOf(ASSIGNMENT, i) && type2 == VARIABLE){
		if(type == ASSIGNMENT || type == STATEMENT || type == STMT_LIST)
			dList->push_back(make_pair(rootI, pkb->getAssignEntry(i).varIndex));
		
		EvaluateContains::getParentInsertAll(dList, type, pkb->getAssignEntry(i).varIndex, i);

		string prefix = pkb->getAssignEntry(i).prefixTree;
		DATA_LIST varIndexes =  EvaluateContains::getPrefixTreeVarIndexes(prefix, 0);
		DATA_LIST::iterator varItr = varIndexes.begin();

		while(varItr!=varIndexes.end()){
			if(type == ASSIGNMENT || type == STATEMENT || type == STMT_LIST)
				dList->push_back(make_pair(rootI, *varItr));
			EvaluateContains::getParentInsertAll(dList, type, *varItr, i);
			varItr++;
		}
	}

	if(Helper::isStatementTypeOf(ASSIGNMENT, i) && type2 == CONSTANT){
		string prefix = pkb->getAssignEntry(i).prefixTree;
		DATA_LIST constantIndexes =  EvaluateContains::getPrefixTreeConstants(prefix, 0);
		DATA_LIST::iterator constantItr = constantIndexes.begin();

		while(constantItr!=constantIndexes.end()){
			if(type == ASSIGNMENT || type == STATEMENT || type == STMT_LIST)
				dList->push_back(make_pair(rootI, *constantItr));

			EvaluateContains::getParentInsertAll(dList, type, *constantItr, i);
			constantItr++;
		}
	}

	if(Helper::isStatementTypeOf(ASSIGNMENT, i) && ( type2 == PLUS ||  type2 == MINUS ||  type2 == MULTIPLY)){
		string prefix = pkb->getAssignEntry(i).prefixTree;
		DATA_LIST operatorIndexes =  EvaluateContains::getPrefixTreeOperators(prefix,type2, 0);
		DATA_LIST::iterator operatorItr = operatorIndexes.begin();

		while(operatorItr!=operatorIndexes.end()){
			if(type == ASSIGNMENT || type == STATEMENT || type == STMT_LIST)
				dList->push_back(make_pair(rootI, *operatorItr));
			EvaluateContains::getParentInsertAll(dList, type, *operatorItr, i);
			operatorItr++;
		}
	}

	if(Helper::isStatementTypeOf(WHILE, i) && type2 == VARIABLE){
		if(type == WHILE || type == STATEMENT || type == STMT_LIST)
			dList->push_back(make_pair(rootI, pkb->getWhileCtrVar(i)));
		EvaluateContains::getParentInsertAll(dList, type, pkb->getWhileCtrVar(i), i);
	}

	if(Helper::isStatementTypeOf(IF, i) && type2 == VARIABLE){
		if(type == IF || type == STATEMENT || type == STMT_LIST)
			dList->push_back(make_pair(rootI, pkb->getIfCtrVar(i)));
		EvaluateContains::getParentInsertAll(dList, type, pkb->getIfCtrVar(i), i);
	}
}
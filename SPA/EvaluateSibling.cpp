#include "EvaluateSibling.h"
#include <stack>

PKB * EvaluateSibling::pkb =NULL;
EvaluateSibling::EvaluateSibling(void)
{
}


EvaluateSibling::~EvaluateSibling(void)
{
}

bool EvaluateSibling::getIsSibling(TYPE type1,TYPE type2,INDEX index1,INDEX index2){
	
	
	if(type1 ==PROCEDURE&&type2 ==PROCEDURE){ //1. Sibling("procName1","procName2"
		if(index1!=NULL&&index2!=NULL) return true;	
	}else if(Helper::isStatement(type1)&&Helper::isStatement(type2)){//2. Sibling(1,5),Sibling(4,2) for stmt/w/a/if/call
		if((type1==STATEMENT||Helper::isStatementTypeOf(type1,index1))&&(type2==STATEMENT||Helper::isStatementTypeOf(type2,index2))){
			return (EvaluateFollows::getIsFollowsStarResult(index1,index2)||EvaluateFollows::getIsFollowsResult(index2,index1));
		}
	}else if(type1==VARIABLE&&type2==STMT_LIST){//3. a.Sibling("x",4) for while or if stmt
		AST * stmtListNode = (*pkb->getASTBy(index2)->begin())->getAncestor();
		AST * parentNode = stmtListNode->getAncestor();		//get the parentNode to check if it is while or if
		AST * varNode = parentNode->getFirstDescendant();	//get the varNode of while or if

		if(varNode->getRootType() ==VARIABLE&&varNode->getRootData()==index1)
			return true;
	
	}else if(type2==VARIABLE&&type1==STMT_LIST){ //3. b.Sibling(4,"x") for while of if stmt
		AST * stmtListNode = (*pkb->getASTBy(index1)->begin())->getAncestor();
		AST * parentNode = stmtListNode->getAncestor();		//get the parentNode to check if it is while or if
		AST * varNode = parentNode->getFirstDescendant();	//get the varNode of while or if

		if(varNode->getRootType() ==VARIABLE&&varNode->getRootData()==index2)
			return true;
		
	}else if(type1==STMT_LIST&&type2==STMT_LIST){//3. c. Sibling( 22,23) for stmtList of if
		AST * stmtListNode1 = (*pkb->getASTBy(index1)->begin())->getAncestor();
		AST * stmtListNode2 = (*pkb->getASTBy(index2)->begin())->getAncestor();

		if(stmtListNode1->getAncestor()->getRootData()==stmtListNode2->getAncestor()->getRootData())
			return true;
	}else if(Helper::isExprElement(type1)&&Helper::isExprElement(type2)){//4. Sibling ("x"/"*"/"+"/"-","x"/"*"/"+"/"-"). operator data: -1
		DATA_LIST * assignList = pkb->getAllAssigns();
		
		for(DATA_LIST::iterator itr= assignList->begin();itr!=assignList->end();itr++){
			//get assignNode
			AST * assignNode = *(pkb->getASTBy(*itr)->begin());

			//check if modified var and expression tree root are sibling
			AST * leftVarNode = assignNode->getFirstDescendant();
			AST * expressionRoot = leftVarNode->getRightSibling();
			
			//pattern is symmetric
			bool isPattern1 = (leftVarNode->getRootType()==type1&&leftVarNode->getRootData()==index1)&&(expressionRoot->getRootType()==type2&&expressionRoot->getRootData()==index2);
			bool isPattern2 =(expressionRoot->getRootType()==type1&&expressionRoot->getRootData()==index1)&&(leftVarNode->getRootType()==type2&&leftVarNode->getRootData()==index2);
			if(isPattern1||isPattern2) return true; // x = expression, x is matched with var or +,-,*

			//traverse the tree and check matched pattern
			stack<AST *> exprStack;
			if(expressionRoot->getRootType()!=VARIABLE&&expressionRoot->getRootType()!=CONSTANT){
				exprStack.push(expressionRoot);
				AST * left;
				AST * right;

				while(!exprStack.empty()){
					AST * curNode = exprStack.top();
					exprStack.pop();

					left = curNode->getFirstDescendant();
					right = left->getRightSibling();

					//pattern is symmetric
					isPattern1 = (left->getRootType()==type1&&left->getRootData()==index1)&&(right->getRootType()==type2&&right->getRootData()==index2);
					isPattern2 =(right->getRootType()==type1&&right->getRootData()==index1)&&(left->getRootType()==type2&&left->getRootData()==index2);
					if(isPattern1||isPattern2) return true;

					//push the next operator into stack
					if(left->getRootType()!=VARIABLE&&left->getRootType()!=CONSTANT) exprStack.push(left);
					if(right->getRootType()!=VARIABLE&&right->getRootType()!=CONSTANT) exprStack.push(right);
				}
			}
			
		}
		delete assignList;
	}
	return false;
}

RELATION_LIST EvaluateSibling::getSiblingResult(TYPE type1,TYPE type2,INDEX index1,INDEX index2){
	if(index1=-2&&index2!=-2){ //sibling(unknown,known)
		return getOneSibling(type2,type1,index2,2);
	}else if(index1!=-2&&index2==-2){ //sibling(known,unknown)
		return getOneSibling(type1,type2,index1,1);
	}else {
		return getSiblingUnknown(type1,type2);
	}
}

RELATION_LIST EvaluateSibling::getOneSibling(TYPE known,TYPE unknown,INDEX knownIndex,int knownPos){
	RELATION_LIST returnList;
	if(known==PROCEDURE&&unknown==PROCEDURE){ //1. sibling ( p1,p2)
		int procTableSize = pkb->getProceTableSize();

		for(int i =1;i<procTableSize;i++){
			if(knownIndex!=i){
				if(knownPos==1){ //sibling (known,unknown)
					returnList.push_back(pair<int,int>(knownIndex,i));
				}else{ //sibling(unknown,known)
					returnList.push_back(pair<int,int>(i,knownIndex));
				}
			} 
		}
	}else if(Helper::isStatement(known)&&Helper::isStatement(unknown)){//2. sibling(stmt/a/w/if/call,stmt/a/w/if/call)
		if(knownPos==1){
			//get sequential sibling from Follows*
			returnList = EvaluateFollows::getFollowsStarResult(unknown,knownIndex,0);
			//get sibling in front of the knownIndex
			RELATION_LIST tempList = EvaluateFollows::getFollowsStarResult(unknown,0,knownIndex);
			for(RELATION_LIST::iterator itr=tempList.begin();itr!=tempList.end();itr++){
				returnList.push_back(pair<int,int>(knownIndex,itr->first));
			}
		}else{
			//get sibling before current index from Follows*
			returnList = EvaluateFollows::getFollowsStarResult(unknown,0,knownIndex);
			//get sequential siblings
			RELATION_LIST tempList = EvaluateFollows::getFollowsStarResult(unknown,knownIndex,0);
			for(RELATION_LIST::iterator itr=tempList.begin();itr!=tempList.end();itr++){
				returnList.push_back(pair<int,int>(itr->second,knownIndex));
			}
		}
	}else if(known==VARIABLE&&unknown==STMT_LIST){//3. a. sibling( x, stmtList)
		if(knownPos==1){	//sibling("x",stmtList)
			//get all the whiles
			DATA_LIST * whileList = pkb->getAllWhiles();
			for(DATA_LIST::iterator itr=whileList->begin();itr!=whileList->end();itr++){

				if(pkb->getWhileCtrVar(*itr)==knownIndex)
					returnList.push_back(pair<int,int>(knownIndex,*itr+1));
			}
			delete whileList;

			//get all the ifs
			DATA_LIST * ifList = pkb->getAllIfs();
			for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
				AST * ifVarNode = *pkb->getASTBy(*itr)->begin();
				AST * elseNode = ifVarNode->getRightSibling();
				AST * thenNode = elseNode->getRightSibling();

				if(ifVarNode->getRootData()==knownIndex){
					returnList.push_back(pair<int,int>(knownIndex,elseNode->getFirstDescendant()->getRootData()));
					returnList.push_back(pair<int,int>(knownIndex,thenNode->getFirstDescendant()->getRootData()));
				}
			}
			delete ifList;
		}else{ //sibling(var,"1")
			//get stmtList Node
			AST * stmtListNode = *pkb->getASTBy(knownIndex)->begin();
			AST * parentNode= stmtListNode->getAncestor();

			if(parentNode->getRootType()==WHILE||parentNode->getRootType()==IF){
				returnList.push_back(pair<int,int>(parentNode->getFirstDescendant()->getRootData(),knownIndex));
			}
		}
				
	}else  if(known==STMT_LIST&&unknown==STMT_LIST){ //sibling (stmtList,stmtList)
		if(knownPos==1){
			//get all the ifs
			DATA_LIST * ifList = pkb->getAllIfs();
			for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
				AST * ifVarNode = *pkb->getASTBy(*itr)->begin();
				AST * elseNode = ifVarNode->getRightSibling();
				AST * thenNode = elseNode->getRightSibling();

				if(knownIndex==elseNode->getFirstDescendant()->getRootData()){
					returnList.push_back(pair<int,int>(knownIndex,thenNode->getFirstDescendant()->getRootData()));
				}else if(knownIndex= thenNode->getFirstDescendant()->getRootData()){
					returnList.push_back(pair<int,int>(knownIndex,elseNode->getFirstDescendant()->getRootData()));
				}
			}
			delete ifList;
		}else{
			//get all the ifs
			DATA_LIST * ifList = pkb->getAllIfs();
			for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
				AST * ifVarNode = *pkb->getASTBy(*itr)->begin();
				AST * elseNode = ifVarNode->getRightSibling();
				AST * thenNode = elseNode->getRightSibling();

				if(knownIndex==elseNode->getFirstDescendant()->getRootData()){
					returnList.push_back(pair<int,int>(thenNode->getFirstDescendant()->getRootData(),knownIndex));
				}else if(knownIndex= thenNode->getFirstDescendant()->getRootData()){
					returnList.push_back(pair<int,int>(elseNode->getFirstDescendant()->getRootData(),knownIndex));
				}
			}
			delete ifList;

		}
		
	}else if(Helper::isExprElement(known)&&Helper::isExprElement(unknown)){//4. Sibling (var/plus/minus/multiply,var/plus/minus/multiply). 
		DATA_LIST * assignList = pkb->getAllAssigns();
		
		for(DATA_LIST::iterator itr= assignList->begin();itr!=assignList->end();itr++){
			//get assignNode
			AST * assignNode = *(pkb->getASTBy(*itr)->begin());

			//check if modified var and expression tree root are sibling
			AST * leftVarNode = assignNode->getFirstDescendant();
			AST * expressionRoot = leftVarNode->getRightSibling();
			
			/*pattern is symmetric
				if else pattern check is used to avoid repeated result
				eg. x->x in expression tree, sibling("x",var)/sibling(var,"x")
					if pattern1 is matched, no need to check pattern2
				eg. x->+ in expressoin tree, sibling("+",var)/sibling(var,"+")
					if pattern1 is matched, pattern2 can't be matched
			*/
			bool isPattern1 = (leftVarNode->getRootType()==known&&leftVarNode->getRootData()==knownIndex)&&(expressionRoot->getRootType()==unknown);
			bool isPattern2 =(expressionRoot->getRootType()==known&&expressionRoot->getRootData()==knownIndex)&&(leftVarNode->getRootType()==unknown);
			if(isPattern1){
				int siblingValue =EvaluateSibling::getOperatorValue(expressionRoot->getRootType());
				if(siblingValue==-1){
					siblingValue==expressionRoot->getRootData();
				}
				if(knownPos==1){
					returnList.push_back(pair<int,int>(knownIndex,siblingValue));
				}else{
					returnList.push_back(pair<int,int>(siblingValue,knownIndex));
				}
			} else if(isPattern2){
				if(knownPos==1){
					returnList.push_back(pair<int,int>(knownIndex,leftVarNode->getRootData()));
				}else{
					returnList.push_back(pair<int,int>(leftVarNode->getRootData(),knownIndex));
				}
			} // x = expression, x is matched with var or +,-,*

			//traverse the tree and check matched pattern
			stack<AST *> exprStack;
			if(expressionRoot->getRootType()!=VARIABLE&&expressionRoot->getRootType()!=CONSTANT){
				exprStack.push(expressionRoot);
				AST * left;
				AST * right;

				while(!exprStack.empty()){
					AST * curNode = exprStack.top();
					exprStack.pop();

					left = curNode->getFirstDescendant();
					right = left->getRightSibling();

					//pattern is symmetric
					isPattern1 = (left->getRootType()==known&&left->getRootData()==knownIndex)&&(right->getRootType()==unknown);
					isPattern2 =(right->getRootType()==known&&right->getRootData()==knownIndex)&&(left->getRootType()==unknown);
					if(isPattern1){
						int rightValue = EvaluateSibling::getOperatorValue(right->getRootType());
						
						if(rightValue==-1){
							rightValue==right->getRootData();
						}
						if(knownPos==1){
							returnList.push_back(pair<int,int>(known,rightValue));
						}else{
							returnList.push_back(pair<int,int>(rightValue,known));
						}
						
					} else if(isPattern2){
						int leftValue =EvaluateSibling::getOperatorValue(left->getRootType());
					
						if(leftValue==-1){
							leftValue==left->getRootData();
						}
						if(knownPos==1){
							returnList.push_back(pair<int,int>(knownIndex,leftValue));
						}else{
							returnList.push_back(pair<int,int>(leftValue,knownIndex));
						}
						
					} // x = expression, x is matched with var or +,-,*

					//push the next operator into stack
					if(left->getRootType()!=VARIABLE&&left->getRootType()!=CONSTANT) exprStack.push(left);
					if(right->getRootType()!=VARIABLE&&right->getRootType()!=CONSTANT) exprStack.push(right);
				}
			}
			
		}
		delete assignList;
	}
	return returnList;
}
RELATION_LIST EvaluateSibling::getSiblingUnknown(TYPE type1,TYPE type2){
	RELATION_LIST returnList;
	if(type1==PROCEDURE&&type2==PROCEDURE){ //1. sibling ( p1,p2)
		int procTableSize = pkb->getProceTableSize();

		for(int i =1;i<procTableSize;i++){
			for(int j =2;j<procTableSize;j++){
				if(i!=j) returnList.push_back(pair<int,int>(i,j));
			}
		}
	}else if(Helper::isStatement(type1)&&Helper::isStatement(type2)){//2. sibling(stmt/a/w/if/call,stmt/a/w/if/call)
		//get sequential sibling from Follows*
		returnList = EvaluateFollows::getFollowsStarResult(STATEMENT,STATEMENT);

		//insert reversed sibling list
		RELATION_LIST tempList = returnList;
		for(RELATION_LIST::iterator itr=tempList.begin();itr!=tempList.end();itr++){
			returnList.push_back(pair<int,int>(itr->second,itr->first));
		}
	}else if(type1==VARIABLE&&type2==STMT_LIST){//3. a. sibling( x, stmtList)
		//get all the whiles
		DATA_LIST * whileList = pkb->getAllWhiles();
		for(DATA_LIST::iterator itr=whileList->begin();itr!=whileList->end();itr++){
			returnList.push_back(pair<int,int>(pkb->getWhileCtrVar(*itr),*itr+1));
		}
		delete whileList;

		//get all the ifs
		DATA_LIST * ifList = pkb->getAllIfs();
		for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
			AST * ifVarNode = *pkb->getASTBy(*itr)->begin();
			AST * elseNode = ifVarNode->getRightSibling();
			AST * thenNode = elseNode->getRightSibling();

			returnList.push_back(pair<int,int>(ifVarNode->getRootData(),elseNode->getFirstDescendant()->getRootData()));
			returnList.push_back(pair<int,int>(ifVarNode->getRootData(),thenNode->getFirstDescendant()->getRootData()));
		}
		delete ifList;
	}else if(type1==STMT_LIST&&type2==VARIABLE){ //3.b sibling(stmtList,x)
		//get all the whiles
		DATA_LIST * whileList = pkb->getAllWhiles();
		for(DATA_LIST::iterator itr=whileList->begin();itr!=whileList->end();itr++){
			returnList.push_back(pair<int,int>(*itr+1,pkb->getWhileCtrVar(*itr)));
		}
		delete whileList;

		//get all the ifs
		DATA_LIST * ifList = pkb->getAllIfs();
		for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
			AST * ifVarNode = *pkb->getASTBy(*itr)->begin();
			AST * elseNode = ifVarNode->getRightSibling();
			AST * thenNode = elseNode->getRightSibling();

			returnList.push_back(pair<int,int>(elseNode->getFirstDescendant()->getRootData(),ifVarNode->getRootData()));
			returnList.push_back(pair<int,int>(thenNode->getFirstDescendant()->getRootData(),ifVarNode->getRootData()));
		}
		delete ifList;
	}else if(type1==STMT_LIST&&type2==STMT_LIST){ //sibling (stmtList,stmtList)
		//get all the ifs
		DATA_LIST * ifList = pkb->getAllIfs();
		for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
			AST * ifVarNode = *pkb->getASTBy(*itr)->begin();
			AST * elseNode = ifVarNode->getRightSibling();
			AST * thenNode = elseNode->getRightSibling();

			returnList.push_back(pair<int,int>(elseNode->getFirstDescendant()->getRootData(),thenNode->getFirstDescendant()->getRootData()));
			returnList.push_back(pair<int,int>(thenNode->getFirstDescendant()->getRootData(),elseNode->getFirstDescendant()->getRootData()));
		}
		delete ifList;
	}else if(Helper::isExprElement(type1)&&Helper::isExprElement(type2)){//4. Sibling (var/plus/minus/multiply,var/plus/minus/multiply). 
		DATA_LIST * assignList = pkb->getAllAssigns();
		
		for(DATA_LIST::iterator itr= assignList->begin();itr!=assignList->end();itr++){
			//get assignNode
			AST * assignNode = *(pkb->getASTBy(*itr)->begin());

			//check if modified var and expression tree root are sibling
			AST * leftVarNode = assignNode->getFirstDescendant();
			AST * expressionRoot = leftVarNode->getRightSibling();
			
			//pattern is symmetric
			bool isPattern1 = (leftVarNode->getRootType()==type1)&&(expressionRoot->getRootType()==type2);
			bool isPattern2 =(expressionRoot->getRootType()==type1)&&(leftVarNode->getRootType()==type2);
			bool isLeftRightEqual = (leftVarNode->getRootType()==expressionRoot->getRootType())&&(leftVarNode->getRootData()==expressionRoot->getRootData());
			
			if(isPattern1){  //type1 matched with left, type2 matched with right
				int siblingValue =EvaluateSibling::getOperatorValue(expressionRoot->getRootType());
				if(siblingValue==-1){ //check if the expressionNode is an operator
					siblingValue==expressionRoot->getRootData();
				}
				returnList.push_back(pair<int,int>(leftVarNode->getRootData(),siblingValue));
			}
			if(!isLeftRightEqual){ //to avoid duplicate result
				if(isPattern2){	//type1 matched with right, type2 matched with left
					int siblingValue = EvaluateSibling::getOperatorValue(expressionRoot->getRootType());
					if(siblingValue==-1){
						siblingValue==expressionRoot->getRootData();
					}
					returnList.push_back(pair<int,int>(siblingValue,leftVarNode->getRootData()));
				} // x = expression, x is matched with var or +,-,*
			}
			//traverse the tree and check matched pattern
			stack<AST *> exprStack;
			if(expressionRoot->getRootType()!=VARIABLE&&expressionRoot->getRootType()!=CONSTANT){
				exprStack.push(expressionRoot);
				AST * left;
				AST * right;

				while(!exprStack.empty()){
					AST * curNode = exprStack.top();
					exprStack.pop();

					left = curNode->getFirstDescendant();
					right = left->getRightSibling();

					//pattern is symmetric
					isPattern1 = (left->getRootType()==type1)&&(right->getRootType()==type2);
					isPattern2 =(right->getRootType()==type1)&&(left->getRootType()==type2);
					isLeftRightEqual = (left->getRootType()==right->getRootType())&&(left->getRootData()==right->getRootData());
					
					if(isPattern1){
						int leftValue =EvaluateSibling::getOperatorValue(left->getRootType());
						int rightValue = EvaluateSibling::getOperatorValue(right->getRootType());
						if(leftValue==-1){
							leftValue==left->getRootData();
						}
						if(rightValue==-1){
							rightValue==right->getRootData();
						}
						returnList.push_back(pair<int,int>(leftValue,rightValue));
					} 
					
					if(!isLeftRightEqual){
						if(isPattern2){
							int leftValue =EvaluateSibling::getOperatorValue(left->getRootType());
							int rightValue = EvaluateSibling::getOperatorValue(right->getRootType());
							if(leftValue==-1){
								leftValue==left->getRootData();
							}
							if(rightValue==-1){
								rightValue==right->getRootData();
							}
							returnList.push_back(pair<int,int>(rightValue,leftValue));
						} // x = expression, x is matched with var or +,-,*
					}
					//push the next operator into stack
					if(left->getRootType()!=VARIABLE&&left->getRootType()!=CONSTANT) exprStack.push(left);
					if(right->getRootType()!=VARIABLE&&right->getRootType()!=CONSTANT) exprStack.push(right);
				}	
			}
			
		}
		delete assignList;
	}
	return returnList;
}

int EvaluateSibling::getOperatorValue (TYPE operater){
	switch(operater){
	case PLUS:
		return 1;
	case MINUS:
		return 2;
	case MULTIPLY:
		return 3;
	}
	return -1;
}
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
	bool returnValue = false;
	
	if(type1 ==PROCEDURE&&type2 ==PROCEDURE){ //1. Sibling("procName1","procName2"
		if(index1!=NULL&&index2!=NULL) returnValue = true;	
	}
	if(Helper::isStatement(type1)&&Helper::isStatement(type2)){//2. Sibling(1,5),Sibling(4,2) for stmt/w/a/if/call
		if((type1==STATEMENT||Helper::isStatementTypeOf(type1,index1))&&(type2==STATEMENT||Helper::isStatementTypeOf(type2,index2))){
			returnValue = (EvaluateFollows::getIsFollowsStarResult(index1,index2)||EvaluateFollows::getIsFollowsStarResult(index2,index1));
		}
	}
	if(type1==VARIABLE&&(type2==STMT_LIST||type2==STATEMENT)){//3. a.Sibling("x",4) for while or if stmt,4 is stmtList
		AST * stmtListNode =NULL;
		AST_LIST * nodeList = pkb->getASTBy(index2);
		for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
			if((**astItr).getRootType()==STMT_LIST){
				stmtListNode = *astItr;
				break;
			}
		} 
		if(stmtListNode!=NULL){
			AST * parentNode = stmtListNode->getAncestor();		//get the parentNode to check if it is while or if
			AST * varNode = parentNode->getFirstDescendant();	//get the varNode of while or if

			if(varNode->getRootType() ==VARIABLE&&varNode->getRootData()==index1)
				returnValue= true;	
		}
	}
	if(type2==VARIABLE&&(type1==STMT_LIST||type1==STATEMENT)){ //3. b.Sibling(4,"x") for while of if stmt
		AST * stmtListNode = NULL;
		AST_LIST * nodeList = pkb->getASTBy(index1);
		for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
			if((**astItr).getRootType()==STMT_LIST){
				stmtListNode = *astItr;
				break;
			}
		} 
		if(stmtListNode!=NULL){
			AST * parentNode = stmtListNode->getAncestor();		//get the parentNode to check if it is while or if
			AST * varNode = parentNode->getFirstDescendant();	//get the varNode of while or if

			if(varNode->getRootType() ==VARIABLE&&varNode->getRootData()==index2)
				returnValue = true;
		}
		
	}
	if((type1==STMT_LIST||type1==STATEMENT)&&(type2==STMT_LIST||type2==STATEMENT)){//3. c. Sibling( 22,23) for stmtList of if
		AST * stmtListNode1 =NULL;
		AST_LIST * nodeList = pkb->getASTBy(index1);
		for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
			if((**astItr).getRootType()==STMT_LIST){
				stmtListNode1 = *astItr;
				break;
			}
		} 
		AST * stmtListNode2=NULL;
		nodeList = pkb->getASTBy(index2);
		for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
			if((**astItr).getRootType()==STMT_LIST){
				stmtListNode2 = *astItr;
				break;
			}
		} 
		if(stmtListNode1!=NULL&&stmtListNode2!=NULL){
			if(stmtListNode1->getAncestor()->getRootData()==stmtListNode2->getAncestor()->getRootData())
				returnValue= true;
		}
	}
	if(Helper::isExprElement(type1)&&Helper::isExprElement(type2)){//4. Sibling ("x"/"*"/"+"/"-","x"/"*"/"+"/"-"). operator data: -1
		DATA_LIST * assignList = pkb->getAllAssigns();
		
		for(DATA_LIST::iterator itr= assignList->begin();itr!=assignList->end();itr++){
			//get assignNode
			AST * assignNode = NULL;
			AST_LIST * nodeList = pkb->getASTBy(*itr);
			for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
				if((**astItr).getRootType()==ASSIGNMENT){
					assignNode = *astItr;
					break;
				}
			} 
			//check if modified var and expression tree root are sibling
			AST * leftVarNode = assignNode->getFirstDescendant();
			AST * expressionRoot = leftVarNode->getRightSibling();
			
			int exprData = expressionRoot->getRootData();
			if(expressionRoot->getRootType()==CONSTANT) exprData = pkb->getConstantValue(exprData);

			//pattern is symmetric, find if x is matched with some elements in x=y+z+...
			bool isPattern1 = (leftVarNode->getRootType()==type1&&leftVarNode->getRootData()==index1)&&(expressionRoot->getRootType()==type2&&exprData==index2);
			bool isPattern2 =(expressionRoot->getRootType()==type1&&exprData==index1)&&(leftVarNode->getRootType()==type2&&leftVarNode->getRootData()==index2);
			if(isPattern1||isPattern2) 
				return true; // x = expression, x is matched with var or +,-,*

			//traverse the tree and check matched pattern, search in left hand side x= y+z...
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

					int leftData = left->getRootData();
					if(left->getRootType()==CONSTANT) leftData = pkb->getConstantValue(leftData);

					int rightData = right->getRootData();
					if(right->getRootType()==CONSTANT) rightData = pkb->getConstantValue(rightData);
					//pattern is symmetric
					isPattern1 = (left->getRootType()==type1&&leftData==index1)&&(right->getRootType()==type2&&rightData==index2);
					isPattern2 =(right->getRootType()==type1&&rightData==index1)&&(left->getRootType()==type2&&leftData==index2);
					if(isPattern1||isPattern2)
						return true;

					//push the next operator into stack
					if(left->getRootType()!=VARIABLE&&left->getRootType()!=CONSTANT) exprStack.push(left);
					if(right->getRootType()!=VARIABLE&&right->getRootType()!=CONSTANT) exprStack.push(right);
				}
			}
			
		}
		delete assignList;
	}
	return returnValue;
}

RELATION_LIST EvaluateSibling::getSiblingResult(TYPE type1,TYPE type2,INDEX index1,INDEX index2){
	if(index1==-2&&index2!=-2){ //sibling(unknown,known)
		return getOneSibling(type2,type1,index2,2);
	}else if(index1!=-2&&index2==-2){ //sibling(known,unknown)
		return getOneSibling(type1,type2,index1,1);
	}else {
		return getSiblingUnknown(type1,type2);
	}
}

RELATION_LIST EvaluateSibling::getOneSibling(TYPE known,TYPE unknown,INDEX knownIndex,int knownPos){
	RELATION_LIST returnList;
	if(known==PROCEDURE&&unknown==PROCEDURE){ //1. sibling ( "procName",p) or sibling ( p,"procName")
		int procTableSize = pkb->getProceTableSize();

		for(int i =1;i<=procTableSize;i++){
			if(knownIndex!=i){
				if(knownPos==1){ //sibling (known,unknown)
					returnList.push_back(pair<int,int>(knownIndex,i));
				}else{ //sibling(unknown,known)
					returnList.push_back(pair<int,int>(i,knownIndex));
				}
			} 
		}
	}
	if(Helper::isStatement(known)&&Helper::isStatement(unknown)){//2. sibling(stmt/a/w/if/call,stmt/a/w/if/call), either field is unknown
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
	}
	if(known==VARIABLE&&unknown==STMT_LIST){//3. a. sibling( "x", stmtList) or sibling(stmtList,"x")
		//sibling("x",stmtList)
		//get all the whiles
		DATA_LIST * whileList = pkb->getAllWhiles();
		for(DATA_LIST::iterator itr=whileList->begin();itr!=whileList->end();itr++){
			if(pkb->getWhileCtrVar(*itr)==knownIndex){
				if(knownPos==1)	
					returnList.push_back(pair<int,int>(knownIndex,*itr+1));
				else
					returnList.push_back(pair<int,int>(*itr+1,knownIndex));
			}
		}
		delete whileList;

		//get all the ifs
		DATA_LIST * ifList = pkb->getAllIfs();
		for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
			AST * ifNode = NULL;
				AST_LIST * nodeList = pkb->getASTBy(*itr);
				for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
					if((**astItr).getRootType()==IF){
						ifNode = *astItr;
						break;
					}
				} 

			AST * ifVarNode = ifNode->getFirstDescendant(); 
			AST * thenNode = ifVarNode->getRightSibling();
			AST * elseNode = thenNode->getRightSibling();

			if(ifVarNode->getRootData()==knownIndex){
				if(knownIndex==1){
					returnList.push_back(pair<int,int>(knownIndex,thenNode->getRootStatementNum()));
					returnList.push_back(pair<int,int>(knownIndex,elseNode->getRootStatementNum()));
				}else{
					returnList.push_back(pair<int,int>(thenNode->getRootStatementNum(),knownIndex));
					returnList.push_back(pair<int,int>(elseNode->getRootStatementNum(),knownIndex));
				}
			}
		}
		delete ifList;
		
	}
	if(known==STMT_LIST&&unknown==VARIABLE){//3.b sibling(x,2) or sibling(2,x) where 2 is stmtList no.
		AST * stmtListNode = NULL;
		AST_LIST * nodeList = pkb->getASTBy(knownIndex);
		for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
			if((**astItr).getRootType()==STMT_LIST){
				stmtListNode = *astItr;
				break;
			}
		} 
		if(stmtListNode!=NULL){
			AST * varNode = stmtListNode->getFirstDescendant();

			if(varNode->getRootType()==VARIABLE){
				if(knownPos==1)
					returnList.push_back(pair<int,int>(knownIndex,varNode->getRootData()));
				else
					returnList.push_back(pair<int,int>(varNode->getRootData(),knownIndex));
			}
		}
	}
	if(known==STMT_LIST&&unknown==STMT_LIST){ //sibling (stmtList,2) or sibling(2,stmtList)
		//get all the ifs
			DATA_LIST * ifList = pkb->getAllIfs();
			for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
				AST * ifNode = NULL;
				AST_LIST * nodeList = pkb->getASTBy(*itr);
				for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
					if((**astItr).getRootType()==IF){
						ifNode = *astItr;
						break;
					}
				} 

				AST * ifVarNode = ifNode->getFirstDescendant();
				AST * thenNode = ifVarNode->getRightSibling();
				AST * elseNode = thenNode->getRightSibling();

				if(knownIndex==elseNode->getRootStatementNum()){
					if(knownPos==1)
						returnList.push_back(pair<int,int>(knownIndex,thenNode->getRootStatementNum()));
					else
						returnList.push_back(pair<int,int>(thenNode->getRootStatementNum(),knownIndex));
				}else if(knownIndex= thenNode->getRootStatementNum()){
					if(knownPos==1)
						returnList.push_back(pair<int,int>(knownIndex,elseNode->getRootStatementNum()));
					else
						returnList.push_back(pair<int,int>(elseNode->getRootStatementNum(),knownIndex));
				}
			}
			delete ifList;
		
		
	}else if(Helper::isExprElement(known)&&Helper::isExprElement(unknown)){//4. Sibling (var/plus/minus/multiply,var/plus/minus/multiply). 
		DATA_LIST * assignList = pkb->getAllAssigns();
		
		for(DATA_LIST::iterator itr= assignList->begin();itr!=assignList->end();itr++){
			//get assignNode
			AST * assignNode = NULL;
			AST_LIST * nodeList = pkb->getASTBy(*itr);
			for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
				if((**astItr).getRootType()==ASSIGNMENT){
					assignNode = *astItr;
					break;
				}
			}

			//check if modified var and expression tree root are sibling
			AST * leftVarNode = assignNode->getFirstDescendant();
			AST * expressionRoot = leftVarNode->getRightSibling();
			
			int exprData = expressionRoot->getRootData();
			if(expressionRoot->getRootType()==CONSTANT) exprData = pkb->getConstantValue(exprData);

			/*pattern is symmetric
				if else pattern check is used to avoid repeated result
				eg. x->x in expression tree, sibling("x",var)/sibling(var,"x")
					if pattern1 is matched, no need to check pattern2
				eg. x->+ in expressoin tree, sibling("+",var)/sibling(var,"+")
					if pattern1 is matched, pattern2 can't be matched
			*/
			bool isPattern1 = (leftVarNode->getRootType()==known&&leftVarNode->getRootData()==knownIndex)&&(expressionRoot->getRootType()==unknown);
			bool isPattern2 =(expressionRoot->getRootType()==known&&exprData==knownIndex)&&(leftVarNode->getRootType()==unknown);
			if(isPattern1){
								
				if(knownPos==1){
					returnList.push_back(pair<int,int>(knownIndex,expressionRoot->getRootData()));
				}else{
					returnList.push_back(pair<int,int>(expressionRoot->getRootData(),knownIndex));
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

					int leftData = left->getRootData();
					if(left->getRootType()==CONSTANT) leftData = pkb->getConstantValue(leftData);

					int rightData = right->getRootData();
					if(right->getRootType()==CONSTANT) rightData = pkb->getConstantValue(rightData);

					//pattern is symmetric
					isPattern1 = (left->getRootType()==known&&leftData==knownIndex)&&(right->getRootType()==unknown);
					isPattern2 =(right->getRootType()==known&&rightData==knownIndex)&&(left->getRootType()==unknown);
					if(isPattern1){
																	
						if(knownPos==1){
							returnList.push_back(pair<int,int>(known,right->getRootData()));
						}else{
							returnList.push_back(pair<int,int>(right->getRootData(),known));
						}
						
					} else if(isPattern2){
		
						if(knownPos==1){
							returnList.push_back(pair<int,int>(knownIndex,left->getRootData()));
						}else{
							returnList.push_back(pair<int,int>(left->getRootData(),knownIndex));
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

		for(int i =1;i<=procTableSize;i++){
			for(int j =1;j<=procTableSize;j++){
				if(i!=j) returnList.push_back(pair<int,int>(i,j));
			}
		}
	}
	if(Helper::isStatement(type1)&&Helper::isStatement(type2)){//2. sibling(stmt/a/w/if/call,stmt/a/w/if/call)
		//get sequential sibling from Follows*
		returnList = EvaluateFollows::getFollowsStarResult(STATEMENT,STATEMENT);

		//insert reversed sibling list
		RELATION_LIST tempList = returnList;
		for(RELATION_LIST::iterator itr=tempList.begin();itr!=tempList.end();itr++){
			returnList.push_back(pair<int,int>(itr->second,itr->first));
		}
	}
	if(type1==VARIABLE&&type2==STMT_LIST){//3. a. sibling( x, stmtList)
		//get all the whiles
		DATA_LIST * whileList = pkb->getAllWhiles();
		for(DATA_LIST::iterator itr=whileList->begin();itr!=whileList->end();itr++){
			returnList.push_back(pair<int,int>(pkb->getWhileCtrVar(*itr),*itr+1)); // No. of stmtList = stmtNo of while+1
		}
		delete whileList;

		//get all the ifs
		DATA_LIST * ifList = pkb->getAllIfs();
		for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
			AST * ifNode = NULL;
			AST_LIST * nodeList = pkb->getASTBy(*itr);
			for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
				if((**astItr).getRootType()==IF){
					ifNode = *astItr;
					break;
				}
			} 

			AST * ifVarNode = ifNode->getFirstDescendant();
			AST * elseNode = ifVarNode->getRightSibling();
			AST * thenNode = elseNode->getRightSibling();

			returnList.push_back(pair<int,int>(ifVarNode->getRootData(),elseNode->getRootStatementNum()));
			returnList.push_back(pair<int,int>(ifVarNode->getRootData(),thenNode->getRootStatementNum()));
		}
		delete ifList;
	}
	if(type1==STMT_LIST&&type2==VARIABLE){ //3.b sibling(stmtList,x)
		//get all the whiles
		DATA_LIST * whileList = pkb->getAllWhiles();
		for(DATA_LIST::iterator itr=whileList->begin();itr!=whileList->end();itr++){
			returnList.push_back(pair<int,int>(*itr+1,pkb->getWhileCtrVar(*itr)));
		}
		delete whileList;

		//get all the ifs
		DATA_LIST * ifList = pkb->getAllIfs();
		for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
			AST * ifNode = NULL;
			AST_LIST * nodeList = pkb->getASTBy(*itr);
			for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
				if((**astItr).getRootType()==IF){
					ifNode = *astItr;
					break;
				}
			} 

			AST * ifVarNode = ifNode->getFirstDescendant();
			AST * thenNode = ifVarNode->getRightSibling();
			AST * elseNode = thenNode->getRightSibling();

			returnList.push_back(pair<int,int>(elseNode->getRootStatementNum(),ifVarNode->getRootData()));
			returnList.push_back(pair<int,int>(thenNode->getRootStatementNum(),ifVarNode->getRootData()));
		}
		delete ifList;
	}
	if(type1==STMT_LIST&&type2==STMT_LIST){ //sibling (stmtList,stmtList),only possible in If stmts
		//get all the ifs
		DATA_LIST * ifList = pkb->getAllIfs();
		for(DATA_LIST::iterator itr=ifList->begin();itr!=ifList->end();itr++){
			AST * ifNode = NULL;
			AST_LIST * nodeList = pkb->getASTBy(*itr);
			for(AST_LIST::iterator astItr=nodeList->begin();astItr!=nodeList->end();astItr++){
				if((**astItr).getRootType()==IF){
					ifNode = *astItr;
					break;
				}
			} 

			AST * ifVarNode = ifNode->getFirstDescendant();
			AST * thenNode = ifVarNode->getRightSibling();
			AST * elseNode = thenNode->getRightSibling();

			//(else,then)
			returnList.push_back(pair<int,int>(elseNode->getRootStatementNum(),thenNode->getRootStatementNum()));
			//(then,else)
			returnList.push_back(pair<int,int>(thenNode->getRootStatementNum(),elseNode->getRootStatementNum()));
		}
		delete ifList;
	}
	if(Helper::isExprElement(type1)&&Helper::isExprElement(type2)){//4. Sibling (var/plus/minus/multiply,var/plus/minus/multiply). 
		DATA_LIST * assignList = pkb->getAllAssigns();
		
		for(DATA_LIST::iterator itr= assignList->begin();itr!=assignList->end();itr++){
			//get assignNode
			AST_LIST * assignNodes = pkb->getASTBy(*itr);
			AST * assignNode;
			for(AST_LIST::iterator astItr=assignNodes->begin();astItr!=assignNodes->end();astItr++){
				if((**astItr).getRootType()==ASSIGNMENT){
					assignNode = *astItr;
					break;
				}
			} 

			//check if modified var and expression tree root are sibling
			AST * leftVarNode = assignNode->getFirstDescendant();
			AST * expressionRoot = leftVarNode->getRightSibling();
			
			//pattern is symmetric
			bool isPattern1 = (leftVarNode->getRootType()==type1)&&(expressionRoot->getRootType()==type2);
			bool isPattern2 =(expressionRoot->getRootType()==type1)&&(leftVarNode->getRootType()==type2);

			//if in the case of x=x,y=y, pattern1 and pattern2 are same
			bool isLeftRightEqual = (leftVarNode->getRootType()==expressionRoot->getRootType())&&(leftVarNode->getRootData()==expressionRoot->getRootData());
			
			if(isPattern1){  //type1 matched with left, type2 matched with right
								
				returnList.push_back(pair<int,int>(leftVarNode->getRootData(),expressionRoot->getRootData()));
			}else if(isPattern2){	//to avoid duplicate result,type1 matched with right, type2 matched with left
			
					returnList.push_back(pair<int,int>(expressionRoot->getRootData(),leftVarNode->getRootData()));
				 // x = expression, x is matched with var or +,-,*
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
							returnList.push_back(pair<int,int>(left->getRootData(),right->getRootData()));
					} else if(isPattern2){
							returnList.push_back(pair<int,int>(right->getRootData(),left->getRootData()));
						 // x = expression, x is matched with var or +,-,*
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
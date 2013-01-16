#include "Pattern.h"
#include <iostream>


Pattern::Pattern()
{

}


Pattern::~Pattern(void)
{
}

void Pattern::setPKB(PKB* p){
	pkb = p;
	
}
//done
bool Pattern::isPattern(TYPE type,VAR_INDEX varIndex,QTREE* astExpression){
	//return true if the returnList is not empty, otherwise return false
	if(type==ASSIGNMENT){
		//cout<<Pattern::isPatternAssign(varIndex,astExpression);
		return Pattern::isPatternAssign(varIndex,astExpression);
	}
	if(type==WHILE){
		return Pattern::isPatternWhile(varIndex);
	}

	//ivalid types return false
	return false;
	
}
//done
STATEMENT_LIST* Pattern::getPatternStmt(TYPE type,VAR_INDEX index,QTREE* astExpression){
	STATEMENT_LIST *returnList;


	//return true if the returnList is not empty, otherwise return false
	if(type==ASSIGNMENT){
		returnList = Pattern::getPatternAssign(index, astExpression);
		//if(returnList ==NULL) return NULL;
		return returnList;
	}
	if(type==WHILE){
		returnList = Pattern::getPatternWhile(index);
	//	if(returnList ==NULL) return NULL;
		return returnList;
	}

	return NULL;
	
}

bool Pattern::isPatternAssign(VAR_INDEX varIndex,QTREE* astExpression){
	STATEMENT_LIST * returnList;
	returnList = new STATEMENT_LIST;
	AST_LIST * treeList;
	AST * rootTree;

	if(varIndex!=NULL){
		MODIFIES_LIST stmtList = pkb->getModifies(ASSIGNMENT,NULL,varIndex);

		if(stmtList.size()<1) return NULL;

		//interator
		MODIFIES_LIST::iterator itr1;  
		for(itr1=stmtList.begin();itr1!=stmtList.end();itr1++){

			int stmtNo = itr1->first;
			treeList = pkb->getASTBy(stmtNo);

			AST_LIST::iterator itr2;
			itr2=treeList->begin();
			bool found = false;

			do{
				if((*itr2)->getRootType()==ASSIGNMENT){
					found=true;
					rootTree = *itr2;
				}
				itr2++;
			}while(itr2!=treeList->end()||!found);
			
			if(Pattern::isSubTree(rootTree,astExpression)){
				
				return true;
			}
		}
		return false;
	}else{ //varIndex ==NULL
		
		//get all variables that are modified
		MODIFIES_LIST* modifiesMap;
		MODIFIES_LIST::iterator itr1;
		modifiesMap = new MODIFIES_LIST(pkb->getModifies(ASSIGNMENT,EMPTY,EMPTY));

		for(itr1=(*modifiesMap).begin();itr1!=(*modifiesMap).end();itr1++){

			int stmtNo = itr1->first;
			treeList = pkb->getASTBy(stmtNo);
		
			AST_LIST::iterator itr3;
			itr3=treeList->begin();
			bool found = false;

			//loop through to get the AST rooted at ASSIGNMENT
			do{
				if((*itr3)->getRootType()==ASSIGNMENT){
					found=true;
					rootTree = *itr3;
				}
				itr3++;
			}while(itr3!=treeList->end()||!found);
			
			//if pattern is matched, add the statement No in the return list
			if(Pattern::isSubTree(rootTree,astExpression)){
				return true;
			}
		}
	}
	return false;
}
//TODO
bool Pattern::isPatternWhile(VAR_INDEX varIndex){
	STATEMENT_LIST * returnList;
	returnList = new STATEMENT_LIST;
	AST_LIST * treeList;

	if(varIndex!=NULL){
		USES_LIST stmtList = pkb->getUses(WHILE,NULL,varIndex);

		if(stmtList.size()<1) return NULL;

		USES_LIST::iterator itr1;
		for(itr1=stmtList.begin();itr1!=stmtList.end();itr1++){

			int stmtNo = itr1->first;
			treeList = pkb->getASTBy(stmtNo);

			AST_LIST::iterator itr2;
			itr2=treeList->begin();
			bool found = false;

			do{
				//get the node with WHILE
				if((*itr2)->getRootType()==WHILE){
					// varIndex is the control var for the WHILE
				    if((*itr2)->getFirstDescendant()->getRootData()== varIndex)
						found = true;
				}
				itr2++;
			}while(itr2!=treeList->end()||!found);
			
			if(found){
				return true;
			}
		}
	}else{//varIndex ==NULL
		//TODO
	
		//get all variables that are modified
		USES_LIST* usesList;
		usesList = new USES_LIST(pkb->getUses(WHILE,EMPTY,EMPTY));
		//cout<<(*usesList).size();
		if((*usesList).size()>0) return true;
	
	}
	return false;
}

STATEMENT_LIST* Pattern::getPatternAssign(VAR_INDEX varIndex,QTREE * astExpression){
	STATEMENT_LIST * returnList;
	returnList = new STATEMENT_LIST;
	AST_LIST * treeList;
	AST * rootTree;

	if(varIndex!=NULL){
		MODIFIES_LIST stmtList = pkb->getModifies(ASSIGNMENT,NULL,varIndex);

	//	cout<<stmtList.size();
		if(stmtList.size()<1) return NULL;

		MODIFIES_LIST::iterator itr1;
		for(itr1=stmtList.begin();itr1!=stmtList.end();itr1++){

			int stmtNo = itr1->first;
			treeList = pkb->getASTBy(stmtNo);

			AST_LIST::iterator itr2;
			itr2=treeList->begin();
			bool found = false;

		//	cout<<"stmtNO: " <<stmtNo;
			//ast is empty
			if(astExpression==NULL){
				returnList->push_back(stmtNo);
			}else{//ast is not "_"
				do{
					if((*itr2)->getRootType()==ASSIGNMENT){
						found=true;
						rootTree = *itr2;
					}
					itr2++;
				}while(itr2!=treeList->end()&&!found);
			
				if(Pattern::isSubTree(rootTree,astExpression)){
					returnList->push_back(stmtNo);
				}
			}
		}

		if((*returnList).size()>0){
			(*returnList).sort();
			(*returnList).unique();
			return returnList;
		}
	}else{ //varIndex ==NULL
		
		//get all variables that are modified
		MODIFIES_LIST* modifiesList;
		MODIFIES_LIST::iterator itr1;
		modifiesList = new MODIFIES_LIST(pkb->getModifies(ASSIGNMENT,EMPTY,EMPTY));

		if((*modifiesList).size()<1) return NULL;

		for(itr1=(*modifiesList).begin();itr1!=(*modifiesList).end();itr1++){
			
			int stmtNo = itr1->first;
			//get the AST list from pkb
			treeList = pkb->getASTBy(stmtNo);
		
			AST_LIST::iterator itr3;
			itr3=treeList->begin();
			bool found = false;

			//expression tree is empty
			if(astExpression ==NULL){
				returnList->push_back(stmtNo);
			}else{//loop through to get the AST rooted at ASSIGNMENT
				do{
					if((*itr3)->getRootType()==ASSIGNMENT){
						found=true;
						rootTree = *itr3;
					}
					itr3++;
				}while(itr3!=treeList->end()&&!found);
			
				//if pattern is matched, add the statement No in the return list
				if(Pattern::isSubTree(rootTree,astExpression)){
					returnList->push_back(stmtNo);
				}
			}
		}
		if((*returnList).size()>0){
			(*returnList).sort();
			(*returnList).unique();
			return returnList;
		}
	}
	return NULL;

}

STATEMENT_LIST* Pattern::getPatternWhile(VAR_INDEX varIndex){
	STATEMENT_LIST * returnList;
	returnList = new STATEMENT_LIST;
	AST_LIST * treeList;

	if(varIndex!=NULL){
		USES_LIST stmtList = pkb->getUses(WHILE,NULL,varIndex);

		if(stmtList.size()<1) return NULL;

		USES_LIST::iterator itr1;
		for(itr1=stmtList.begin();itr1!=stmtList.end();itr1++){

			int stmtNo = itr1->first;
			treeList = pkb->getASTBy(stmtNo);
		//	cout<<"stmtNo: "<<stmtNo;
			AST_LIST::iterator itr2;
			itr2 = treeList->begin();
			bool found = false;

			do{
				//get the node with WHILE
				if((*itr2)->getRootType()==WHILE){
				    if((*itr2)->getFirstDescendant()->getRootData()== varIndex)
						found = true;
				}
				itr2++;
			}while(itr2!=treeList->end()&&!found);
			
			if(found){
				returnList->push_back(itr1->first);
			}
		}
		if((*returnList).size()>0){
			(*returnList).sort();
			(*returnList).unique();
			return returnList;
		}
	}else{//varIndex ==NULL
		//TODO
	
		//get all variables that are used
		USES_LIST* usesList;
		USES_LIST::iterator itr1;
		usesList = new USES_LIST(pkb->getUses(WHILE,EMPTY,EMPTY));
		
		if((*usesList).size()<1) return NULL;

		for(itr1=(*usesList).begin();itr1!=(*usesList).end();itr1++){
	
			returnList->push_back(itr1->first);
		}

		if((*returnList).size()>0){
			(*returnList).sort();
			(*returnList).unique();
			return returnList;
		}
	}
	return NULL;
}
//done
bool Pattern::isSubTree(AST* rootTree, QTREE* subTree){

	AST* current1;
	QTREE* current2;
	//both tree start with assign, check if the whole tree is matched
	if(rootTree->getRootType()==subTree->getType()){
		current1 = rootTree->getFirstDescendant()->getRightSibling();
		current2 = subTree->getFirstDescendant()->getRightSibling();

	//	cout<<"rootType"<<rootTree->getRootType();
	//	cout<<"root Data"<<rootTree->getRootData();
   	//	cout<<"sub Type"<<subTree->getType();
	//	cout<<"sub Data"<<subTree->getData();

		// here
		if((current1->getRootType()==current2->getType())&&(current1->getRootData()==current2->getData())){
			return true;
		}

	}else{
		current1 = rootTree;
		current2 = subTree;
		//cout<< rootTree->getRootStatementNum();
		//cout<< subTree->getRootData();
		//cout<< pkb->isUses(ASSIGNMENT,rootTree->getRootStatementNum(),subTree->getRootData());
		if(current2->getType()!=CONSTANT){
			return pkb->isUses(ASSIGNMENT,rootTree->getRootStatementNum(),subTree->getData());
		}else{//need to refactor
			int stmtNo = current1->getRootStatementNum();
			AST_LIST* treeList;
			treeList = pkb->getASTBy(stmtNo);
			AST_LIST::iterator itr;

			for(itr=treeList->begin();itr!=treeList->end();itr++){
				if((current2->getType()==(*itr)->getRootType())&&(current2->getData()==(*itr)->getRootData())){
					return true;
				}
			}
			return false;
		}
	}

	
		return false;
}
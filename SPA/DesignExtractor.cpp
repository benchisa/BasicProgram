#include "DesignExtractor.h"


DesignExtractor::DesignExtractor(PKB* pkb)
{
	this->pkb = pkb;
}


DesignExtractor::~DesignExtractor(void)
{
}

//calltable

CALL_LIST DesignExtractor::getCallResult(PROC_NAME caller, PROC_NAME callee)
{
	return pkb->getCall(caller, callee);
}
bool DesignExtractor::getIsCallResult(PROC_NAME caller, PROC_NAME callee)
{
	return pkb->isExistsCall(caller, callee);
}

CALL_LIST  DesignExtractor::getCallStarResult(PROC_NAME caller, PROC_NAME callee)
{
	list<pair<string,string>> finalResult;
	list<string> firstResult;
	DesignExtractor::computeCallStar(caller, callee, firstResult);
	if (firstResult.size()>0)
	{
		list<string>::iterator itr;
		for (itr=firstResult.begin(); itr!=firstResult.end(); itr++)
		{
			if (caller!=" " && callee==" ")
			{

				finalResult.push_back(make_pair(caller, *itr));
			}
			else if (callee!=" " && caller==" ")
			{

				finalResult.push_back(make_pair(*itr, callee));
			}
		}
	}
	return finalResult;

}

bool DesignExtractor::getIsCallStarResult(PROC_NAME caller, PROC_NAME callee)
{
	if (caller!=" " && callee!=" ")
	{
		CALL_LIST result=DesignExtractor::getCallStarResult(caller," ");
		CALL_LIST::iterator itr;
		if (!result.empty())
		{
			for (itr=result.begin(); itr!=result.end(); itr++)
			{
				if (caller==itr->first && callee==itr->second)
				{
					return true;
				}
			}
		}

	}
	return false;
}


bool DesignExtractor::isStatementTypeOf(TYPE typeName,STATEMENT_NUM stmtNum){
	//find the type of statement
	AST_LIST* currentAST = pkb->getASTBy(stmtNum);
	AST_LIST::iterator itr;
	for(itr = currentAST->begin();itr!=currentAST->end();itr++){
		if((*itr)->getRootType()==typeName) return true;
	}
	return false;
}

void DesignExtractor::createCFG()
{
	
	int size = pkb->getMaxProgLine();

	// create CFG of progline_size
	pkb->createCFG(size);

	// traverse AST and create the CFG
	AST* cAST = pkb->getRootAST();
	AST* tmp = cAST;
	buildCFG(tmp);
}

AST* DesignExtractor::buildCFG(AST * node)
{
	AST* tmp = node;
	AST* containerTmp, *lastNode;

	while(tmp)
	{
		switch(pkb->getType(tmp)){
		
		case ASSIGNMENT:
			if(tmp->getRightSibling()){
				pkb->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}
			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		
		case WHILE:
			cfgWhileKeepers.push_back(pkb->getProgLine(tmp));

			if(tmp->getRightSibling()){
				pkb->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}
			
			pkb->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getFirstDescendant()));

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling());
			if(pkb->getType(lastNode) != IF)
				pkb->addEdge(pkb->getProgLine(lastNode), cfgWhileKeepers.back());

			cfgWhileKeepers.pop_back();
			tmp = tmp->getRightSibling();
			break;
		case IF:

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling());
			if(cfgWhileKeepers.size()!= 0)
				pkb->addEdge(pkb->getProgLine(lastNode), cfgWhileKeepers.back());

			if(tmp->getRightSibling()){
				pkb->addEdge(pkb->getProgLine(lastNode), pkb->getProgLine(tmp->getRightSibling()));
			}

			pkb->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getFirstDescendant()));

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling()->getRightSibling());
			if(cfgWhileKeepers.size() != 0)
				pkb->addEdge(pkb->getProgLine(lastNode), cfgWhileKeepers.back());
			
			
			if(tmp->getRightSibling()){
				pkb->addEdge(pkb->getProgLine(lastNode), pkb->getProgLine(tmp->getRightSibling()));
			}

			pkb->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()));
			
			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		case CALL:
			if(tmp->getRightSibling()){
				pkb->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}

			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		case PROCEDURE:
			lastNode = buildCFG(tmp->getFirstDescendant());
			tmp = tmp->getRightSibling();
			break;
		case STMT_LIST:
			tmp = tmp->getFirstDescendant();
			break;
		}
	}

	return lastNode;
}

bool DesignExtractor::isNextResult(PROG_LINE p1, PROG_LINE p2)
{
	return pkb->isNext(p1, p2);
}

// already stored in CFG
NEXT_LIST DesignExtractor::getNextResult(PROG_LINE p1, PROG_LINE p2)
{
	return pkb->getNext(p1, p2);
}

// on demand
bool DesignExtractor::isNextStarResult(PROG_LINE p1, PROG_LINE p2)
{
	int size = pkb->getMaxProgLine();

	if(p1 > size || p2 > size || p1 <= 0 || p2 <=0)
		return false;

	list<PROG_LINE> tmp = pkb->getAllProgLines(p1, p2);
	
	if(tmp.size() != 0){
		list<PROG_LINE>::iterator itr = tmp.begin();
		while(itr!=tmp.end()){
			if(*itr==p2)
				return true;

			itr++;
		}
	}
	return false;
}


// on demand
NEXT_LIST DesignExtractor::getNextStarResult(PROG_LINE p1, PROG_LINE p2)
{
	NEXT_LIST result;
	int size = pkb->getMaxProgLine();

	// Next*(n1, n2) --> findAll
	if(p1 == 0 && p2 == 0){
		for(int i = 1; i <=size; i++){
			list<PROG_LINE> tmp = pkb->getAllProgLines(i, i);

			list<PROG_LINE>::iterator itr = tmp.begin();
			while(itr!=tmp.end())
			{
				pair<PROG_LINE, PROG_LINE> tPair;
				tPair.first = i;
				tPair.second = *itr;
				result.push_back(tPair);
				itr++;
			}
		}
	}
	//Next*(n, n), Next*(1,n1), Next*(n1,2) --> findAll
	else if(p1 == p2 || (p1 != 0 && p2 == 0) || (p1 == 0 && p2 != 0) || (p1 != 0 && p2 != 0)){
		list<PROG_LINE> tmp = pkb->getAllProgLines(p1, p2);
		list<PROG_LINE>::iterator itr = tmp.begin();
		while(itr!=tmp.end())
		{
			pair<PROG_LINE, PROG_LINE> tPair;
			if(p1 != 0) {
				tPair.first = p1;
				tPair.second = *itr;
			}
			else if(p1 == 0) {
				tPair.first = *itr;
				tPair.second = p2;
			}
			result.push_back(tPair);
			itr++;
		}
	}
	return result;
}

FOLLOWS_LIST DesignExtractor::getFollowsResult(TYPE type1, TYPE type2){

	FOLLOWS_LIST result, followLst;
	
	followLst = pkb->getFollows(0, 0);
	DesignExtractor::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST DesignExtractor::getFollowsStarResult(TYPE type1, TYPE type2){
	FOLLOWS_LIST result, followLst, tmpLst;
	FOLLOWS_LIST::iterator followItr;
	
	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = procLst->back().getEndProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = DesignExtractor::getFollowsStar(i, 0);

		if(!tmpLst.empty()){
			followItr = tmpLst.begin();
			while(followItr !=  tmpLst.end()){
				followLst.push_back(*followItr);
				followItr++;
			}
		}
	}
	
	DesignExtractor::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST DesignExtractor::getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;
	
	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getFollows(s1, s2);
		DesignExtractor::computeFollows(result, tmp, type, s1, s2);
	}

	return result;
}

FOLLOWS_LIST DesignExtractor::getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;
	

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = DesignExtractor::getFollowsStar(s1, s2);
		DesignExtractor::computeFollows(result, tmp, type, s1, s2);
	}
	return result;
}

bool DesignExtractor::getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if(s1 != 0 && s2 != 0 &&pkb->isFollows(s1, s2) ){
		return true;
	}

	return false;
}

bool DesignExtractor::getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if (s1!=NULL && s2!=NULL)
	{
		FOLLOWS_LIST result=DesignExtractor::getFollowsStar(s1,NULL);
		FOLLOWS_LIST::iterator itr;
		if (!result.empty())
		{
			for (itr=result.begin(); itr!=result.end(); itr++)
			{
				if (s1==itr->first && s2==itr->second)
				{
					return true;
				}
			}
		}

	}
	return false;
}



PARENT_LIST DesignExtractor::getParentResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst;
	
	parentLst = pkb->getParent(0, 0);
	DesignExtractor::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST DesignExtractor::getParentStarResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst, tmpLst;
	PARENT_LIST::iterator parentItr;
	AST_LIST::iterator astItr_t1, astItr_t2;

	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = procLst->back().getEndProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = DesignExtractor::getParentStar(i, 0);

		if(!tmpLst.empty()){
			parentItr = tmpLst.begin();
			while(parentItr !=  tmpLst.end()){
				parentLst.push_back(*parentItr);
				parentItr++;
			}
		}
	}
	DesignExtractor::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST DesignExtractor::getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;
	
	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getParent(s1, s2);
		DesignExtractor::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

PARENT_LIST DesignExtractor::getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = DesignExtractor::getParentStar(s1, s2);
		DesignExtractor::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

bool DesignExtractor::getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2){	
	if(s1 != 0 && s2 != 0 && pkb->isParent(s1, s2))
		return true;
	else
		return false;
}

bool DesignExtractor::getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2)
{
	if (s1!=NULL && s2!=NULL)
	{
		PARENT_LIST result=DesignExtractor::getParentStar(s1,NULL);
		PARENT_LIST::iterator itr;
		if (!result.empty())
		{
			for (itr=result.begin(); itr!=result.end(); itr++)
			{
				if (s1==itr->first && s2==itr->second)
				{
					return true;
				}
			}
		}
	}
	return false;
}

MODIFIES_LIST DesignExtractor::getModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	MODIFIES_LIST result,tmpLst;
	MODIFIES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;
	//cout << "TYPE: " << type << "\n";

	if((type == WHILE  || type == ASSIGNMENT||type == IF)){
		//cout << "v1: " << pkb->getVarName(v1) << "\n";
			result = pkb->getModifies(type, arg1, v1);

	}
	// Modifies(_, "x"), Modifies(1, "x")
	else if((type == STATEMENT && arg1 != 0 && v1 == 0
		|| type == ANY && arg1 != 0 && v1 == 0)){
		// Find the type first
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();
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
	else if(type == ANY || (type == STATEMENT && arg1 == 0)){
		// Modifies(_,_) or Modifies(_, "x")
		tmpLst = pkb->getModifies(WHILE, 0, 0);
		iterateAndStore(result, tmpLst, v1);

		//need to use function getModifies(CALL,....)
/*		if(tmpLst.size() == 0){
			tmpLst = pkb->getModifies(PROCEDURE, 0, 0);
//			iterateAndStore(result, tmpLst, v1);
		}*/

		if(result.size() == 0){
			tmpLst = pkb->getModifies(ASSIGNMENT, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}


		if(result.size() == 0){
			tmpLst = pkb->getModifies(IF, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}

	}


	//cout << "SIZE OF RESULT (MODIFIES): " << result.size() << "\n";
	
	return result;
}

bool DesignExtractor::getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
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
	else if(type == PROCEDURE){ //need to be changed
		result = pkb->isModifies(type, arg1, v1);
	}
	//need to consider call

	return result;
}

USES_LIST DesignExtractor::getUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	USES_LIST result,tmpLst;
	USES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;

	//Uses(w, "b")
	if((type == WHILE  || type == ASSIGNMENT || type == IF)){
		result = pkb->getUses(type, arg1, v1);
	}
	// Uses(1, a)
	else if((type == STATEMENT && arg1 != 0 && v1 == 0) || (type == ANY && arg1 != 0 && v1 == 0)){
		// Find the type first
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();
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
	else if(type == ANY || (type == STATEMENT && arg1 == 0))
	{
		tmpLst = pkb->getUses(WHILE, 0, 0);
		iterateAndStore(result, tmpLst, v1);
		
		if(result.size() == 0){
			tmpLst = pkb->getUses(IF, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}
/* @Zhang Xi: I think no need to check proc here, but need to check CALL
		if(tmpLst.size() == 0){
			tmpLst = pkb->getUses(PROCEDURE, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}
*/
		if(result.size() == 0){		
			tmpLst = pkb->getUses(ASSIGNMENT, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}
	}

	//cout << "SIZE OF RESULT (USES): " << result.size() << "\n";

	return result;
}

bool DesignExtractor::getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	USES_LIST usesLst;
	bool result = false;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;

	if(type == STATEMENT && arg1 != 0 && v1 != 0){
		// Find the type of first argument
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();
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
	else if(type == PROCEDURE) //need to be changed and need to add call
	{
		result = pkb->isUses(type, arg1, v1);
	}
	return result;
}

MODIFIES_LIST computeCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	MODIFIES_LIST resultList;

	if(callStmt==0&&varIndex==-1){
		
	}else if(callStmt!=0&&varIndex==-1){
	
	}else if(callStmt==0&&varIndex!=-1){
	
	}else{
	
	}
	return resultList;
}
bool isCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	if(callStmt==0&&varIndex==-1){
	
	}else if(callStmt!=0&&varIndex==-1){
	
	}else if(callStmt==0&&varIndex!=-1){
	
	}else{
	
	}

	return true;
}
USES_LIST computeCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	USES_LIST resultList;

	if(callStmt==0&&varIndex==-1){
	
	}else if(callStmt!=0&&varIndex==-1){
	
	}else if(callStmt==0&&varIndex!=-1){
	
	}else{
	
	}
	return resultList;
}
bool isCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	if(callStmt==0&&varIndex==-1){
	
	}else if(callStmt!=0&&varIndex==-1){
	
	}else if(callStmt==0&&varIndex!=-1){
	
	}else{
	
	}
	return true;
}
//above done with testing//

void DesignExtractor::computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result)
{
	if (stmt1!=NULL || stmt2!=NULL)
	{
		list<pair<int,int>> parentChild=pkb->getParent(stmt1, stmt2);
		if (parentChild.size()>0)
		{
			list<pair<int,int>>::iterator itr;
			for (itr=parentChild.begin(); itr!=parentChild.end(); itr++)
			{	

				if (stmt1!=0 && stmt2==0)

				{	
					result.push_back(itr->second);
					DesignExtractor::computeParentStar(itr->second, 0,result);
				}
				else if (stmt1==0 && stmt2!=0)
				{	
					result.push_back(itr->first);
					DesignExtractor::computeParentStar(0,itr->first,result);
				}
			}
		}

	}
}

FOLLOWS_LIST DesignExtractor::getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult;
	DesignExtractor::computeParentStar(stmt1, stmt2, firstResult);

	if (firstResult.size()>0)
	{   

		list<int>::iterator itr;
		for (itr=firstResult.begin(); itr!=firstResult.end(); itr++)
		{
			if (stmt1!=NULL && stmt2==NULL)
			{

				finalResult.push_back(make_pair(stmt1, *itr));
			}
			else if (stmt2!=NULL && stmt1==NULL)
			{

				finalResult.push_back(make_pair(*itr, stmt2));
			}
		}

	}
	return finalResult;

}

void  DesignExtractor::computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2,list<int> &result)
{
	if (stmt1!=NULL || stmt2!=NULL)
	{
		list<pair<int,int>> follows=pkb->getFollows(stmt1, stmt2);
		if (follows.size()>0)
		{
			list<pair<int, int>>::iterator itr;

			for (itr=follows.begin(); itr!=follows.end(); itr++)
			{

				if (stmt1!=NULL && stmt2==NULL)
				{	
					result.push_back(itr->second);
					DesignExtractor::computeFollowsStar(itr->second, 0,result);
				}
				else if (stmt1==NULL && stmt2!=NULL)
				{
					result.push_back(itr->first);
					DesignExtractor::computeFollowsStar(0,itr->first,result);
				}
			}


		}
	}
}

FOLLOWS_LIST DesignExtractor::getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult;
	DesignExtractor::computeFollowsStar(stmt1, stmt2, firstResult);
	if (firstResult.size()>0)
	{
		list<int>::iterator itr;
		for (itr=firstResult.begin(); itr!=firstResult.end(); itr++)
		{
			if (stmt1!=NULL && stmt2==NULL)
			{

				finalResult.push_back(make_pair(stmt1, *itr));
			}
			else if (stmt2!=NULL && stmt1==NULL)
			{

				finalResult.push_back(make_pair(*itr, stmt2));
			}
		}
	}
	return finalResult;
}

// Computational Methods
void DesignExtractor::computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &tmpLst, TYPE type1, TYPE type2){
		PARENT_LIST::iterator tmpItr;
		AST_LIST *astLst_t1, *astLst_t2;
		AST_LIST::iterator astItr_t1, astItr_t2;
		
		// Parent(s1, s2)/ Parent(_, _)/Parent(ANY, s2)/Parent(s1, ANY)
		if((type1 == STATEMENT && type2 == STATEMENT ) || (type1== ANY && type2 == ANY)
			|| (type1 == STATEMENT && type2 == ANY) || (type1 == ANY && type2==STATEMENT)){
				result = tmpLst;
		}

		// Parent(_, type2), Parent(type1, _)
		// Parent(STATEMENT, type2)
		else if(type1 == ANY || type2 == ANY 
			|| type1 == STATEMENT || type2 == STATEMENT){

				tmpItr = tmpLst.begin();
				while(tmpItr != tmpLst.end()){

					// Decide which statement to check with type
					if(type1 == ANY || type1 == STATEMENT) 
						astLst_t1 = pkb->getASTBy(tmpItr->second);
					else if(type2 == ANY || type2 == STATEMENT) 
						astLst_t1 = pkb->getASTBy(tmpItr->first);

					astItr_t1 = astLst_t1->begin();
					while(astItr_t1 != astLst_t1->end())
					{
						if(pkb->getType(*astItr_t1) == type2 || pkb->getType(*astItr_t1) == type1)
							result.push_back(*tmpItr);

						astItr_t1++;
					}

					tmpItr++;
				}
		}

		else if(type1 == WHILE || type1 == ASSIGNMENT || type1 == IF){

			tmpItr = tmpLst.begin();
			while(tmpItr != tmpLst.end()){
				astLst_t1 = pkb->getASTBy(tmpItr->first);
				astItr_t1 = astLst_t1->begin();
				while(astItr_t1 != astLst_t1->end()){
					if(pkb->getType(*astItr_t1) == type1){
						astLst_t2 = pkb->getASTBy(tmpItr->second);
						astItr_t2 = astLst_t2->begin();
						while(astItr_t2 != astLst_t2->end()){
							if(pkb->getType(*astItr_t2) == type2)
							{
								// push this guy to our result
								result.push_back(*tmpItr);
							}
							astItr_t2++;
						}
					}
					astItr_t1++;
				}
				tmpItr++;
			}
		}
	}
	
void DesignExtractor::computeParent(PARENT_LIST &result,PARENT_LIST tmpLst, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2)
	{
		PARENT_LIST::iterator tmpItr;
		AST_LIST *astLst;
		AST_LIST::iterator astItr;

		if(!tmpLst.empty()){
			// Check TYPE for WHILE or ASSIGNMENT
			if(type == WHILE || type == ASSIGNMENT || type == IF){
				tmpItr = tmpLst.begin(); // should have only 1 pair
				// Deciding which is empty

				if(s1 == 0) astLst = pkb->getASTBy(tmpItr->first);
				else astLst = pkb->getASTBy(tmpItr->second);
				
				//cout << tmp << "\n";
				if(!astLst->empty())
				{
					astItr = astLst->begin();

					while(astItr!=astLst->end()){
						if(pkb->getType(*astItr) == type){
							result = tmpLst;
							//result = tmp;
							break; // does not need to go through all the nodes.
						}

						astItr++;
					}
				}
			}
			//Parent(_, 2) or Parent(s1, 2) is the same.
			else if(type == STATEMENT || type == ANY)
			{
				// just want the pair of statement.
							
				result = tmpLst;
			}
		}
	}

void DesignExtractor::computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
		FOLLOWS_LIST::iterator followItr;
		AST_LIST *astLst;
		AST_LIST::iterator astItr;
		if(!tmp.empty()){

			// Check TYPE for WHILE or ASSIGNMENT
			if(type == WHILE || type == ASSIGNMENT || type == IF){
				followItr = tmp.begin(); 
				while (followItr!=tmp.end())
				{
					if(s1 == 0) astLst = pkb->getASTBy(followItr->first);
					else astLst = pkb->getASTBy(followItr->second);
				
					astItr = astLst->begin();
					while(astItr!=astLst->end())
					{	
						if(pkb->getType(*astItr) == type){		
							result.push_back(*followItr);
							 
						}
						astItr++;
					}
					followItr++;
				}

			}
			else if(type == STATEMENT || type == ANY)
			{
				result = tmp;
			}
		}
	}

	//This function does not work properly
void DesignExtractor::iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmpLst, int v1){
		list<pair<int,int>>::iterator itr;
		if(!tmpLst.empty())
		{
			itr = tmpLst.begin();

			while(itr != tmpLst.end()){
				if(v1 != 0 && itr->second == v1){
					result.push_back(*itr);
					break;
				}
				else if(v1 == 0){
					result.push_back(*itr);
				}
				itr++;
			}	
		}
	}
/*
void DesignExtractor::iterateAndStore(RELATION_LIST *result, RELATION_LIST list){
	if(list.empty()){
			result=NULL;
		}else{
			    RELATION_LIST::iterator itr = list.begin();
				while(itr!=list.end()){
					result->push_back(*itr);
					itr++;
				}	
		}
	
}
	*/
void  DesignExtractor::computeCallStar(PROC_NAME caller, PROC_NAME callee,list<string> &result)
{
	if (caller!=" " || callee!=" ")
	{
		CALL_LIST calls=pkb->getCall(caller, callee);
		if (calls.size()>0)
		{
			CALL_LIST::iterator itr;

			for (itr=calls.begin(); itr!=calls.end(); itr++)
			{

				if (caller!=" " && callee==" ")
				{	
					result.push_back(itr->second);
					DesignExtractor::computeCallStar(itr->second, " ",result);
				}
				else if (caller==" " && callee!=" ")
				{
					result.push_back(itr->first);
					DesignExtractor::computeCallStar(" ",itr->first,result);
				}
			}


		}
	}
}

DATA_LIST * DesignExtractor::getAllAssigns(){
	DATA_LIST * returnList;
	returnList = new DATA_LIST;

	int stmtNo = pkb->getMaxStatementNum();
	for(int i=1; i<=stmtNo;i++){
		AST_LIST * stmtTree;
		stmtTree = pkb->getASTBy(i);
		AST_LIST::iterator itr;

		for(itr=stmtTree->begin();itr!=stmtTree->end();itr++){
			if((*itr)->getRootType()==ASSIGNMENT){
				returnList->push_back(i);
			} 
		}
	}

	return returnList;
}
DATA_LIST * DesignExtractor::getAllConstants(){
	DATA_LIST * returnList = new DATA_LIST();

	int size = pkb->getConstantTableSize();
	for(int i =1;i<=size;i++){
		returnList->push_back(pkb->getConstantValue(i);
	}
	return returnList;
}
DATA_LIST * DesignExtractor::getAllWhiles(){
	DATA_LIST * returnList;
	returnList = new DATA_LIST;

	int stmtNo = pkb->getMaxStatementNum();
	for(int i=1; i<=stmtNo;i++){
		AST_LIST * stmtTree;
		stmtTree = pkb->getASTBy(i);
		AST_LIST::iterator itr;

		for(itr=stmtTree->begin();itr!=stmtTree->end();itr++){
			if((*itr)->getRootType()==WHILE){
				returnList->push_back(i);
			} 
		}
	}

	return returnList;
}
DATA_LIST * DesignExtractor::getAllIfs(){
	DATA_LIST * returnList;
	returnList = new DATA_LIST;

	int stmtNo = pkb->getMaxStatementNum();
	for(int i=1; i<=stmtNo;i++){
		AST_LIST * stmtTree;
		stmtTree = pkb->getASTBy(i);
		AST_LIST::iterator itr;

		for(itr=stmtTree->begin();itr!=stmtTree->end();itr++){
			if((*itr)->getRootType()==IF){
				returnList->push_back(i);
			} 
		}
	}

	return returnList;
}
DATA_LIST * DesignExtractor::getAllCallStmts(){
	DATA_LIST * returnList;
	returnList = new DATA_LIST;

	int stmtNo = pkb->getMaxStatementNum();
	for(int i=1; i<=stmtNo;i++){
		AST_LIST * stmtTree;
		stmtTree = pkb->getASTBy(i);
		AST_LIST::iterator itr;

		for(itr=stmtTree->begin();itr!=stmtTree->end();itr++){
			if((*itr)->getRootType()==CALL){
				returnList->push_back(i);
			} 
		}
	}

	return returnList;
}
DATA_LIST * DesignExtractor::getStmtListOf(TYPE nodeType){
	DATA_LIST * returnList = new DATA_LIST();

	int maxStmtNum = pkb->getMaxStatementNum();
	int maxProgLineNum = pkb->getMaxProgLine();

	switch(nodeType){
	case PROGLINE:
		for(int i=1; i<=maxProgLineNum;i++){
			returnList->push_back(i);
		}
		break;
	case STATEMENT:
		for(int i=1; i<=maxStmtNum;i++){
			returnList->push_back(i);
		}
		break;
	case ASSIGNMENT:
		returnList = DesignExtractor::getAllAssigns();
		break;
	case WHILE:
		returnList = DesignExtractor::getAllWhiles();
		break;
	case IF:
		returnList = DesignExtractor::getAllIfs();
		break;
	case CALL:
		returnList = DesignExtractor::getAllCallStmts();
		break;
	case VARIABLE:
		{
			int varSize = pkb->getVarTableSize();
			for(int i =1;i<=varSize;i++){
				returnList->push_back(i);
			}
		}
		break;
	case PROCEDURE:
		{
			int procSize = pkb->getProceTableSize();
			for(int i =1;i<=procSize;i++){
				returnList->push_back(i);
			}
		}
		break;
	
	case CONSTANT:
		{
			returnList = DesignExtractor::getAllConstants();
		}
		break;
	}
	return returnList;
}

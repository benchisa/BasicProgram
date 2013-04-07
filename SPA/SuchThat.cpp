#include "SuchThat.h"


FOLLOWS_LIST SuchThat::getFollowsResult(TYPE type1, TYPE type2){

	FOLLOWS_LIST result, followLst;
	
	followLst = pkb->getFollows(0, 0);
	SuchThat::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST SuchThat::getFollowsStarResult(TYPE type1, TYPE type2){
	FOLLOWS_LIST result, followLst, tmpLst;
	FOLLOWS_LIST::iterator followItr;
	
	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = procLst->back().getEndProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = SuchThat::getFollowsStar(i, 0);

		if(!tmpLst.empty()){
			followItr = tmpLst.begin();
			while(followItr !=  tmpLst.end()){
				followLst.push_back(*followItr);
				followItr++;
			}
		}
	}
	
	SuchThat::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST SuchThat::getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;
	
	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getFollows(s1, s2);
		SuchThat::computeFollows(result, tmp, type, s1, s2);
	}

	return result;
}

FOLLOWS_LIST SuchThat::getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;
	

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = SuchThat::getFollowsStar(s1, s2);
		SuchThat::computeFollows(result, tmp, type, s1, s2);
	}
	return result;
}

bool SuchThat::getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if(s1 != 0 && s2 != 0 &&pkb->isFollows(s1, s2) ){
		return true;
	}

	return false;
}

bool SuchThat::getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if (s1!=NULL && s2!=NULL)
	{
		FOLLOWS_LIST result=SuchThat::getFollowsStar(s1,s2);
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



PARENT_LIST SuchThat::getParentResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst;
	
	parentLst = pkb->getParent(0, 0);
	SuchThat::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST SuchThat::getParentStarResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst, tmpLst;
	PARENT_LIST::iterator parentItr;
	AST_LIST::iterator astItr_t1, astItr_t2;

	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = procLst->back().getEndProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = SuchThat::getParentStar(i, 0);

		if(!tmpLst.empty()){
			parentItr = tmpLst.begin();
			while(parentItr !=  tmpLst.end()){
				parentLst.push_back(*parentItr);
				parentItr++;
			}
		}
	}
	SuchThat::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST SuchThat::getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;
	
	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getParent(s1, s2);
		SuchThat::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

PARENT_LIST SuchThat::getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = SuchThat::getParentStar(s1, s2);
		SuchThat::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

bool SuchThat::getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2){	
	if(s1 != 0 && s2 != 0 && pkb->isParent(s1, s2))
		return true;
	else
		return false;
}

bool SuchThat::getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2)
{
	if (s1!=NULL && s2!=NULL)
	{
		PARENT_LIST result=SuchThat::getParentStar(s1,s2);
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

MODIFIES_LIST SuchThat::getModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	MODIFIES_LIST result, tmpLst;
	MODIFIES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;
	//cout << "TYPE: " << type << "\n";

	if((type == WHILE || type == PROCEDURE || type == ASSIGNMENT||type == IF)){
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

		if(tmpLst.size() == 0){
			tmpLst = pkb->getModifies(PROCEDURE, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}

		if(tmpLst.size() == 0){
			tmpLst = pkb->getModifies(ASSIGNMENT, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}


		if(tmpLst.size() == 0){
			tmpLst = pkb->getModifies(IF, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}
	}


	//cout << "SIZEX OF RESULT (MODIFIES): " << result.size() << "\n";
	
	return result;
}

bool SuchThat::getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
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
	else if(type == PROCEDURE){
		result = pkb->isModifies(type, arg1, v1);
	}

	return result;
}

USES_LIST SuchThat::getUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	USES_LIST result, tmpLst;
	USES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;

	//Uses(w, "b")
	if((type == WHILE || type == PROCEDURE || type == ASSIGNMENT || type == IF)){
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
		
		if(tmpLst.size() == 0){
			tmpLst = pkb->getUses(IF, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}

		if(tmpLst.size() == 0){
			tmpLst = pkb->getUses(PROCEDURE, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}

		if(tmpLst.size() == 0){		
			tmpLst = pkb->getUses(ASSIGNMENT, 0, 0);
			iterateAndStore(result, tmpLst, v1);
		}
	}

	//cout << "SIZEX OF RESULT (USES): " << result.size() << "\n";
	return result;
}

bool SuchThat::getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1){
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
	else if(type == PROCEDURE)
	{
		result = pkb->isUses(type, arg1, v1);
	}
	return result;
}

//above done with testing//

void SuchThat::computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result)
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
					SuchThat::computeParentStar(itr->second, 0,result);
				}
				else if (stmt1==0 && stmt2!=0)
				{	
					result.push_back(itr->first);
					SuchThat::computeParentStar(0,itr->first,result);
				}
			}
		}

	}
}

FOLLOWS_LIST SuchThat::getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult;
	SuchThat::computeParentStar(stmt1, stmt2, firstResult);

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

void  SuchThat::computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2,list<int> &result)
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
					SuchThat::computeFollowsStar(itr->second, 0,result);
				}
				else if (stmt1==NULL && stmt2!=NULL)
				{
					result.push_back(itr->first);
					SuchThat::computeFollowsStar(0,itr->first,result);
				}
			}


		}
	}
}

FOLLOWS_LIST SuchThat::getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult;
	SuchThat::computeFollowsStar(stmt1, stmt2, firstResult);
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
	void SuchThat::computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &tmpLst, TYPE type1, TYPE type2){
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
	
	void SuchThat::computeParent(PARENT_LIST &result,PARENT_LIST tmpLst, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2)
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

	void SuchThat::computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
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

	void SuchThat::iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmpLst, int v1){
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
					break;
				}
				itr++;
			}	
		}
	}
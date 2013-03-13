#include "EvaluateFollows.h"

PKB * EvaluateFollows::pkb = NULL;

EvaluateFollows::EvaluateFollows(void)
{
}


EvaluateFollows::~EvaluateFollows(void)
{
}

FOLLOWS_LIST EvaluateFollows::getFollowsResult(TYPE type1, TYPE type2){
	FOLLOWS_LIST result, followLst;

	followLst = pkb->getFollows(0, 0);
	Helper::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST EvaluateFollows::getFollowsStarResult(TYPE type1, TYPE type2){
	FOLLOWS_LIST result, followLst, tmpLst;
	FOLLOWS_LIST::iterator followItr;

	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = pkb->getMaxProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = getFollowsStar(i, 0);

		if(!tmpLst.empty()){
			followItr = tmpLst.begin();
			while(followItr !=  tmpLst.end()){
				followLst.push_back(*followItr);
				followItr++;
			}
		}
	}

	Helper::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST EvaluateFollows::getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getFollows(s1, s2);
		computeFollows(result, tmp, type, s1, s2);
	}

	return result;
}

FOLLOWS_LIST EvaluateFollows::getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;


	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = EvaluateFollows::getFollowsStar(s1, s2);
		EvaluateFollows::computeFollows(result, tmp, type, s1, s2);
	}
	return result;
}

bool EvaluateFollows::getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if(s1 != 0 && s2 != 0 && pkb->isFollows(s1, s2) ){
		return true;
	}

	return false;
}

bool EvaluateFollows::getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if (s1!=NULL && s2!=NULL)
	{
		if (getIsFollowsResult(s1,s2))
		{
			return true;
		}
		FOLLOWS_LIST result=EvaluateFollows::getFollowsStar(s1,NULL);
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

FOLLOWS_LIST EvaluateFollows::getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult;
	EvaluateFollows::computeFollowsStar(stmt1, stmt2, firstResult);
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

void EvaluateFollows::computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
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

void  EvaluateFollows::computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2,list<int> &result)
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
					EvaluateFollows::computeFollowsStar(itr->second, 0,result);
				}
				else if (stmt1==NULL && stmt2!=NULL)
				{
					result.push_back(itr->first);
					EvaluateFollows::computeFollowsStar(0,itr->first,result);
				}
			}


		}
	}
}


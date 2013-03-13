#include "EvaluateParents.h"

PKB * EvaluateParents::pkb = NULL;

EvaluateParents::EvaluateParents(void)
{
}


EvaluateParents::~EvaluateParents(void)
{
}

PARENT_LIST EvaluateParents::getParentResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst;

	parentLst = pkb->getParent(0, 0);
	Helper::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST EvaluateParents::getParentStarResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst, tmpLst;
	PARENT_LIST::iterator parentItr;
	AST_LIST::iterator astItr_t1, astItr_t2;

	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = pkb->getMaxProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = EvaluateParents::getParentStar(i, 0);

		if(!tmpLst.empty()){
			parentItr = tmpLst.begin();
			while(parentItr !=  tmpLst.end()){
				parentLst.push_back(*parentItr);
				parentItr++;
			}
		}
	}
	Helper::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST EvaluateParents::getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getParent(s1, s2);
		EvaluateParents::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

PARENT_LIST EvaluateParents::getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = EvaluateParents::getParentStar(s1, s2);
		EvaluateParents::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

bool EvaluateParents::getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2){	
	if(s1 != 0 && s2 != 0 && pkb->isParent(s1, s2))
		return true;
	else
		return false;
}

bool EvaluateParents::getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2)
{
	if (s1!=NULL && s2!=NULL)
	{
		if (EvaluateParents::getIsParentResult(s1,s2))
		{
			return true;
		}
		PARENT_LIST result=EvaluateParents::getParentStar(s1,NULL);
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

list<int> EvaluateParents::computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2 )
{
	list<int> result;
	if (stmt1!=NULL || stmt2!=NULL)
	{
		
		stack<PARENT_LIST> stacks;
		stacks.push(pkb->getParent(stmt1, stmt2)); //root
		while (!stacks.empty())
		{
			PARENT_LIST parentChild=stacks.top();
			stacks.pop();
			if (parentChild.size()>0)
			{
				PARENT_LIST::iterator itr;
				for (itr=parentChild.begin(); itr!=parentChild.end(); itr++)
				{	

					if (stmt1!=0 && stmt2==0)

					{	
						result.push_back(itr->second);
						stacks.push(pkb->getParent(itr->second, 0));
					}
					else if (stmt1==0 && stmt2!=0)
					{	
						result.push_back(itr->first);
						stacks.push(pkb->getParent(0,itr->first));
					}
				}
			}
		}
		
	}
	return result;
}


FOLLOWS_LIST EvaluateParents::getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult=EvaluateParents::computeParentStar(stmt1, stmt2);

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





void EvaluateParents::computeParent(PARENT_LIST &result,PARENT_LIST tmpLst, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2)
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


#include "Affects.h"
#include "Helper.h"


PKB * Affects::pkb = NULL;

Affects::Affects(void)
{
}


Affects::~Affects(void)
{
}

bool Affects::getIsAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	if (stmt1!=0 && stmt2!=0)
	{
		//check if they are in same procedure
		//check if both of them are assignment statement  
		if (pkb->isInSameProc(stmt1, stmt2))
		{			
			//if there is a path between them
			if (EvaluateNext::isNextStarResult(stmt1, stmt2))
			{		

				MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, stmt1, 0);
				MODIFIES_LIST::iterator m_itr=m_list.begin();

				bool modVarUsed=false;
				int modVar=0;

				//check if stmt 2 use the variable modified by stmt1
				if (pkb->isUses(ASSIGNMENT, stmt2, m_itr->second))
				{  
					modVarUsed=true;
					modVar=m_itr->second;
				}

				//yes, stmt 2 use the variable modified by stmt1
				if (modVarUsed)
				{

					return Affects::computeIsAffect(stmt1, stmt2, modVar);

				}
		  }
		}
	}

	return false;
}

bool Affects::computeIsAffect(int starting, int ending, int varIndex)
{
	list<int> checkForDuplicate;

	stack<NEXT_LIST> stacks;
	stacks.push(EvaluateNext::getNextResult(starting,0));
	while (stacks.size()>0)
	{
		NEXT_LIST n_list=stacks.top();
		NEXT_LIST::iterator n_itr;
		stacks.pop();
		for (n_itr=n_list.begin(); n_itr!=n_list.end(); n_itr++)

		{
			if (n_itr->second==ending)
			{	
				return true;
			}
			else
			{
				if (Helper::isStatementTypeOf(CALL, n_itr->second))
				{
					string callee=pkb->getCalleeName(n_itr->second);
					int calleeIndex=pkb->getProcIndex(callee);
					if (!pkb->isModifies(PROCEDURE,calleeIndex, varIndex))
					{
						list<int>::iterator findIter = find(checkForDuplicate.begin(), checkForDuplicate.end(), n_itr->second);
						if (findIter==checkForDuplicate.end())
						{
							checkForDuplicate.push_back(n_itr->second);

							stacks.push(EvaluateNext::getNextResult(n_itr->second, 0));

						}
					}
				}
				else if (!pkb->isModifies(ASSIGNMENT,n_itr->second,varIndex))
				{	

					//this is for checking cycle
					list<int>::iterator findIter = find(checkForDuplicate.begin(), checkForDuplicate.end(), n_itr->second);
					if (findIter==checkForDuplicate.end())
					{

						checkForDuplicate.push_back(n_itr->second);

						stacks.push(EvaluateNext::getNextResult(n_itr->second, 0));

					}
				}

			}
		}

	}
	return false;
}

AFFECT_LIST Affects::getAffectResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	AFFECT_LIST answer;
	//affect (known, unknown);
	if (stmt1!=0 && stmt2==0)
	{
		//check if it is assignment type
		if (Helper::isStatementTypeOf(ASSIGNMENT, stmt1))
		{	
			//get the modifies variable
			MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, stmt1, 0);
			MODIFIES_LIST::iterator m_itr=m_list.begin();
			int modVar=m_itr->second;

			//get all the statement that uses modVar
			USES_LIST u_list=pkb->getUses(ASSIGNMENT, 0, modVar);
			USES_LIST::iterator u_itr;

			//check one by one if isaffect
			for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
			{
				if (pkb->isInSameProc(stmt1, u_itr->first))
				{
					if (Affects::getIsAffectResult(stmt1, u_itr->first))
					{
						answer.push_back(make_pair(stmt1, u_itr->first));
					}
				}
			}

		}
	}
	//affect(unknown, known);
	else if (stmt1==0 && stmt2!=0)
	{
		//check if it is assignment type
		if (Helper::isStatementTypeOf(ASSIGNMENT, stmt2))
		{
			//get all the uses variable
			USES_LIST u_list=pkb->getUses(ASSIGNMENT, stmt2, 0);
			USES_LIST::iterator u_itr;

			//for each of the uses variable, get the modifies index
			for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
			{
				MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, 0, u_itr->second);
				MODIFIES_LIST::iterator m_itr;
				//for each of the modifies index, check if they are in same proc and affect each other
				for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
				{
					if (pkb->isInSameProc(m_itr->first, stmt2))
					{
						if (Affects::getIsAffectResult(m_itr->first, stmt2))
						{
							answer.push_back(make_pair(m_itr->first, stmt2));
						}
					}
				}

			}
		}
	}
	//affect(unknown, unknown)
	else if (stmt1==0 && stmt2==0)
	{
		PROC_LIST * p_list=pkb->getAllProc();
		PROC_LIST::iterator p_itr;
		for (p_itr=p_list->begin(); p_itr!=p_list->end(); p_itr++)
		{

			for (int i=p_itr->getStartProgLine(); i<=p_itr->getEndProgLine(); i++)
			{

				if (Helper::isStatementTypeOf(ASSIGNMENT, i))
				{
					//get the modifies variable
					MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, i, 0);
					MODIFIES_LIST::iterator m_itr=m_list.begin();
					int modVar=m_itr->second;

					//get all the statement that uses modVar
					USES_LIST u_list=pkb->getUses(ASSIGNMENT, 0, modVar);
					USES_LIST::iterator u_itr;

					//check one by one if isaffect
					for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
					{
						if (pkb->isInSameProc(i, u_itr->first))
						{
							if (Affects::getIsAffectResult(i, u_itr->first) )
							{
								answer.push_back(make_pair(i, u_itr->first));
							}

						}
					}
				}
			}

		}
	}
	return answer;
}


bool Affects::getIsAffectStarResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
        if(stmt1!=0 && stmt2!=0)
		{ 
			//cout<< stmt1 << " " <<stmt2<< endl;
			if (Affects::getIsAffectResult(stmt1,stmt2))
			{
						return true;
			}
			if (pkb->isInSameProc(stmt1, stmt2))
			{
				if (EvaluateNext::isNextStarResult(stmt1, stmt2))
				{	
						return computeIsAffectStar(stmt1,stmt2);					
				}
			}
		}
    	return false;
}

	 
	bool Affects::computeIsAffectStar(int starting, int ending)
{
	MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT,starting,0);
	int modVar=m_list.begin()->second;
	stack<USES_LIST> stacks;
	stacks.push(pkb->getUses(ASSIGNMENT,0,modVar));
	USES_LIST::iterator u_itr;
	set<int> allModVarStmt;
	
	while (!stacks.empty())
	{
		USES_LIST root=stacks.top();
		stacks.pop();
		//for every subsequent statement that use modify variable
		for (u_itr=root.begin();u_itr!=root.end(); u_itr++)
		{
			
			//check if it is affected by the statement that contains modify variable
			if (Affects::getIsAffectResult(starting,u_itr->first) )
			{
			
					m_list=pkb->getModifies(ASSIGNMENT,u_itr->first,0);
					
					modVar=m_list.begin()->second;
					MODIFIES_LIST temp=pkb->getModifies(ASSIGNMENT, 0, modVar);
					MODIFIES_LIST::iterator m_itr;
					bool test=true;
					allModVarStmt.insert(u_itr->first);
					starting=u_itr->first;
					stacks.push(pkb->getUses(ASSIGNMENT,0,modVar));
						
				
			}
		
			if (u_itr->first==ending)
			{
				set<int>::iterator s_itr;
				//cout<<ending<<" "<<pkb->getVarName(modVar)<<endl;;
				for (s_itr=allModVarStmt.begin(); s_itr!=allModVarStmt.end(); s_itr++)
				{
					if (Affects::getIsAffectResult(*s_itr,ending))
					{
						return true;
					}
					
					
				}
			}
		}
	}
	return false;
	
}

AFFECT_LIST	Affects::getAffectStarResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	AFFECT_LIST result;
	stack<AFFECT_LIST> stacks;
	AFFECT_LIST::iterator a_itr;
	AFFECT_LIST::iterator findItr;
	if (stmt1!=0 && stmt2==0)
	{
		stacks.push(Affects::getAffectResult(stmt1,0));
		while (!stacks.empty())
		{
			AFFECT_LIST root=stacks.top();
			stacks.pop();
			for (a_itr=root.begin(); a_itr!=root.end(); a_itr++)
			{
				pair<int,int> temp_pair=make_pair(stmt1, a_itr->second);
				AFFECT_LIST::iterator findIter = find(result.begin(), result.end(), temp_pair);
				if (findIter==result.end())
				{
					result.push_back(temp_pair);
					stacks.push(Affects::getAffectResult(a_itr->second,0));
				}
				
			}
		}
	}
	else if (stmt1==0&& stmt2!=0)
	{
		stacks.push(Affects::getAffectResult(0,stmt2));
		while (!stacks.empty())
		{
			AFFECT_LIST root=stacks.top();
			stacks.pop();
			for (a_itr=root.begin(); a_itr!=root.end(); a_itr++)
			{
				pair<int,int> temp_pair=make_pair(a_itr->first, stmt2);
				AFFECT_LIST::iterator findIter = find(result.begin(), result.end(), temp_pair);
				if (findIter==result.end())
				{
					result.push_back(temp_pair);
					stacks.push(Affects::getAffectResult(0,a_itr->first));
				}
				
			}
		}
	}
	else if (stmt1==0 && stmt2==0)
	{
		PROC_LIST * p_list=pkb->getAllProc();
		PROC_LIST::iterator p_itr;
		set<int,int> tempResult;
		
		for (p_itr=p_list->begin(); p_itr!=p_list->end(); p_itr++)
		{

			for (int i=p_itr->getStartProgLine(); i<=p_itr->getEndProgLine(); i++)
			{
				if (Helper::isStatementTypeOf(ASSIGNMENT, i))
				{
					stacks.push(Affects::getAffectResult(i,0));
					while (!stacks.empty())
					{
						AFFECT_LIST root=stacks.top();
						stacks.pop();
						for (a_itr=root.begin(); a_itr!=root.end(); a_itr++)
						{
							pair<int,int> temp_pair=make_pair(i, a_itr->second);
							AFFECT_LIST::iterator findIter = find(result.begin(), result.end(), temp_pair);
							if (findIter==result.end())
							{
								result.push_back(temp_pair);
								stacks.push(Affects::getAffectResult(a_itr->second,0));
							}
							
						}
					}

					stacks.push(Affects::getAffectResult(0,i));
					while (!stacks.empty())
					{
						AFFECT_LIST root=stacks.top();
						stacks.pop();
						for (a_itr=root.begin(); a_itr!=root.end(); a_itr++)
						{
							pair<int,int> temp_pair=make_pair(a_itr->first, i);
							AFFECT_LIST::iterator findIter = find(result.begin(), result.end(), temp_pair);
							if (findIter==result.end())
							{
								result.push_back(temp_pair);
								stacks.push(Affects::getAffectResult(0,a_itr->first));
							}
							
						}
					}
				}
			}

		}
		
	}
	return result;
}
		


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
			if (Affects::getIsAffectResult(stmt1,stmt2))
			{
						return true;
			}
			if (pkb->isInSameProc(stmt1, stmt2))
			{
				if (EvaluateNext::isNextStarResult(stmt1, stmt2))
				{	

						MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, stmt1,0);
						MODIFIES_LIST::iterator m_itr;

						int modVar=m_list.begin()->second;

						list<int> checkDuplicate;


						NEXT_LIST::iterator n_itr;


						stack<NEXT_LIST> stacks;
						stacks.push(EvaluateNext::getNextResult(stmt1,0));
						while (!stacks.empty())
						{

							NEXT_LIST temp=stacks.top();
							stacks.pop();
							for (n_itr=temp.begin(); n_itr!=temp.end(); n_itr++)
							{

							//	cout<<n_itr->first<< " "<<n_itr->second<< ". mod var: "<<pkb->getVarName(modVar)<< endl; 
								if (n_itr->second==stmt2 )
								{	

									if (pkb->isUses(ASSIGNMENT,stmt2,modVar))
									{
										//cout<<"hehe"<<endl;
										return true;
									}
								}
								else
								{	
									if (Helper::isStatementTypeOf(CALL, n_itr->second))
									{
										string callee=pkb->getCalleeName(n_itr->second);
										int calleeIndex=pkb->getProcIndex(callee);
										if (!pkb->isModifies(PROCEDURE,calleeIndex, modVar))
										{
											list<int>::iterator findIter = find(checkDuplicate.begin(), checkDuplicate.end(), n_itr->second);
											if (findIter==checkDuplicate.end())
											{
												checkDuplicate.push_back(n_itr->second);
											}

											stacks.push(EvaluateNext::getNextResult(n_itr->second, 0));
										}
									}	

									else if(!pkb->isModifies(ASSIGNMENT,n_itr->second,modVar))
									 { 
										if (pkb->isUses(ASSIGNMENT, n_itr->second, modVar))
										{
											MODIFIES_LIST m_temp=pkb->getModifies(ASSIGNMENT, n_itr->second,0);

											modVar=m_temp.begin()->second;
										}

										list<int>::iterator findIter = find(checkDuplicate.begin(), checkDuplicate.end(), n_itr->second);
										if (findIter==checkDuplicate.end())
										{
											checkDuplicate.push_back(n_itr->second);			

										}

										stacks.push(EvaluateNext::getNextResult(n_itr->second, 0));

									}
								}
							}
						}		
				}

			}
		}
		return false;
	}

#include "Affects.h"
#include "Helper.h"
#include <process.h>

PKB * Affects::pkb = NULL;
CRITICAL_SECTION CriticalSection;
CRITICAL_SECTION CriticalSection2;


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
	set<int> checkForDuplicate;

	stack<NEXT_LIST> stacks;
					
	stacks.push(EvaluateNext::getNextResult(starting,0));
	while (!stacks.empty())
	{
		NEXT_LIST n_list=stacks.top();
		stacks.pop();
				
	    NEXT_LIST::const_iterator n_itr=n_list.cbegin();
		NEXT_LIST::const_iterator n_end_itr=n_list.cend();
						
		for (n_itr; n_itr!=n_end_itr; n_itr++)

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
					
						if (checkForDuplicate.find(n_itr->second)==checkForDuplicate.end())
						{
							checkForDuplicate.insert(n_itr->second);

							stacks.push(EvaluateNext::getNextResult(n_itr->second, 0));
						}
					}
				}
          			else if (!pkb->isModifies(ASSIGNMENT,n_itr->second,varIndex))
					{	

						//this is for checking cycle
		
         				if (checkForDuplicate.find(n_itr->second)==checkForDuplicate.end())
						{

		      				checkForDuplicate.insert(n_itr->second);

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
			USES_LIST::const_iterator u_itr=u_list.cbegin();
			USES_LIST::const_iterator u_end=u_list.cend();

			//check one by one if isaffect
			for (u_itr; u_itr!=u_end; u_itr++)
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
			USES_LIST::const_iterator u_itr=u_list.cbegin();
			USES_LIST::const_iterator u_end=u_list.cend();

			//for each of the uses variable, get the modifies index
			for (u_itr; u_itr!=u_end; u_itr++)
			{
				MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, 0, u_itr->second);
				MODIFIES_LIST::const_iterator m_itr=m_list.cbegin();
				MODIFIES_LIST::const_iterator m_end=m_list.cend();
				
				//for each of the modifies index, check if they are in same proc and affect each other
				for (m_itr; m_itr!=m_end; m_itr++)
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
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );
		InitializeCriticalSection(&CriticalSection);

		int numCPU = sysinfo.dwNumberOfProcessors;
		PROC_LIST * p_list=pkb->getAllProc();
		PROC_LIST::iterator p_itr=p_list->begin();
		int size=p_list->size();
		
		HANDLE * hThr=new HANDLE[numCPU];
		
		affectParam  *param;
		int noLoop=size/numCPU;
		for (int i=1; i<=noLoop; i++)
		{
			for (int j=1; j<=numCPU; j++)
			{
				if ((j*i)<size)
				{
					param=new affectParam;
					param->starting=p_itr->getStartProgLine();

					param->ending=p_itr->getEndProgLine();
					param->result=&answer;
					hThr[j-1]=(HANDLE) _beginthreadex( 0,0,computeGetAffect,  (void*)param, CREATE_SUSPENDED, 0);
					p_itr++;
				}else
				{
					break;
				}
				

			}
			 for (int x=0; x<numCPU; x++)
			 {
				ResumeThread(hThr[x]);
			}
			WaitForMultipleObjects(numCPU,hThr,TRUE,INFINITE);
			for (int x=0; x<numCPU; x++)
			 {
				CloseHandle(hThr[x]);
		
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
	stack<int> startingx;
	startingx.push(starting);
	USES_LIST::const_iterator u_itr;
	USES_LIST::const_iterator u_end;
	
	set<int> allModVarStmt;
	
	while (!stacks.empty())
	{
		USES_LIST root=stacks.top();
		stacks.pop();
		int tempStarting=startingx.top();
		startingx.pop();
		u_itr=root.cbegin();
		u_end=root.cend();
		//for every subsequent statement that use modify variable
		for (u_itr;u_itr!=u_end; u_itr++)
		{
			
			if (u_itr->first==ending)
			{
				
				set<int>::iterator s_itr=allModVarStmt.begin();
				
				//cout<<ending<<" "<<pkb->getVarName(modVar)<<endl;;
				while (s_itr!=allModVarStmt.end())
				{
					
					if (Affects::getIsAffectResult(*s_itr,ending))
					{
						return true;
					}
					else
					{
						s_itr=allModVarStmt.erase(s_itr++);
					}
				}
			}
			
			//check if it is affected by the statement that contains modify variable
			else if (Affects::getIsAffectResult(tempStarting,u_itr->first) )
			{
					
					MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT,u_itr->first,0);
					modVar=m_list.begin()->second;
					
					
					startingx.push(u_itr->first);
					allModVarStmt.insert(u_itr->first);
					stacks.push(pkb->getUses(ASSIGNMENT,0,modVar));
					
						
				
			}
			
		
			
		}
	}
	return false;
	
}
AFFECT_LIST	Affects::getAffectStarResult(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	AFFECT_LIST result;
	stack<AFFECT_LIST> stacks;
	AFFECT_LIST::const_iterator a_itr;
	AFFECT_LIST::const_iterator a_end;
	AFFECT_LIST::iterator findItr;
	if (stmt1!=0 && stmt2==0)
	{
		stacks.push(Affects::getAffectResult(stmt1,0));
		while (!stacks.empty())
		{
			AFFECT_LIST root=stacks.top();
			stacks.pop();
			a_itr=root.cbegin();
			a_end=root.cend();
			for (a_itr; a_itr!=a_end; a_itr++)
			{
				pair<int,int> temp_pair=make_pair(stmt1, a_itr->second);
				AFFECT_LIST::const_iterator findIter = find(result.begin(), result.end(), temp_pair);
				if (findIter==result.cend())
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
			a_itr=root.cbegin();
			a_end=root.cend();
			for (a_itr; a_itr!=a_end; a_itr++)
			{
				pair<int,int> temp_pair=make_pair(a_itr->first, stmt2);
				AFFECT_LIST::const_iterator findIter = find(result.begin(), result.end(), temp_pair);
				if (findIter==result.cend())
				{
					result.push_back(temp_pair);
					stacks.push(Affects::getAffectResult(0,a_itr->first));
				}
				
			}
		}
	}
	else if (stmt1==0 && stmt2==0)
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );
		InitializeCriticalSection(&CriticalSection2);

		int numCPU = sysinfo.dwNumberOfProcessors;
		PROC_LIST * p_list=pkb->getAllProc();
		PROC_LIST::iterator p_itr=p_list->begin();
		int size=p_list->size();
		
		HANDLE * hThr=new HANDLE[numCPU];
		
		affectParam  *param;
		int noLoop=size/numCPU;
		for (int i=1; i<=noLoop; i++)
		{
			for (int j=1; j<=numCPU; j++)
			{
				if ((j*i)<size)
				{
					param=new affectParam;
					param->starting=p_itr->getStartProgLine();
					param->ending=p_itr->getEndProgLine();
					param->result=&result;
					hThr[j-1]=(HANDLE) _beginthreadex( 0,0,computeGetAffectStar,  (void*)param, CREATE_SUSPENDED, 0);
					p_itr++;
				}else
				{
					break;
				}
				

			}
			 for (int x=0; x<numCPU; x++)
			 {
				ResumeThread(hThr[x]);
			}
			WaitForMultipleObjects(numCPU,hThr,TRUE,INFINITE);
			for (int x=0; x<numCPU; x++)
			 {
				CloseHandle(hThr[x]);
		
			}
			  
			 
		
		}
		
	}
	return result;
}
unsigned __stdcall Affects::computeGetAffect(void * pParam)
{
	affectParam *x=static_cast<affectParam*>(pParam);
	int starting=x->starting;
	int ending=x->ending;
	AFFECT_LIST *answer=x->result;
	MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, 1, 0);
	
	
	for (int i=starting; i<=ending; i++)
	{
	    if (Helper::isStatementTypeOf(ASSIGNMENT, i))
	    {
		//get the modifies variable

	  	
		MODIFIES_LIST m_list=pkb->getModifies(ASSIGNMENT, i, 0);

		int modVar=m_list.cbegin()->second;

		//get all the statement that uses modVar
					
		USES_LIST u_list=pkb->getUses(ASSIGNMENT, 0, modVar);
			
		USES_LIST::const_iterator u_itr=u_list.cbegin();
	        USES_LIST::const_iterator u_end=u_list.cend();

		//check one by one if isaffect
		 		
		for (u_itr; u_itr!=u_end; u_itr++)
		{
					
							
		    if ( Affects::getIsAffectResult(i, u_itr->first))
		    {
			EnterCriticalSection(&CriticalSection);
			answer->push_back(make_pair(i, u_itr->first));
			LeaveCriticalSection(&CriticalSection);
								
      		   } 
  
							
		}
					
            }
				
	}
	
	return 1;
}
unsigned __stdcall Affects::computeGetAffectStar(void * pParam)
{
	affectParam *x=static_cast<affectParam*>(pParam);
	int starting=x->starting;
	int ending=x->ending;
	AFFECT_LIST *answer=x->result;

	stack<AFFECT_LIST> stacks;
	AFFECT_LIST::const_iterator a_itr;
	AFFECT_LIST::const_iterator a_end;

	for (int i=starting; i<=ending; i++)
	{
		if (Helper::isStatementTypeOf(ASSIGNMENT, i))
		{
			stacks.push(Affects::getAffectResult(i,0));
			while (!stacks.empty())
			{
				AFFECT_LIST root=stacks.top();
				stacks.pop();
				a_itr=root.cbegin();
				a_end=root.cend();
				for (a_itr; a_itr!=a_end; a_itr++)
				{
					pair<int,int> temp_pair=make_pair(i, a_itr->second);
					AFFECT_LIST::const_iterator findIter = find(answer->begin(), answer->end(), temp_pair);
					if (findIter==answer->cend())
					{
						EnterCriticalSection(&CriticalSection2);
						answer->push_back(temp_pair);
						LeaveCriticalSection(&CriticalSection2);
						stacks.push(Affects::getAffectResult(a_itr->second,0));
					}
							
				}
			}
				
		}
	}
return 1;
}		


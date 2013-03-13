#include "EvaluateCalls.h"

PKB * EvaluateCalls::pkb = NULL;

EvaluateCalls::EvaluateCalls(void)
{
}


EvaluateCalls::~EvaluateCalls(void)
{
}


CALL_LIST  EvaluateCalls::getCallStarResult(PROC_NAME caller, PROC_NAME callee)
{
	set<pair<string,string>> finalResult;

	if (caller==" " && callee==" ")
	{
		set<string> caller=pkb->getAllCaller();
		 std::set<string>::iterator it;
		 for (it=caller.begin(); it!=caller.end(); it++)
		 {

			list<string> &result=computeCallStar(*it, " ");
			 if (result.size()>0)
			 {
				 list<string>::iterator itr;
				for (itr=result.begin(); itr!=result.end(); itr++)
				{

					finalResult.insert(make_pair(*it, *itr));
				}
			 }

		 }
	}
	else{

		list<string> firstResult= computeCallStar(caller, callee );

	   if (firstResult.size()>0)
	   {

		  list<string>::iterator itr;
		  for (itr=firstResult.begin(); itr!=firstResult.end(); itr++)
		  {

		 	if (caller!=" " && callee==" ")
			{

				finalResult.insert(make_pair(caller, *itr));
			}
			else if (callee!=" " && caller==" ")
			{

				finalResult.insert(make_pair(*itr, callee));
			}

		}
	  }
	}

	list <pair<string,string>> answer(finalResult.begin(), finalResult.end());

	return answer;

}
bool DesignExtractor::getIsCallResult(PROC_NAME caller, PROC_NAME callee)
{
	return pkb->isExistsCall(caller, callee);
}

bool EvaluateCalls::getIsCallStarResult(PROC_NAME caller, PROC_NAME callee)
{
	if (caller!=" " && callee!=" ")
	{
		if (pkb->isExistCall(caller, callee))
		{
			
			return true;
		}
		CALL_LIST result=EvaluateCalls::getCallStarResult(caller," ");
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

list<string>  EvaluateCalls::computeCallStar(PROC_NAME caller, PROC_NAME callee)
{	
	list<string> result;
	if (caller!=" " || callee!=" ")
	{
		
		stack<CALL_LIST> stacks;
		stacks.push(pkb->getCall(caller, callee););
		while (!stacks.empty())
		{
			CALL_LIST calls=stacks.top();
			stacks.pop();

			if (calls.size()>0)
			{
				CALL_LIST::iterator itr;

				for (itr=calls.begin(); itr!=calls.end(); itr++)
				{

					if (caller!=" " && callee==" ")
					{	
						list<string>::iterator findIter = find(result.begin(), result.end(), itr->second);
						if (findIter==result.end())
						{
							result.push_back(itr->second);
							stacks.push(pkb->getCall(itr->second, " "););
						}


					}
					else if (caller==" " && callee!=" ")
					{
						list<string>::iterator findIter = find(result.begin(), result.end(), itr->first);
						if (findIter==result.end())
						{
							result.push_back(itr->first);
							stacks.push(pkb->getCall(" ",itr->first););
						}

					}
				}


			}
		}

	}
	return result;
}

MODIFIES_LIST EvaluateCalls::computeCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	MODIFIES_LIST resultList;

	if(callStmt==0&&varIndex==0){

		vector<int>* callStmt=pkb->getAllCallerStmt();
		vector<int>::iterator s_itr;
		for (s_itr=callStmt->begin();s_itr!=callStmt->end(); s_itr++)
		{
			string calleeName=pkb->getCalleeName(*s_itr);
			if (calleeName!=" ")
			{
				MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, pkb->getProcIndex(calleeName), 0);
				MODIFIES_LIST::iterator m_itr;
				for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
				{
					resultList.push_back(make_pair(*s_itr,m_itr->second));
				}
			}
		}
	}else if(callStmt!=0&&varIndex==0){


		string calleeName=pkb->getCalleeName(callStmt);
		if (calleeName!=" ")
		{
			MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, pkb->getProcIndex(calleeName), 0);
			MODIFIES_LIST::iterator m_itr;
			for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
			{
				resultList.push_back(make_pair(callStmt,m_itr->second));
			}
		}




	}else if(callStmt==0&&varIndex!=0){


		MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, 0,varIndex);
		MODIFIES_LIST::iterator m_itr;
		MODIFIES_LIST::iterator m_itr2;
		for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
		{
			for (m_itr2=m_list.begin(); m_itr2!=m_list.end(); m_itr2++)
			{
				string callerName=pkb->getProcedure(m_itr->first)->getProcName();
				string calleeName=pkb->getProcedure(m_itr2->first)->getProcName();

				int callerStmt=pkb->getCallStmt(callerName, calleeName);
				if (callerStmt!=0)
				{	

					resultList.push_back(make_pair(callerStmt, varIndex));
				}
			}
		}
	}
	return resultList;
}


bool EvaluateCalls::isCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	if (callStmt!=0 && varIndex!=-1)
	{
		MODIFIES_LIST m_list=EvaluateCalls::computeCallModifies(callStmt, 0);
		MODIFIES_LIST::iterator m_itr;
		for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
		{
			if (m_itr->first==callStmt && m_itr->second==varIndex)
			{
				return true;
			}
		}
	}

	return false;
}
USES_LIST EvaluateCalls::computeCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	USES_LIST resultList;

	if(callStmt==0&&varIndex==0){

		vector<int>* callStmt=pkb->getAllCallerStmt();
		vector<int>::iterator s_itr;
		for (s_itr=callStmt->begin();s_itr!=callStmt->end(); s_itr++)
		{
			string calleeName=pkb->getCalleeName(*s_itr);
			if (calleeName!=" ")
			{
				USES_LIST u_list=pkb->getUses(PROCEDURE, pkb->getProcIndex(calleeName), 0);
				USES_LIST::iterator u_itr;
				for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
				{
					resultList.push_back(make_pair(*s_itr,u_itr->second));
				}
			}
		}
	}else if(callStmt!=0&&varIndex==0){


		string calleeName=pkb->getCalleeName(callStmt);
		if (calleeName!=" ")
		{
			USES_LIST u_list=pkb->getUses(PROCEDURE, pkb->getProcIndex(calleeName), 0);
			USES_LIST::iterator u_itr;
			for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
			{
				resultList.push_back(make_pair(callStmt,u_itr->second));
			}
		}




	}else if(callStmt==0&&varIndex!=0){


		USES_LIST u_list=pkb->getUses(PROCEDURE, 0,varIndex);
		USES_LIST::iterator u_itr;
		USES_LIST::iterator u_itr2;
		for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
		{
			for (u_itr2=u_list.begin(); u_itr2!=u_list.end(); u_itr2++)
			{
				string callerName=pkb->getProcedure(u_itr->first)->getProcName();
				string calleeName=pkb->getProcedure(u_itr2->first)->getProcName();

				int callerStmt=pkb->getCallStmt(callerName, calleeName);
				if (callerStmt!=0)
				{	

					resultList.push_back(make_pair(callerStmt, varIndex));
				}
			}
		}
	}
	return resultList;
}


bool EvaluateCalls::isCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	if (callStmt!=0 && varIndex!=0)
	{

		USES_LIST u_list=EvaluateCalls::computeCallUses(callStmt, 0);
		USES_LIST::iterator u_itr;
		for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
		{

			if (u_itr->first==callStmt && u_itr->second==varIndex)
			{
				return true;
			}
		}
	}

	return false;
}

void EvaluateCalls::insertProcModifiesUses()
{
	list<CALL_PAIR> procs=EvaluateCalls::getCallStarResult(" "," ");
	list<CALL_PAIR>::iterator c_itr;

	for (c_itr=procs.begin(); c_itr!=procs.end(); c_itr++)
	{  
		if (c_itr->first!=c_itr->second)
		{
			int callerIndex=pkb->getProcIndex(c_itr->first);
			int calleeIndex=pkb->getProcIndex(c_itr->second);

			MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, calleeIndex, 0);
			USES_LIST u_list=pkb->getUses(PROCEDURE, calleeIndex, 0);

			MODIFIES_LIST::iterator m_itr;
			USES_LIST::iterator u_itr;

			for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
			{
				pkb->insertModifies(PROCEDURE, callerIndex, m_itr->second);
			}

			for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
			{
				pkb->insertUses(PROCEDURE, callerIndex, u_itr->second);
			}		
		}		
	}
}

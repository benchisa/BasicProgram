#pragma once
#include "CallTable.h"

CallTable::CallTable(void){

	callDictionary=new unordered_multimap<PROC_NAME, pair<int,PROC_NAME>>();
	counter=0;
}
CallTable::~CallTable(void){}

CALL_INDEX CallTable::insertCall(PROC_NAME caller,STATEMENT_NUM callStmt, PROC_NAME callee)
{
	if (callStmt>0 && caller!= " " && callee != " ")
	{
		callItr c_itr=callDictionary->find(caller);
		if (c_itr==callDictionary->end())
		{
			callDictionary->insert(make_pair(caller, make_pair(callStmt, callee)));
			
		}
		else
		{
			bool exist=false;
			int internalCounter=1;
		    for (c_itr; c_itr!=callDictionary->end(); c_itr++)
			{
			
				if (c_itr->first==caller )
				{	
					pair<int,PROC_NAME> temp=c_itr->second;

					if (temp.first==callStmt && temp.second!=callee)
					{
						return -1;
					}
					else if (temp.first==callStmt && temp.second==callee)
					{
						exist=true;
						break;
					}
						
				}
				internalCounter++;
			}

			if (!exist)
			{
				callDictionary->insert(make_pair(caller, make_pair(callStmt, callee)));
				
			}
			else
			{
				return internalCounter;
			}
		}

		counter++;
		return counter;
	}
	else
	{
		return -1;
	}
}

CALL_PAIR CallTable::getCallPair(CALL_INDEX index)
{	
	CALL_PAIR answer=make_pair(" ", " ");
	if (counter<index || index<1)
	{
		return answer;
	}
	else
	{
		callItr c_itr;
	int internalCounter=1;
	
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		if (internalCounter==index)
		{
			answer=make_pair(c_itr->first, c_itr->second.second);
		}
		internalCounter++;
	}
	return answer;
	}
	
}

CALL_INDEX CallTable::getCallPairIndex(PROC_NAME caller,PROC_NAME callee)
{
	if (caller!=" " && callee!= " ")
	{
		callItr c_itr;
		int internalCounter=1;
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{

			if (c_itr->first==caller && c_itr->second.second==callee)
			{
			return internalCounter;
			}
			internalCounter++;
		}
	}
	
	return -1;
	
	
}

bool CallTable::isExistsCall(PROC_NAME caller,PROC_NAME callee)
{
	if (caller!=" " && callee!= " ")
	{
		callItr c_itr;
		int internalCounter=1;
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{

			if (c_itr->first==caller && c_itr->second.second==callee)
			{
				return true;
			}

		}
	}
	
		return false;

	
}

list<CALL_PAIR> CallTable::getCall(PROC_NAME caller,PROC_NAME callee)
{
	list<CALL_PAIR>  answer;
	callItr c_itr;
	if (caller!=" " || callee!=" ")
	{
		
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{
		
			if (caller!=" " && callee==" ")
			{
				if (c_itr->first==caller)
				{
					answer.push_back(CALL_PAIR(caller, c_itr->second.second));
				}
			}
			else if (caller==" "  && callee!=" ")
			{
				if (c_itr->second.second==callee)
				{
					answer.push_back(CALL_PAIR(c_itr->first, callee));
				}
			} 
		}
	}
	else
	{
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{
			answer.push_back(CALL_PAIR(c_itr->first, c_itr->second.second));
		}
	}
	

	return answer;
}

CALL_LIST   CallTable::getAllCalls()
{
	CALL_LIST  answer;

	callItr c_itr;
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		answer.push_back(make_pair(c_itr->first, c_itr->second.second));
	}

	return answer;
}
SIZE CallTable::getCallTableSize()
{
	return counter;	
}

set<string> CallTable::getAllCaller()
{
	set<string> answer;
	callItr c_itr;
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		answer.insert(c_itr->first);
	}
	return answer;
}

string CallTable::getCallerName(STATEMENT_NUM cIndex)
{
	callItr c_itr;
	
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		if (c_itr->second.first==cIndex)
		{
			return c_itr->first;
		}

	}
	return " ";
}

string CallTable::getCalleeName(STATEMENT_NUM cIndex)
{
	callItr c_itr;
	
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		if (c_itr->second.first==cIndex)
		{
			return c_itr->second.second;
		}
	}
	return " ";
}

DATA_LIST CallTable::getAllCallerStmt()
{
	DATA_LIST answer;
	callItr c_itr;
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		answer.push_back(c_itr->second.first);
	}
	return answer;
}
STATEMENT_NUM CallTable::getCallStmt(PROC_NAME caller, PROC_NAME callee)
{
	callItr c_itr;
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		if (c_itr->first==caller && c_itr->second.second==callee)
		{
			return c_itr->second.first;
		}
	}
	return 0;
}
bool CallTable::isExistCallStmt(STATEMENT_NUM callStmt)
{
	callItr c_itr;
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		if (c_itr->second.first==callStmt)
		{
			return true;
		}
	}
	return false;
}

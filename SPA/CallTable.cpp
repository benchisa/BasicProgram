#pragma once
#include "CallTable.h"
#include "Procedure.h"
CallTable::CallTable(void){

  callDictionary=new unordered_multimap<Procedure*, Procedure*>;
	counter=0;
}
CallTable::~CallTable(void){}

CALL_INDEX CallTable::insertCall(Procedure * caller, Procedure* callee)
{

	if (caller!=nullptr && callee!=nullptr)
	{   
		
		callItr c_itr=callDictionary->find(caller);
		if (c_itr==callDictionary->end())
		{
			callDictionary->insert(CALL_PAIR(caller, callee));
			
		}
		else
		{
			bool exist=false;
			
		    for (c_itr; c_itr!=callDictionary->end(); c_itr++)
			{
			
				if (c_itr->first==caller &&c_itr->second==callee)
				{	
					exist=true;
					break;
				}
			}
			if (!exist)
			{
				callDictionary->insert(CALL_PAIR(caller, callee));
				
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
	callItr c_itr;
	int counter=1;
	CALL_PAIR answer=make_pair(nullptr, nullptr);
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		if (counter==index)
		{
			answer=make_pair(c_itr->first, c_itr->second);
		}
		counter++;
	}
	return answer;
}

CALL_INDEX CallTable::getCallPairIndex(PROC_NAME caller,PROC_NAME callee)
{
	if (caller!=" " && callee!= " ")
	{
		callItr c_itr;
		int counter=1;
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{

			if (c_itr->first->getProcName()==caller && c_itr->second->getProcName()==callee)
			{
			return counter;
			}
			counter++;
		}
	}
	else
	{
		return -1;
	}
	
}

bool CallTable::isExistsCall(PROC_NAME caller,PROC_NAME callee)
{
	if (caller!=" " && callee!= " ")
	{
		callItr c_itr;
		int counter=1;
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{

			if (c_itr->first->getProcName()==caller && c_itr->second->getProcName()==callee)
			{
				return true;
			}

		}
	}
	else
	{
		return false;

	}
}

list<CALL_PAIR> CallTable::getCall(PROC_NAME caller,PROC_NAME callee)
{
	list<CALL_PAIR> answer;
	callItr c_itr;
	if (caller!=" " && callee!=" ")
	{
		
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{
		
		if (caller!=" " && callee==" ")
		{
			if (c_itr->first->getProcName()==caller)
			{
				answer.push_back(CALL_PAIR(c_itr->first, c_itr->second));
			}
		}
		else if (caller==" "  && callee!=" ")
		{
			if (c_itr->second->getProcName()==callee)
			{
				answer.push_back(CALL_PAIR(c_itr->first, c_itr->second));
			}
		} 
	}
	}
	else
	{
		for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
		{
			answer.push_back(CALL_PAIR(c_itr->first, c_itr->second));
		}
	}
	

	return answer;
}

CALL_LIST *  CallTable::getAllCalls()
{
	CALL_LIST * answer=new CALL_LIST;

	callItr c_itr;
	for (c_itr=callDictionary->begin(); c_itr!=callDictionary->end(); c_itr++)
	{
		answer->push_back(make_pair(c_itr->first->getProcName(), c_itr->second->getProcName()));
	}

	return answer;
}

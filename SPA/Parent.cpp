#pragma once
#include "Parent.h"
#include <utility>

using namespace std;

//This is a constructor
Parent::Parent(void)
{
	parentDictionary = new multimap<int, int>;
}
//This is a destructor
Parent::~Parent(void){
	delete parentDictionary;
}
bool Parent::insertParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	if (stmt1!=NULL && stmt2!=NULL)
	{
		parentItr c_itr=parentDictionary->find(stmt1);
		if (c_itr==(*parentDictionary).end())
		{
			(*parentDictionary).insert(parentPair(stmt1, stmt2));
			return true;
		}
		else
		{	bool exist=false;
			
		    for (c_itr; c_itr!=parentDictionary->end(); c_itr++)
			{
			
				if (c_itr->first==stmt1 &&c_itr->second==stmt2)
				{	
					exist=true;
					break;
				}
			}
			if (!exist)
			{
				parentDictionary->insert(parentPair(stmt1, stmt2));
				return true;
			}

		}
	}
	return false;
}


bool Parent::isParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	if (stmt1!=NULL && stmt2!=NULL)
	{
			parentItr c_itr=(*parentDictionary).find(stmt1);
			if (c_itr!=(*parentDictionary).end())
			{
				while (c_itr!=parentDictionary->end())
				{
					if (c_itr->first==stmt1 && c_itr->second==stmt2)
					{
						
							return true;
					}
					c_itr++;
				}
			}

		
	}

	return false;
}

PARENT_LIST Parent::getParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> lists;

	if (stmt1!=NULL && stmt2==NULL)
	{
		parentItr c_itr=parentDictionary->find(stmt1);
		if (c_itr!=(*parentDictionary).end())
		{
			for (c_itr;  c_itr != parentDictionary->end();  c_itr++)
			{	
				if (c_itr->first==stmt1)
				{
					lists.push_back(make_pair(stmt1,(*c_itr).second));
				}
				
			}
		}
	}
	else if (stmt2!=NULL && stmt1==NULL)
	{
		parentItr c_itr;
		for (c_itr=parentDictionary->begin(); c_itr!=parentDictionary->end(); c_itr++)
		{
			if (c_itr->second==stmt2)
			{
				lists.push_back(make_pair(c_itr->first, stmt2));
			}
		}
	}
	else if (stmt2==NULL && stmt1==NULL)
	{
		parentItr c_itr;
		for (c_itr=parentDictionary->begin(); c_itr!=parentDictionary->end(); c_itr++)
		{
			lists.push_back(make_pair(c_itr->first, c_itr->second));
		}
	}
	lists.sort();
	lists.unique();
	return lists;
}

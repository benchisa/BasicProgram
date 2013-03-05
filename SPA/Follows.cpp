#pragma once
#include "Follows.h"

using namespace std;


Follows::Follows()
{
	followsTable=new unordered_map<int, int>;
}
Follows::~Follows()
{
	delete followsTable;
}

bool Follows::insertFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	if (stmt1!=NULL && stmt2!=NULL)
	{
		
			if (followsTable->find(stmt1)==followsTable->end())
			{
				followsTable->insert(followsPair(stmt1, stmt2));
				return true;
			}
			
		
	}
	return false;
}

bool Follows::isFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	if (stmt1!=NULL && stmt2!=NULL)
	
	{
		if (!followsTable->empty())
		
		{
			followsItr=followsTable->find(stmt1);
			if (followsItr!=followsTable->end())
			{
				if (followsItr->first==stmt1 && followsItr->second==stmt2)
				{
					return true;
				}
				
			}	
		}
	}
	return false;
}

FOLLOWS_LIST Follows::getFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int, int>> answerList;
   if (stmt1!=NULL && stmt2==NULL)
   {
	   if (!followsTable->empty())
	   {
		   followsItr=followsTable->find(stmt1);
		   if (followsItr!=followsTable->end())
		   {
			   answerList.push_back(make_pair(stmt1, followsItr->second));
		   }
	   }
   }
   else if (stmt1==NULL && stmt2!=NULL)

   {
	   
	  
	   for (followsItr=followsTable->begin(); followsItr!=followsTable->end(); followsItr++)
	   {
		   if (followsItr->second==stmt2)
		   {
			   
			   answerList.push_back(make_pair(followsItr->first, stmt2))	;
			   break;
		   }
	   }
   }
   else if (stmt1==NULL && stmt2==NULL)
   {
	   for (followsItr=followsTable->begin(); followsItr!=followsTable->end(); followsItr++)
	   {
		   answerList.push_back(make_pair(followsItr->first, followsItr->second));	
	   }
   }
   return answerList;

}
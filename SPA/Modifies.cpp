#pragma once
#include "Modifies.h"
#include <utility>
using namespace std;



Modifies::Modifies(void)
{
	modifiesDictionary=new concurrent_unordered_multimap<int, concurrent_unordered_multimap<int,int>>;
	
}
Modifies::~Modifies(void)
{
	delete modifiesDictionary;
}


bool Modifies::insertModifies(TYPE type, int index, int varIndex)
{
	concurrent_unordered_multimap<int,concurrent_unordered_multimap<int, int>>::iterator firstItr;
	 concurrent_unordered_multimap<int,int>::iterator secondItr;
	//check type is allowed
	if (type==ASSIGNMENT|| type==WHILE|| type==IF|| type==PROCEDURE)
	{	//check  both type are not null
		if (index!=NULL && varIndex!=NULL)
		{
			firstItr=modifiesDictionary->find(type);
			//there is no such key "type" in the data structure
			if (firstItr==modifiesDictionary->end())
			{   //create a new element and insert it
				concurrent_unordered_multimap<int, int> inner;
				inner.insert(pair<int, int>(index, varIndex));
				modifiesDictionary->insert(pair<int, concurrent_unordered_multimap<int, int>>(type, inner));
				return true;
			}
			else
			{	//there exist a key of type
				bool indexKeyExist=false;
				concurrent_unordered_multimap<int, int> *inner;
				//so we loop through the data structure
				while (firstItr!=modifiesDictionary->end())
				{
					//check if the key==type, then check if index exist..
					if (firstItr->first==type)
					{
						inner=&(*firstItr).second		;
						secondItr=inner->find(index);

						if (secondItr!=inner->end())
						{

							indexKeyExist=true;
							break;
						}		
					}
					firstItr++;
				}
				//if index does not exist, then we create a concurrent_unordered_multimap<index, varIndex) and insert into the data structure
				if (!indexKeyExist)
				{
					concurrent_unordered_multimap<int, int> newInner;
					newInner.insert(pair<int, int>(index, varIndex));
					modifiesDictionary->insert(pair<int, concurrent_unordered_multimap<int, int>>(type, newInner));
					return true;
				}
				else
				{	
					//index exist. we check if varIndex is available as the value. if It is, we ignore, else we insert a new row/record

					secondItr=inner->begin();
					bool valueExist=false;
					while (secondItr!=inner->end())
					{
						if (secondItr->second==varIndex)
						{
							valueExist=true;
							break;
						}

						secondItr++;
					}
					if (!valueExist)
					{
						inner->insert(pair<int, int>(index, varIndex));
						return true;
					}

				}
			}
		}

	}
	return false;

}
list<pair<int,int>> Modifies::getModifies(TYPE type, int index, int varIndex)
{

	list<pair<int,int>> result;
	//check only allowed type
	if (type==ASSIGNMENT|| type==WHILE|| type==IF|| type==PROCEDURE)
	{
		
	concurrent_unordered_multimap<int,concurrent_unordered_multimap<int, int>>::const_iterator firstItr;
	concurrent_unordered_multimap<int,concurrent_unordered_multimap<int,int>>::const_iterator first_end_itr=modifiesDictionary->cend();
	 concurrent_unordered_multimap<int,int>::const_iterator secondItr;
	 concurrent_unordered_multimap<int,int>::const_iterator second_end_itr;
		//check both are not null
		if (index!=0 && varIndex==0)
		{
			//check there is a key "type" in the data structure
			
			firstItr=modifiesDictionary->find(type);
		
			if (firstItr!=first_end_itr)
			{
				bool indexKeyExist=false;
				concurrent_unordered_multimap<int, int> *inner;
				//loop through the first layer to see if there exist a secondary key "index"
				while (firstItr!=first_end_itr)
				{
					if (firstItr->first==type)
					{
						inner=&firstItr->second;
						secondItr=inner->find(index);
						second_end_itr=inner->cend();
						if (secondItr!=second_end_itr)
						{
							indexKeyExist=true;
							break;
						}	
					}
					else
					{
						break;
					}
				
					firstItr++;
				
				}
				//it exist, so we continue loop through and add all the values;
				if (indexKeyExist)
				{
					
					while (secondItr!=second_end_itr)
					{	
					
						result.push_back(make_pair(index,secondItr->second));
						secondItr++;
						
						
					}
				}
			}
			
		}
		else if (varIndex!=0 && index==0)
			{
				//check if there is a key type
				firstItr=modifiesDictionary->find(type);
				if (firstItr!=first_end_itr)
				{
					
					concurrent_unordered_multimap<int, int> *inner;
					//loop through everything, see if value=varIndex. if it is, add the secondary key(index) to the result
					while (firstItr!=first_end_itr)
					{
						if (firstItr->first==type)
						{
							inner=&firstItr->second;
							secondItr=inner->begin();
							second_end_itr=inner->cend();
							if (secondItr!=second_end_itr)
							{
								while (secondItr!=second_end_itr)
								{
									if (secondItr->second==varIndex)
									{
										
										result.push_back(make_pair(secondItr->first, varIndex));
										
									}
								
									secondItr++;
									
								}
							}	
						}
						else
						{
							break;
						}
						
						firstItr++;
						
					}
				}
			}
		else if (index==NULL && varIndex==NULL)
		{
			firstItr=modifiesDictionary->find(type);
			if (firstItr!=first_end_itr)
			{
				for (firstItr; firstItr!=first_end_itr; firstItr++)
				{
					//add the concurrent_unordered_multimap belong to key type
					if (firstItr->first==type)
					{
						concurrent_unordered_multimap<int, int>* inner=&firstItr->second;
						secondItr=inner->begin();
						second_end_itr=inner->cend();
						while (secondItr!=second_end_itr)
						{
							
							result.push_back(make_pair(secondItr->first, secondItr->second));
							secondItr++;
							
						}
						
					}
					
					
				}
			}
		}
	}
	
	result.sort();
	result.unique();
	
	return result;
}


bool Modifies::isModifies(TYPE type, int index, int varIndex)
{	//check only allowed type
	if (type==ASSIGNMENT|| type==WHILE|| type==IF|| type==PROCEDURE)
	{
		concurrent_unordered_multimap<int,concurrent_unordered_multimap<int, int>>::const_iterator firstItr;
		concurrent_unordered_multimap<int,concurrent_unordered_multimap<int, int>>::const_iterator first_end_itr=modifiesDictionary->cend();
		concurrent_unordered_multimap<int,int>::const_iterator secondItr;
		concurrent_unordered_multimap<int,int>::const_iterator second_end_itr;
		//check both not null
		if (index!=NULL && varIndex!=NULL)
		{
			//check if key type available
			firstItr=modifiesDictionary->find(type);
			if (firstItr!=first_end_itr)
			{
				bool indexKeyExist=false;
				concurrent_unordered_multimap<int, int> *inner;
				//loop to check if index is available
				while (firstItr!=first_end_itr)
				{
					if (firstItr->first==type)
					{
						inner=&firstItr->second;
						secondItr=inner->find(index);
						second_end_itr=inner->cend();

						if (secondItr!=second_end_itr)
						{
							indexKeyExist=true;

							break;
						}	
					}
					else
					{
						break;
					}
					firstItr++;
				}
				//if it is, check if the one of the value==varIndex. if it is, return true;
				if (indexKeyExist)
				{
					

					while (secondItr!=second_end_itr)
					{
						if (secondItr->second==varIndex)
						{
							return true;
						}

						secondItr++;
					}
				}
			}
		}

	}
	return false;
}

void Modifies::debug()
{
	concurrent_unordered_multimap<int, concurrent_unordered_multimap<int, int>>::iterator firstItr=modifiesDictionary->begin();

	while (firstItr!=modifiesDictionary->end())
	{


		concurrent_unordered_multimap<int, int> inner=(*firstItr).second;
		concurrent_unordered_multimap<int, int>::iterator secondItr=inner.begin();
		if (secondItr!=inner.end())
		{

			while (secondItr!=inner.end())
			{


			//	cout<<firstItr->first<<" "<<secondItr->first<< " "<<secondItr->second<< endl; 

				secondItr++;
			}
		}

		firstItr++;
	}

}




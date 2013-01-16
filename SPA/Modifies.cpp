#pragma once
#include "Modifies.h"
#include <utility>
using namespace std;


Modifies::Modifies(void)
{
	modifiesDictionary=new multimap<int, multimap<int,int>>;
}
Modifies::~Modifies(void)
{
	delete modifiesDictionary;
}


bool Modifies::insertModifies(TYPE type, int index, int varIndex)
{
	//check type is allowed
	if (type==ASSIGNMENT|| type==WHILE|| type==IF|| type==PROCEDURE)
	{	//check  both type are not null
		if (index!=NULL && varIndex!=NULL)
		{
			firstItr=modifiesDictionary->find(type);
			//there is no such key "type" in the data structure
			if (firstItr==modifiesDictionary->end())
			{   //create a new element and insert it
				multimap<int, int> inner;
				inner.insert(pair<int, int>(index, varIndex));
				modifiesDictionary->insert(pair<int, multimap<int, int>>(type, inner));
				return true;
			}
			else
			{	//there exist a key of type
				bool indexKeyExist=false;
				multimap<int, int> *inner;
				//so we loop through the data structure
				while (firstItr!=modifiesDictionary->end())
				{
					//check if the key==type, then check if index exist..
					if (firstItr->first==type)
					{
						inner=&(*firstItr).second;
						secondItr=inner->find(index);

						if (secondItr!=inner->end())
						{

							indexKeyExist=true;
							break;
						}		
					}
					firstItr++;
				}
				//if index does not exist, then we create a multimap<index, varIndex) and insert into the data structure
				if (!indexKeyExist)
				{
					multimap<int, int> newInner;
					newInner.insert(pair<int, int>(index, varIndex));
					modifiesDictionary->insert(pair<int, multimap<int, int>>(type, newInner));
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
		//check both are not null
		if (index!=0 && varIndex==0)
		{
			//check there is a key "type" in the data structure
			firstItr=modifiesDictionary->find(type);
			if (firstItr!=modifiesDictionary->end())
			{
				bool indexKeyExist=false;
				multimap<int, int> *inner;
				//loop through the first layer to see if there exist a secondary key "index"
				while (firstItr!=modifiesDictionary->end())
				{
					if (firstItr->first==type)
					{
						inner=&firstItr->second;
						secondItr=inner->find(index);
						if (secondItr!=inner->end())
						{
							indexKeyExist=true;
							break;
						}	
					}
					firstItr++;
				}
				//it exist, so we continue loop through and add all the values;
				if (indexKeyExist)
				{
					secondItr=inner->begin();
					while (secondItr!=inner->end())
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
				if (firstItr!=modifiesDictionary->end())
				{
					
					multimap<int, int> *inner;
					//loop through everything, see if value=varIndex. if it is, add the secondary key(index) to the result
					while (firstItr!=modifiesDictionary->end())
					{
						if (firstItr->first==type)
						{
							inner=&firstItr->second;
							secondItr=inner->begin();
							if (secondItr!=inner->end())
							{
								while (secondItr!=inner->end())
								{
									if (secondItr->second==varIndex)
									{
										result.push_back(make_pair(secondItr->first, varIndex));
									}
									secondItr++;
								}
							}	
						}
						firstItr++;
					}
				}
			}
		else if (index==NULL && varIndex==NULL)
		{
			firstItr=modifiesDictionary->find(type);
			if (firstItr!=modifiesDictionary->end())
			{
				for (firstItr; firstItr!=modifiesDictionary->end(); firstItr++)
				{
					//add the multimap belong to key type
					if (firstItr->first==type)
					{
						multimap<int, int> inner=firstItr->second;
						for (secondItr=inner.begin(); secondItr!=inner.end(); secondItr++)
						{
							result.push_back(make_pair(secondItr->first, secondItr->second));
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
		//check both not null
		if (index!=NULL && varIndex!=NULL)
		{
			//check if key type available
			firstItr=modifiesDictionary->find(type);
			if (firstItr!=modifiesDictionary->end())
			{
				bool indexKeyExist=false;
				multimap<int, int> *inner;
				//loop to check if index is available
				while (firstItr!=modifiesDictionary->end())
				{
					if (firstItr->first==type)
					{
						inner=&firstItr->second;
						secondItr=inner->find(index);
						if (secondItr!=inner->end())
						{
							indexKeyExist=true;

							break;
						}	
					}
					firstItr++;
				}
				//if it is, check if the one of the value==varIndex. if it is, return true;
				if (indexKeyExist)
				{
					secondItr=inner->begin();

					while (secondItr!=inner->end())
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
	multimap<int, multimap<int, int>>::iterator firstItr=modifiesDictionary->begin();

	while (firstItr!=modifiesDictionary->end())
	{


		multimap<int, int> inner=(*firstItr).second;
		multimap<int, int>::iterator secondItr=inner.begin();
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
#pragma once
#include "ConstantTable.h"
ConstantTable::ConstantTable(void)
{
  counter=0;
}
ConstantTable::~ConstantTable(void)
{

}

CONSTANT_INDEX ConstantTable::insertConst(int constantValue)
{
	
	int internalCounter=1;
	listItr l_itr;
	for (l_itr=constantList.begin(); l_itr!=constantList.end(); l_itr++)
    {   
		if (*l_itr==constantValue)
		{
			return internalCounter;
		}
		 internalCounter++;
	}

	constantList.push_back(constantValue);
	counter++;
	return counter;


}
SIZE ConstantTable::getConstantTableSize()
{
	return constantList.size();
}

int ConstantTable::getConstantValue(CONSTANT_INDEX index)
{
	int internalCounter=1;
	listItr l_itr;
	for (l_itr=constantList.begin(); l_itr!=constantList.end(); l_itr++)
    {   
		if (index==internalCounter)
		{
			return *l_itr;
		}
		internalCounter++;
	}
	return -1;
	
}
CONSTANT_INDEX ConstantTable::getConstantIndex(int constantValue)
{
	int internalCounter=1;
	listItr l_itr;
	for (l_itr=constantList.begin(); l_itr!=constantList.end(); l_itr++)
    {   
		if (constantValue==*l_itr)
		{
			return internalCounter;
		}
		internalCounter++;
	}
	return -1;
}

bool ConstantTable::isExists(int constantValue)
{
	listItr l_itr;
	for (l_itr=constantList.begin(); l_itr!=constantList.end(); l_itr++)

	{
		if (constantValue==*l_itr)
		{
			return true;
		}

	}
	return false;
}

CONSTANT_LIST *  ConstantTable::getAllConstant()
{
	list<int>* answer=new list<int>;
	listItr l_itr;
	for (l_itr=constantList.begin(); l_itr!=constantList.end(); l_itr++)

	{
		answer->push_back(*l_itr);
	}
	return answer;

}

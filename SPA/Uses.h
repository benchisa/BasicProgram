#pragma once 
#include "Global_Data_Abstraction.h"
#include <utility>
#include <list>


class Uses
{
public:
	Uses(void);
	~Uses(void);

	bool insertUses(TYPE type, USES_INDEX index,VAR_INDEX varIndex);
	//type can not be NULL. Take care of three cases:
	//1. getUses(type, empty, 1)
	//2. getUses(type,1,empty)
	//3. getUses(type,empty, empty)
	USES_LIST getUses(TYPE type, USES_INDEX index,VAR_INDEX varIndex);
	bool isUses(TYPE type, USES_INDEX index,VAR_INDEX varIndex);
	USES_LIST getUsesIndexInSameProc(STATEMENT_NUM starting, STATEMENT_NUM ending,VAR_INDEX varIndex);
	void debug();

private:
	concurrent_unordered_multimap<int,concurrent_unordered_multimap<int, int>> *usesDictionary;
	 
	 
	
};
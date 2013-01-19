#pragma once
#include "Global_Data_Abstraction.h"
class CallTable
{
public:
	CallTable(void);
	~CallTable(void);

	CALL_INDEX insertCall(Procedure * caller, Procedure* callee);
	SIZE getCallTableSize();

	//if caller is empty, return callee procedure
	//if callee is empty, return caller procedure
	//if both empty, return all
	list<CALL_PAIR> getCall(PROC_NAME caller,PROC_NAME callee);

	CALL_INDEX getCallPairIndex(PROC_NAME caller,PROC_NAME callee);
	CALL_PAIR getCallPair(CALL_INDEX index);
	bool isExistsCall(PROC_NAME caller,PROC_NAME callee);
	CALL_LIST *  getAllCalls();

private:
	int counter;
	unordered_multimap<Procedure*,Procedure*> *callDictionary;
	typedef unordered_multimap<Procedure*,Procedure*>::iterator callItr;
};

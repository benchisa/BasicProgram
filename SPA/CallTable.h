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
	Procedure * getCall(PROC_NAME caller,PROC_NAME callee);

	CALL_INDEX getCallPairIndex(PROC_NAME caller,PROC_NAME callee);
	CALL_PAIR getCALLPair(CALL_INDEX index);
	bool isExistsCall(PROC_NAME caller,PROC_NAME callee);
	CALL_LIST *  getAllCalls();
};


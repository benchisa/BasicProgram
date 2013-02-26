#pragma once
#include "Global_Data_Abstraction.h"

class CallTable
{
public:
	CallTable(void);
	~CallTable(void);

	CALL_INDEX insertCall(PROC_NAME caller, STATEMENT_NUM callStmt,PROC_NAME callee);
	SIZE getCallTableSize();

	//if caller is empty, return callee procedure
	//if callee is empty, return caller procedure
	CALL_LIST  getCall( PROC_NAME caller,PROC_NAME callee);

	CALL_INDEX getCallPairIndex(PROC_NAME caller,PROC_NAME callee);
	CALL_PAIR getCallPair(CALL_INDEX index);
	bool isExistsCall(PROC_NAME caller,PROC_NAME callee);
	CALL_LIST   getAllCalls();
	set<string> getAllCaller();
	
	string getCallerName(STATEMENT_NUM cIndex);
	string getCalleeName(STATEMENT_NUM callerStatement);
	int getCallerStmt(PROC_NAME calleeName);
	set<int> getAllCallerStmt();
	STATEMENT_NUM getCallStmt(PROC_NAME caller, PROC_NAME callee);

private:
	int counter;
	
	unordered_multimap<PROC_NAME, pair<int,PROC_NAME>> *callDictionary;
	typedef unordered_multimap<PROC_NAME,pair<int,PROC_NAME>>::iterator callItr;
};

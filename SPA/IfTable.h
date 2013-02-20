#pragma once
#include "Global_Data_Abstraction.h"

class IfTable
{
public:
	IfTable(void);
	~IfTable(void);
		
	typedef RAWDATA IFTABLE;

	SIZE getSize();
	void insert(STATEMENT_NUM stmtNo,INDEX ctrVarIndex);
	INDEX getCtrVar(STATEMENT_NUM stmtNo);
	DATA_LIST * getIfs(INDEX ctrVarIndex);
	DATA_LIST * getAllIfs();

private:
	IFTABLE ifTable;
	DATA_LIST ifStmtList;
	IFTABLE::iterator itr;
};


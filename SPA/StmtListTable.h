#pragma once
#include "Global_Data_Abstraction.h"
class StmtListTable
{
public:
	StmtListTable(void);
	~StmtListTable(void);

	typedef hash_map<int,TYPE> STMTLISTTABLE;
	
	SIZE getSize();
	void insert(STATEMENT_NUM stmtNo,TYPE type);
	STMTENTRY getStmtListEntry(STATEMENT_NUM stmtNo);
	DATA_LIST * getAllStmtLists();

private:
	STMTLISTTABLE stmtTable;
	STMTLISTTABLE::iterator itr;
	DATA_LIST stmtList;
};

